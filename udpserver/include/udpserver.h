/***********************************************************************
*    udpserver.h:                                                      *
*    UDPServer, for receiving datagrams over a UDP port number         *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    this file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    this file holds the declarations of a UDPServer class             *
*    It is used to receive datagrams over a specified UDP port number  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_UDPSERVER_H
#define TJLUTILS_UDPSERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <mutex>
#include <future>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <generalutilities.h>
#include <eventtimer.h>
#include <tstream.h>

#if defined(__ANDROID__)
    using platform_socklen_t = socklen_t;
#elif defined(_WIN32)
     using platform_socklen_t = int;
#else
    using platform_socklen_t = socklen_t;
#endif

class UDPDatagram
{
public:
    UDPDatagram(struct sockaddr_in socketAddress, const std::string &message) :
        m_message{message}
    { 
       this->m_socketAddress.sin_family = socketAddress.sin_family;
       this->m_socketAddress.sin_port = socketAddress.sin_port;
       this->m_socketAddress.sin_addr.s_addr = socketAddress.sin_addr.s_addr;
    }

    UDPDatagram() :
        m_message{""} 
    { 
       this->m_socketAddress.sin_family = 0;
       this->m_socketAddress.sin_port = 0;
       this->m_socketAddress.sin_addr.s_addr = 0;
    }

    struct sockaddr_in socketAddress () const 
    {
        sockaddr_in returnSocket;
        returnSocket.sin_family = this->m_socketAddress.sin_family;
        returnSocket.sin_port = this->m_socketAddress.sin_port;
        returnSocket.sin_addr.s_addr = this->m_socketAddress.sin_addr.s_addr; 
        return returnSocket;
    }

    uint16_t portNumber() const { return ntohs(this->m_socketAddress.sin_port); }
    std::string message() const { return this->m_message; }
    std::string hostName() const 
    { 
        char lowLevelTempBuffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(this->m_socketAddress.sin_addr), lowLevelTempBuffer, INET_ADDRSTRLEN);
        return std::string{lowLevelTempBuffer};
    }

private:
    struct sockaddr_in m_socketAddress;
    std::string m_message;
};

class UDPServer
{
public:
    UDPServer();
    UDPServer(uint16_t port);

    char readByte();
    UDPDatagram readDatagram();
    std::string readLine();
    std::string readUntil(const std::string &until);
    std::string readUntil(const char *until);
    std::string readUntil(char until);
    ssize_t available();
    void startListening();
    void stopListening();
    bool isListening() const;
    std::string lineEnding() const;

    unsigned long timeout() const;
    void setPortNumber(uint16_t portNumber);
    uint16_t portNumber() const;
    void setTimeout(unsigned long timeout);
    void flushRXTX();
    void flushRX();
    void flushTX();

    void openPort();
    void closePort();
    bool isOpen() const;

    void putBack(const std::string &str);
    void putBack(const char *str);
    void putBack(char back);
    void putBack(const UDPDatagram &datagram);


    void setLineEnding(const std::string &str);
    std::string peek();
    char peekByte();
    UDPDatagram peekDatagram();

    static uint16_t doUserSelectPortNumber();
    static std::shared_ptr<UDPServer> doUserSelectUDPServer();

    static const constexpr uint16_t DEFAULT_PORT_NUMBER{8888};
    static const constexpr unsigned int DEFAULT_TIMEOUT{100};

private:
    struct sockaddr_in m_socketAddress;
    int m_setSocketResult;
    bool m_isListening;
    unsigned long m_timeout;
    std::deque<UDPDatagram> m_datagramQueue;
    std::mutex m_ioMutex;
    bool m_shutEmDown;
    std::string m_lineEnding;

    void initialize(uint16_t portNumber);
#if defined(__ANDROID__)
    std::thread *m_asyncFuture;
#else
    std::future<void> m_asyncFuture;
#endif

    void asyncDatagramListener();
    void syncDatagramListener();

    static const uint16_t BROADCAST;
    static const constexpr size_t RECEIVED_BUFFER_MAX{65535};
    static const constexpr size_t MAXIMUM_BUFFER_SIZE{65535};

    static constexpr bool isValidPortNumber(int portNumber);

};

#endif //TJLUTILS_UDPSERVER_H
