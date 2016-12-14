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

class UDPServer
{
public:
    UDPServer();
    UDPServer(uint16_t port);

    char readByte();
    std::string readString();
    std::string readStringUntil(const std::string &until);
    std::string readStringUntil(const char *until);
    std::string readStringUntil(char until);
    int available() const;
    void startListening();
    void stopListening();
    bool isListening() const;

    void timeout() const;
    void setTimeout(unsigned int timeout);
    void flush();


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
    
    void initialize();
    void staticAsyncUdpServer();

    static const constexpr uint16_t s_DEFAULT_PORT{8888};
    static const constexpr uint16_t s_BROADCAST{1};
    static const constexpr size_t s_RECEIVED_BUFFER_MAX{10000};
    static const constexpr size_t s_MAXIMUM_BUFFER_SIZE{65535};
    static const constexpr unsigned int s_DEFAULT_TIMEOUT{100};

    static constexpr bool isValidPortNumber(int portNumber);
};

#endif //TJLUTILS_UDPSERVER_H