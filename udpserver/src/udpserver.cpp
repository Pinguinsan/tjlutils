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
    m_broadcast{UDPServer::s_BROADCAST},
    m_isListening{false},
    m_setSocketResult{0},
    m_timeout{UDPServer::s_DEFAULT_TIMEOUT},
    m_datagramQueue{},
    m_shutEmDown{false}
{
    if (!this->isValidPortNumber(this->m_portNumber)) {
        this->m_portNumber = UDPServer::s_DEFAULT_PORT_NUMBER;
        throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max())
                                 + "("
                                 + std::to_string(this->m_portNumber)
                                 + ")");
    }
    this->initialize();
}

bool constexpr UDPServer::isValidPortNumber(int portNumber)
{
    return ((portNumber > 0) && (portNumber < std::numeric_limits<uint16_t>::max()));
}

void UDPServer::setPortNumber(uint16_t portNumber)
{
    if (!this->isValidPortNumber(portNumber)) {
        throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max())
                                 + "("
                                 + std::to_string(portNumber)
                                 + ")");
    }
    this->m_portNumber = portNumber;
    this->initialize();
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


void UDPServer::flushRXTX()
{
    std::lock_guard<std::mutex> ioLock{this->m_ioMutex};
    this->m_datagramQueue.clear();
}

void UDPServer::flushRX()
{
    return this->flushRXTX();
}

void UDPServer::flushTX()
{
    return this->flushRXTX();
}

void UDPServer::initialize()
{
    using namespace GeneralUtilities;
    this->m_setSocketResult = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->m_setSocketResult == -1) {
       throw std::runtime_error("ERROR: UDPClient could not set socket " + tQuoted(this->m_setSocketResult) + " (is something else using it?");
    }

    setsockopt(this->m_setSocketResult, SOL_SOCKET, SO_BROADCAST, &this->m_broadcast, sizeof(this->m_broadcast));
    memset(&this->m_socketAddress, 0, sizeof(this->m_socketAddress));
    this->m_socketAddress.sin_family = AF_INET;
    this->m_socketAddress.sin_port = htons(this->m_portNumber);
    this->m_socketAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(this->m_setSocketResult, reinterpret_cast<sockaddr *>(&this->m_socketAddress), sizeof(sockaddr)) == -1) {
       throw std::runtime_error("ERROR: UDPClient could not bind socket " + tQuoted(this->m_setSocketResult) + " (is something else using it?");
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
    return this->m_datagramQueue.size();
}

void UDPServer::staticAsyncUdpServer()
{
    std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex, std::defer_lock};
    do {
        char lowLevelReceiveBuffer[UDPServer::s_RECEIVED_BUFFER_MAX];
        memset(lowLevelReceiveBuffer, 0, UDPServer::s_RECEIVED_BUFFER_MAX);
        std::string receivedString{""};
        sockaddr_in receivedAddress{};
        unsigned int socketSize{sizeof(sockaddr)};
        ssize_t returnValue = recvfrom(this->m_setSocketResult,
                            lowLevelReceiveBuffer,
                            sizeof(lowLevelReceiveBuffer)-1,
                            0,
                            reinterpret_cast<sockaddr *>(&receivedAddress),
                            &socketSize);
        if ((returnValue == EAGAIN) || (returnValue == EWOULDBLOCK) || (returnValue == -1)) { 
            continue;
        }
        receivedString = std::string{lowLevelReceiveBuffer};
        if (receivedString.length() > 0) {
            ioMutexLock.lock();
            this->m_datagramQueue.emplace_back(receivedAddress, receivedString);
            ioMutexLock.unlock();

        }
    } while (!this->m_shutEmDown);
}

std::string UDPServer::peek()
{
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return "";
    } else {
        return this->m_datagramQueue.front().message();
    }
}


UDPDatagram UDPServer::peekDatagram()
{
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return UDPDatagram{};
    } else {
        return this->m_datagramQueue.front();
    }
}

char UDPServer::peekByte()
{
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return 0;
    } else {
        if (this->m_datagramQueue.front().message().size() == 0) {
            return 0;
        } else {
            return this->m_datagramQueue.front().message().at(0);
        }
    }
}

char UDPServer::readByte()
{
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    std::string str{this->m_datagramQueue.front().message()};
    if (str.length() == 0) {
        return 0;
    } 
    char charToReturn{str.at(0)};
    std::string newDatagramMessage{str.substr(1)};
    sockaddr_in newDatagramAddress{this->m_datagramQueue.front().socketAddress()};
    this->m_datagramQueue.pop_front();
    this->m_datagramQueue.emplace_front(newDatagramAddress, newDatagramMessage);
    return charToReturn;

}

UDPDatagram UDPServer::readDatagram()
{
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return UDPDatagram{};
    } else {
        UDPDatagram returnDatagram{this->m_datagramQueue.front()};
        this->m_datagramQueue.pop_front();
        return returnDatagram;
    }
}

std::string UDPServer::readString(int maximumReadSize)
{
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return "";
    }
    std::string stringToReturn{this->m_datagramQueue.front().message()};
    this->m_datagramQueue.pop_front();
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

std::string UDPServer::readStringUntil(const char *until, int maximumReadSize)
{
    return this->readStringUntil(static_cast<std::string>(until), maximumReadSize);
}

std::string UDPServer::readStringUntil(const std::string &until, int maximumReadSize)
{

    using namespace GeneralUtilities;
    std::string returnString{""};
    EventTimer eventTimer;
    eventTimer.start();
    int readCount{0};
    do {
        std::string tempString{""};
        tempString = this->readString();
        if ((returnString.size() + tempString.size()) > maximumReadSize) {
            int amountToAdd = (returnString.size() + tempString.size()) - maximumReadSize;
            this->putBack(tempString.substr(amountToAdd));
            returnString += tempString.substr(0, amountToAdd);
            break;
        } else {
            readCount += tempString.size();
            returnString += tempString;
        }
        eventTimer.update();
    } while (!endsWith(returnString, until) && (eventTimer.totalTime() < this->m_timeout));
    return returnString;
}

void UDPServer::putBack(const UDPDatagram &datagram)
{
    std::lock_guard<std::mutex> ioMutex{this->m_ioMutex};
    this->m_datagramQueue.push_front(datagram);
}

void UDPServer::putBack(char back)
{
    return UDPServer::putBack(std::string{1, back});
}

void UDPServer::putBack(const char *str)
{
    return UDPServer::putBack(static_cast<std::string>(str));
}

void UDPServer::putBack(const std::string &str)
{   
    if (str.length() == 0) {
        return;
    } 
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    std::string newDatagramMessage{str + this->m_datagramQueue.front().message()};
    sockaddr_in newDatagramAddress{this->m_datagramQueue.front().socketAddress()};
    this->m_datagramQueue.pop_front();
    this->m_datagramQueue.emplace_front(newDatagramAddress, newDatagramMessage);
}

uint16_t UDPServer::doUserSelectPortNumber()
{
    return GeneralUtilities::doUserEnterNumericParameter("Server Port Number",
                                                         static_cast<std::function<bool(uint16_t)>>(UDPServer::isValidPortNumber),
                                                         std::numeric_limits<uint16_t>::min()+1,
                                                         std::numeric_limits<uint16_t>::max());
}

std::shared_ptr<UDPServer> UDPServer::doUserSelectUDPServer()
{
    uint16_t portNumber{UDPServer::doUserSelectPortNumber()};
    std::shared_ptr<UDPServer> udpServer{std::make_shared<UDPServer>(portNumber)};
    return udpServer;
}