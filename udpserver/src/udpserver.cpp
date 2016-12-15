/***********************************************************************
*    udpserver.cpp:                                                    *
*    UDPServer, for receiving datagrams over a UDP port number         *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a UDPServer class           *
*    It is used to receive datagrams over a specified UDP port number  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "udpserver.h"

UDPServer::UDPServer() :
    UDPServer{UDPServer::s_DEFAULT_PORT_NUMBER}
{

}

UDPServer::UDPServer(uint16_t portNumber) :
    m_portNumber{portNumber},
    m_socketAddress{},
    m_receivingSocketAddress{},
    m_broadcast{UDPServer::s_BROADCAST},
    m_isListening{false},
    m_setSocketResult{0},
    m_timeout{UDPServer::s_DEFAULT_TIMEOUT},
    m_shutEmDown{false}
{
    if (!this->isValidPortNumber(this->m_portNumber)) {
        this->m_portNumber = UDPServer::s_DEFAULT_PORT_NUMBER;
        throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<int16_t>::max()) 
                                 + "("
                                 + std::to_string(this->m_portNumber) 
                                 + ")");
    }
    this->initialize();
}

bool constexpr UDPServer::isValidPortNumber(int portNumber)
{
    return ((portNumber > 0) && (portNumber < std::numeric_limits<int16_t>::max()));
}

void UDPServer::setPortNumber(uint16_t portNumber)
{
    if (!this->isValidPortNumber(this->m_portNumber)) {
        throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<int16_t>::max()) 
                                 + "("
                                 + std::to_string(this->m_portNumber) 
                                 + ")");
    }
    this->initialize();
    std::cout << "Initialized" << std::endl;
}

void UDPServer::setTimeout(unsigned int timeout)
{
    this->m_timeout = timeout;
}

unsigned int UDPServer::timeout() const
{
    return this->m_timeout;
}

uint16_t UDPServer::portNumber() const
{
    return this->m_portNumber;
}


void UDPServer::flush()
{
    std::unique_lock<std::mutex> ioLock{this->m_ioMutex};
    this->m_messageQueue.clear();
}

void UDPServer::initialize()
{


    this->m_setSocketResult = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->m_setSocketResult == -1) {
        throw std::runtime_error("ERROR: An error condition occurred while setting UDP socket");
    }
    
    setsockopt(this->m_setSocketResult, SOL_SOCKET, SO_BROADCAST, &this->m_broadcast, sizeof(this->m_broadcast));
    memset(&this->m_socketAddress, 0, sizeof(this->m_socketAddress));
    this->m_socketAddress.sin_family = AF_INET;
    this->m_socketAddress.sin_port = htons(this->m_portNumber);
    this->m_socketAddress.sin_addr.s_addr = INADDR_ANY;

    if (::bind(this->m_setSocketResult, (sockaddr *)&this->m_socketAddress, sizeof(sockaddr)) == -1) {
        throw std::runtime_error("ERROR: An error condition occurred while binding UDP socket");
    }
}

void UDPServer::startListening()
{
    if (!this->m_isListening) {
        this->m_isListening = true;
        this->m_asyncFuture = std::async(std::launch::async, 
                                         &UDPServer::staticAsyncUdpServer, 
                                         this);
    }
}

void UDPServer::stopListening()
{
    this->m_shutEmDown = true;
    this->m_isListening = false;
}

bool UDPServer::isListening() const 
{
    return this->m_isListening;
}

unsigned int UDPServer::available() const
{
    return this->m_messageQueue.size();
}

void UDPServer::staticAsyncUdpServer()
{
    do {
        std::string receivedString{""};
        char lowLevelReceiveBuffer[UDPServer::s_RECEIVED_BUFFER_MAX];
        unsigned int socketSize{sizeof(sockaddr)};
        recvfrom(this->m_setSocketResult, 
                 lowLevelReceiveBuffer, 
                 sizeof(lowLevelReceiveBuffer)-1, 
                 0, 
                 (sockaddr *)&this->m_receivingSocketAddress, 
                 &socketSize);
        receivedString = std::string{lowLevelReceiveBuffer};
        if (receivedString.length() > 0) {
            std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex};
            for (auto it : receivedString) {
                if (this->m_messageQueue.size() >= UDPServer::s_MAXIMUM_BUFFER_SIZE) {
                    this->m_messageQueue.pop_front();
                }
                this->m_messageQueue.push_back(it);
            }
            ioMutexLock.unlock();
        }        
    } while (!this->m_shutEmDown);
}

char UDPServer::readByte()
{
    std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_messageQueue.size() == 0) {
        return 0;
    } else {
        char returnChar{this->m_messageQueue.front()};
        this->m_messageQueue.pop_front();
        return returnChar;
    }
}

std::string UDPServer::readString()
{
    std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_messageQueue.size() == 0) {
        return "";
    }
    std::string stringToReturn{""};
    for (auto it : this->m_messageQueue) {
        stringToReturn += it;
    }
    this->m_messageQueue.clear();
    return stringToReturn;
}

void UDPServer::openPort()
{
    this->startListening();
}

void UDPServer::closePort()
{
    this->stopListening();
}

bool UDPServer::isOpen() const
{
    return this->m_isListening;
}

std::string UDPServer::readStringUntil(char until) 
{ 
    return this->readStringUntil(std::string{1, until});
}

std::string UDPServer::readStringUntil(const char *until) 
{ 
    return this->readStringUntil(static_cast<std::string>(until)); 
}

std::string UDPServer::readStringUntil(const std::string &until)
{
    using namespace GeneralUtilities;

    std::string returnString{""};
    EventTimer eventTimer;
    eventTimer.start();
    do {
        char byteRead{0};
        byteRead = this->readByte();
        if (byteRead != 0) {
            returnString += byteRead;
        }
        eventTimer.update();
    } while (!endsWith(returnString, until) && (eventTimer.totalTime() < this->m_timeout));
    return returnString;
}