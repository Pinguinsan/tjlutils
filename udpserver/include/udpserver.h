/***********************************************************************
*    udpserver.h:                                                      *
*    UDPServer, for receiving datagrams over a UDP port number         *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a UDPServer class             *
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

#include <generalutilities.h>
#include <eventtimer.h>
#include <tstream.h>

class UDPServer
{
public:
    UDPServer();
    UDPServer(uint16_t port);

    char readByte();
    std::string readString(int maximumReadSize = TStream::NO_MAXIMUM_READ_SIZE);
    std::string readStringUntil(const std::string &until, int maximumReadSize = TStream::NO_MAXIMUM_READ_SIZE);
    std::string readStringUntil(const char *until, int maximumReadSize = TStream::NO_MAXIMUM_READ_SIZE);
    std::string readStringUntil(char until);
    unsigned int available() const;
    void startListening();
    void stopListening();
    bool isListening() const;
    
    unsigned int timeout() const;
    void setPortNumber(uint16_t portNumber);
    uint16_t portNumber() const;
    void setTimeout(unsigned int timeout);
    void flushRXTX();
    void flushRX();
    void flushTX();

    void openPort();
    void closePort();
    bool isOpen() const;

    void putBack(const std::string &str);
    void putBack(const char *str);
    void putBack(char back);

    std::string peek();
    char peekByte();

    static uint16_t doUserSelectPortNumber();
    static std::shared_ptr<UDPServer> doUserSelectUDPServer();

    static const constexpr uint16_t s_DEFAULT_PORT_NUMBER{8888};
    static const constexpr unsigned int s_DEFAULT_TIMEOUT{100};

private:
    sockaddr_in m_socketAddress;
    sockaddr_in m_receivingSocketAddress;
    uint16_t m_portNumber;
    uint16_t m_broadcast;
    int m_setSocketResult;
    bool m_isListening;
    unsigned int m_timeout;
    std::deque<char> m_messageQueue;
    std::mutex m_ioMutex;
    bool m_shutEmDown;
    std::future<void> m_asyncFuture;
    
    void initialize();
    void staticAsyncUdpServer();

    static const constexpr uint16_t s_BROADCAST{1};
    static const constexpr size_t s_RECEIVED_BUFFER_MAX{10000};
    static const constexpr size_t s_MAXIMUM_BUFFER_SIZE{65535};

    static constexpr bool isValidPortNumber(int portNumber);

};

#endif //TJLUTILS_UDPSERVER_H