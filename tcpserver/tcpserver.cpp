/***********************************************************************
*    tcpserver.cpp:                                                    *
*    TCPServer, for receiving datagrams over a TCP port number         *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                          *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a TCPServer class           *
*    It is used to receive data over a specified TCP port number       *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "tcpserver.h"

const uint16_t TCPServer::BROADCAST{1};

TCPServer::TCPServer() :
    TCPServer{TCPServer::DEFAULT_PORT_NUMBER}
{
    
}

TCPServer::TCPServer(uint16_t portNumber) :
    m_socketAddress{},
    m_isListening{false},
    m_setSocketResult{0},
    m_timeout{TCPServer::DEFAULT_TIMEOUT},
    m_datagramQueue{},
    m_shutEmDown{false}
{
    this->initialize(portNumber);
}

bool constexpr TCPServer::isValidPortNumber(int portNumber)
{
    return ((portNumber > 0) && (portNumber < std::numeric_limits<uint16_t>::max()));
}

void TCPServer::setPortNumber(uint16_t portNumber)
{
    if (!this->isValidPortNumber(portNumber)) {
        throw std::runtime_error("ERROR: Invalid port set for TCPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max())
                                 + "("
                                 + std::to_string(portNumber)
                                 + ")");
    } else {
        this->m_socketAddress.sin_port = htons(portNumber);
    }
}

void TCPServer::setTimeout(long timeout)
{
    long tempTimeout{this->m_timeout};
    if (timeout < 0) {
        this->m_timeout = 0;
    } else {
        this->m_timeout = timeout;
    }
    struct timeval tv{};
    tv.tv_sec = 0;
    tv.tv_usec = this->m_timeout * 3;
    if (setsockopt(this->m_setSocketResult, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        this->m_timeout = tempTimeout;
        throw std::runtime_error("In TCPServer::setTimeout(long): An error occurred while attempting to set the socket timeout");
    }
}


void TCPServer::setTimeout(int socketIndex, long timeout)
{
    long tempTimeout{this->m_timeout};
    if (timeout < 0) {
        this->m_timeout = 0;
    } else {
        this->m_timeout = timeout;
    }
    struct timeval tv{};
    tv.tv_sec = 0;
    tv.tv_usec = this->m_timeout * 3;
    if (setsockopt(socketIndex, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        this->m_timeout = tempTimeout;
        throw std::runtime_error("In TCPServer::setTimeout(int, long): An error occurred while attempting to set the socket timeout");
    }
}

long TCPServer::timeout() const
{
    return this->m_timeout;
}

void TCPServer::flushRXTX()
{
    std::lock_guard<std::mutex> ioLock{this->m_ioMutex};
    this->m_datagramQueue.clear();
}

uint16_t TCPServer::portNumber() const 
{ 
    return ntohs(this->m_socketAddress.sin_port); 
}

void TCPServer::flushRX()
{
    return this->flushRXTX();
}

void TCPServer::flushTX()
{
    return this->flushRXTX();
}

void TCPServer::initialize(uint16_t portNumber)
{
    using namespace GeneralUtilities;
    if (!this->isValidPortNumber(portNumber)) {
        portNumber = TCPServer::DEFAULT_PORT_NUMBER;
        throw std::runtime_error("ERROR: Invalid port set for TCPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max())
                                 + "("
                                 + std::to_string(portNumber)
                                 + ")");
    }


    this->m_setSocketResult = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->m_setSocketResult == -1) {
       throw std::runtime_error("ERROR: TCPClient could not set socket " + tQuoted(this->m_setSocketResult) + " (is something else using it?)");
    }

    setsockopt(this->m_setSocketResult, SOL_SOCKET, SO_SNDBUF, &TCPServer::BROADCAST, sizeof(TCPServer::BROADCAST));
    memset(&this->m_socketAddress, 0, sizeof(this->m_socketAddress));
    this->m_socketAddress.sin_family = AF_INET;
    this->m_socketAddress.sin_addr.s_addr = INADDR_ANY;
    this->m_socketAddress.sin_port = htons(portNumber);

    if (bind(this->m_setSocketResult, reinterpret_cast<sockaddr *>(&this->m_socketAddress), sizeof(sockaddr)) == -1) {
       throw std::runtime_error("ERROR: TCPClient could not bind socket " + tQuoted(this->m_setSocketResult) + " (is something else using it?)");
    }
}

void TCPServer::startListening(int socketNumber)
{
    if (!this->m_isListening) {
        this->m_isListening = true;
#if defined(__ANDROID__)
        if (this->m_asyncFuture) {
            this->m_asyncFuture.join();
            delete this->m_asyncFuture;
        }
        this->m_asyncFuture = new std::thread{&static_cast<void (TCPServer::*)(int)>(&TCPServer::asyncDatagramListener),
                                              this,
                                              socketNumber};
#else
    try {
        do {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } while (this->m_asyncFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready);
        //this->m_asyncFuture.reset();
    } catch (std::exception &e) {
        
    }
    this->m_asyncFuture = std::async(std::launch::async,
                                    static_cast<void (TCPServer::*)(int)>(&TCPServer::asyncDatagramListener),
                                    this,
                                    socketNumber);
#endif
    }
}

void TCPServer::startListening()
{
    if (!this->m_isListening) {
        this->m_isListening = true;
#if defined(__ANDROID__)
        if (this->m_asyncFuture) {
            this->m_asyncFuture.join();
            delete this->m_asyncFuture;
        }
        this->m_asyncFuture = new std::thread{&static_cast<void (TCPServer::*)()>(&TCPServer::asyncDatagramListener),
                                              this};
#else
    try {
        do {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } while (this->m_asyncFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready);
        //this->m_asyncFuture.reset();
    } catch (std::exception &e) {
        
    }
    this->m_asyncFuture = std::async(std::launch::async,
                                    static_cast<void (TCPServer::*)()>(&TCPServer::asyncDatagramListener),
                                    this);
#endif
    }
}

void TCPServer::stopListening()
{
    this->m_shutEmDown = true;
    this->m_isListening = false;
#if defined(__ANDROID__)
    if (this->m_asyncFuture) {
        this->m_asyncFuture->join();
        delete this->m_asyncFuture;
    }
#else
    try {
        do {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } while (this->m_asyncFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready);
        //this->m_asyncFuture.reset();
    } catch (std::exception &e) {

    }

#endif
}

bool TCPServer::isListening() const
{
    return this->m_isListening;
}

void TCPServer::asyncDatagramListener()
{
    std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex, std::defer_lock};
    do {
        char lowLevelReceiveBuffer[TCPServer::RECEIVED_BUFFER_MAX];
        memset(lowLevelReceiveBuffer, 0, TCPServer::RECEIVED_BUFFER_MAX);
        std::string receivedString{""};
        sockaddr_in receivedAddress{};
        platform_socklen_t socketSize{sizeof(sockaddr)};
        ssize_t returnValue {recvfrom(this->m_setSocketResult,
                            lowLevelReceiveBuffer,
                            sizeof(lowLevelReceiveBuffer)-1,
                            0,
                            reinterpret_cast<sockaddr *>(&receivedAddress),
                            &socketSize)};
        if ((returnValue == EAGAIN) || (returnValue == EWOULDBLOCK) || (returnValue == -1)) {
            //No data;
        } else {
            receivedString = std::string{lowLevelReceiveBuffer};
            if (receivedString.length() > 0) {
                ioMutexLock.lock();
                this->m_datagramQueue.emplace_back(receivedAddress, receivedString);
                ioMutexLock.unlock();

            }
        }
    } while (!this->m_shutEmDown);
}


void TCPServer::asyncDatagramListener(int socketNumber)
{
    std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex, std::defer_lock};
    do {
        char lowLevelReceiveBuffer[TCPServer::RECEIVED_BUFFER_MAX];
        memset(lowLevelReceiveBuffer, 0, TCPServer::RECEIVED_BUFFER_MAX);
        std::string receivedString{""};
        sockaddr_in receivedAddress{};
        platform_socklen_t socketSize{sizeof(sockaddr)};
        ssize_t returnValue {recvfrom(socketNumber,
                            lowLevelReceiveBuffer,
                            sizeof(lowLevelReceiveBuffer)-1,
                            0,
                            reinterpret_cast<sockaddr *>(&receivedAddress),
                            &socketSize)};
        if ((returnValue == EAGAIN) || (returnValue == EWOULDBLOCK) || (returnValue == -1)) {
            //No data;
        } else {
            receivedString = std::string{lowLevelReceiveBuffer};
            if (receivedString.length() > 0) {
                ioMutexLock.lock();
                this->m_datagramQueue.emplace_back(receivedAddress, receivedString);
                ioMutexLock.unlock();

            }
        }
    } while (!this->m_shutEmDown);
}

void TCPServer::syncDatagramListener(int socketNumber)
{
    std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex, std::defer_lock};
    char lowLevelReceiveBuffer[TCPServer::RECEIVED_BUFFER_MAX];
    memset(lowLevelReceiveBuffer, 0, TCPServer::RECEIVED_BUFFER_MAX);
    std::string receivedString{""};
    sockaddr_in receivedAddress{};
    platform_socklen_t socketSize{sizeof(sockaddr)};
    ssize_t returnValue{recvfrom(socketNumber,
                        lowLevelReceiveBuffer,
                        sizeof(lowLevelReceiveBuffer)-1,
                        0,
                        reinterpret_cast<sockaddr *>(&receivedAddress),
                        &socketSize)};
    if ((returnValue == EAGAIN) || (returnValue == EWOULDBLOCK) || (returnValue == -1)) {
        return;
    }
    receivedString = std::string{lowLevelReceiveBuffer};
    if (receivedString.length() > 0) {
        ioMutexLock.lock();
        this->m_datagramQueue.emplace_back(receivedAddress, receivedString);
        ioMutexLock.unlock();
    }
}

void TCPServer::syncDatagramListener()
{
    std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex, std::defer_lock};
    char lowLevelReceiveBuffer[TCPServer::RECEIVED_BUFFER_MAX];
    memset(lowLevelReceiveBuffer, 0, TCPServer::RECEIVED_BUFFER_MAX);
    std::string receivedString{""};
    sockaddr_in receivedAddress{};
    platform_socklen_t socketSize{sizeof(sockaddr)};
    ssize_t returnValue{recvfrom(this->m_setSocketResult,
                        lowLevelReceiveBuffer,
                        sizeof(lowLevelReceiveBuffer)-1,
                        0,
                        reinterpret_cast<sockaddr *>(&receivedAddress),
                        &socketSize)};
    if ((returnValue == EAGAIN) || (returnValue == EWOULDBLOCK) || (returnValue == -1)) {
        return;
    }
    receivedString = std::string{lowLevelReceiveBuffer};
    if (receivedString.length() > 0) {
        ioMutexLock.lock();
        this->m_datagramQueue.emplace_back(receivedAddress, receivedString);
        ioMutexLock.unlock();
    }
}

void TCPServer::setLineEnding(const std::string &lineEnding)
{
    this->m_lineEnding = lineEnding;
}

std::string TCPServer::lineEnding() const
{
    return this->m_lineEnding;
}

std::string TCPServer::peek()
{
    this->syncDatagramListener();
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return "";
    } else {
        return this->m_datagramQueue.front().message();
    }
}


TCPDatagram TCPServer::peekDatagram()
{
    this->syncDatagramListener();
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return TCPDatagram{};
    } else {
        return this->m_datagramQueue.front();
    }
}

char TCPServer::peekByte()
{
    this->syncDatagramListener();
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

char TCPServer::readByte()
{
    this->syncDatagramListener();
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    std::string str{this->m_datagramQueue.front().message()};
    if (str.length() == 0) {
        return 0;
    }
    char charToReturn{str.at(0)};
    std::string newDatagramMessage{str.substr(1)};
    struct sockaddr_in newDatagramAddress;
    newDatagramAddress.sin_family = this->m_datagramQueue.front().socketAddress().sin_family;
    newDatagramAddress.sin_port = this->m_datagramQueue.front().socketAddress().sin_port;
    newDatagramAddress.sin_addr.s_addr = this->m_datagramQueue.front().socketAddress().sin_addr.s_addr;
    this->m_datagramQueue.pop_front();
    this->m_datagramQueue.emplace_front(newDatagramAddress, newDatagramMessage);
    return charToReturn;

}

TCPDatagram TCPServer::readDatagram()
{
    this->syncDatagramListener();
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return TCPDatagram{};
    } else {
        TCPDatagram returnDatagram{this->m_datagramQueue.front()};
        this->m_datagramQueue.pop_front();
        return returnDatagram;
    }
}


std::string TCPServer::readLine()
{
    this->syncDatagramListener();
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return "";
    }
    std::string stringToReturn{this->m_datagramQueue.front().message()};
    this->m_datagramQueue.pop_front();
    return stringToReturn;
}

void TCPServer::openPort()
{
    //this->startListening();
}

void TCPServer::closePort()
{
    //this->stopListening();
}

bool TCPServer::isOpen() const
{
    return this->m_isListening;
}

std::string TCPServer::readUntil(char until)
{
    return this->readUntil(std::string{1, until});
}

std::string TCPServer::readUntil(const char *until)
{
    return this->readUntil(static_cast<std::string>(until));
}

std::string TCPServer::readUntil(const std::string &until)
{
    std::string tempLineEnding{this->m_lineEnding};
    this->m_lineEnding = until;
    std::string returnString{this->readLine()};
    this->m_lineEnding = tempLineEnding;
    return returnString;
}

void TCPServer::putBack(const TCPDatagram &datagram)
{
    std::lock_guard<std::mutex> ioMutex{this->m_ioMutex};
    this->m_datagramQueue.push_front(datagram);
}

void TCPServer::putBack(char back)
{
    return TCPServer::putBack(std::string{1, back});
}

void TCPServer::putBack(const char *str)
{
    return TCPServer::putBack(static_cast<std::string>(str));
}

void TCPServer::putBack(const std::string &str)
{
    if (str.length() == 0) {
        return;
    }
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    std::string newDatagramMessage{str + this->m_datagramQueue.front().message()};
    struct sockaddr_in newDatagramAddress;
    newDatagramAddress.sin_family = this->m_datagramQueue.front().socketAddress().sin_family;
    newDatagramAddress.sin_port = this->m_datagramQueue.front().socketAddress().sin_port;
    newDatagramAddress.sin_addr.s_addr = this->m_datagramQueue.front().socketAddress().sin_addr.s_addr;
    this->m_datagramQueue.pop_front();
    this->m_datagramQueue.emplace_front(newDatagramAddress, newDatagramMessage);
}

uint16_t TCPServer::doUserSelectPortNumber()
{
    return GeneralUtilities::doUserEnterNumericParameter("Server Port Number",
                                                         static_cast<std::function<bool(uint16_t)>>(TCPServer::isValidPortNumber),
                                                         std::numeric_limits<uint16_t>::min()+1,
                                                         std::numeric_limits<uint16_t>::max());
}

std::shared_ptr<TCPServer> TCPServer::doUserSelectTCPServer()
{
    uint16_t portNumber{TCPServer::doUserSelectPortNumber()};
    std::shared_ptr<TCPServer> tcpServer{std::make_shared<TCPServer>(portNumber)};
    return tcpServer;
}

std::string TCPServer::readUntil(int socketNumber, char until)
{
    return this->readUntil(socketNumber, std::string{1, until});
}

std::string TCPServer::readUntil(int socketNumber, const char *until)
{
    return this->readUntil(socketNumber, static_cast<std::string>(until));
}

std::string TCPServer::readUntil(int socketNumber, const std::string &until)
{
    std::string tempLineEnding{this->m_lineEnding};
    this->m_lineEnding = until;
    std::string returnString{this->readLine(socketNumber)};
    this->m_lineEnding = tempLineEnding;
    return returnString;
}

std::string TCPServer::peek(int socketNumber)
{
    this->syncDatagramListener(socketNumber);
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return "";
    } else {
        return this->m_datagramQueue.front().message();
    }
}

TCPDatagram TCPServer::peekDatagram(int socketNumber)
{
    this->syncDatagramListener(socketNumber);
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return TCPDatagram{};
    } else {
        return this->m_datagramQueue.front();
    }
}

char TCPServer::peekByte(int socketNumber)
{
    this->syncDatagramListener(socketNumber);
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

ssize_t TCPServer::available() 
{
    this->syncDatagramListener();
    return this->m_datagramQueue.size();
}

ssize_t TCPServer::available(int socketNumber) 
{
    this->syncDatagramListener(socketNumber);
    return this->m_datagramQueue.size();
}

char TCPServer::readByte(int socketNumber)
{
    this->syncDatagramListener(socketNumber);
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    std::string str{this->m_datagramQueue.front().message()};
    if (str.length() == 0) {
        return 0;
    }
    char charToReturn{str.at(0)};
    std::string newDatagramMessage{str.substr(1)};
    struct sockaddr_in newDatagramAddress;
    newDatagramAddress.sin_family = this->m_datagramQueue.front().socketAddress().sin_family;
    newDatagramAddress.sin_port = this->m_datagramQueue.front().socketAddress().sin_port;
    newDatagramAddress.sin_addr.s_addr = this->m_datagramQueue.front().socketAddress().sin_addr.s_addr;
    this->m_datagramQueue.pop_front();
    this->m_datagramQueue.emplace_front(newDatagramAddress, newDatagramMessage);
    return charToReturn;

}

TCPDatagram TCPServer::readDatagram(int socketNumber)
{
    this->syncDatagramListener(socketNumber);
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return TCPDatagram{};
    } else {
        TCPDatagram returnDatagram{this->m_datagramQueue.front()};
        this->m_datagramQueue.pop_front();
        return returnDatagram;
    }
}


std::string TCPServer::readLine(int socketNumber)
{
    this->syncDatagramListener(socketNumber);
    std::lock_guard<std::mutex> ioMutexLock{this->m_ioMutex};
    if (this->m_datagramQueue.size() == 0) {
        return "";
    }
    std::string stringToReturn{this->m_datagramQueue.front().message()};
    this->m_datagramQueue.pop_front();
    return stringToReturn;
}

TCPServer::~TCPServer()
{
    this->stopListening();
    shutdown(this->m_setSocketResult, SHUT_RDWR);
}