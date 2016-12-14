/***********************************************************************
*    udpduplex.h:                                                      *
*    UDPDuplex for sending/receiving datagrams over a UDP port number  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a UDPDuplex class             *
*    It is used to send and receive datagrams over a specified UDP     *
*    port number, to a specified IP address, with DNS lookup supported *
*    This class also implements the TStream interface                  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_UDPDUPLEX_H
#define TJLUTILS_UDPDUPLEX_H

#include <iostream>
#include <cstdlib>
#include <memory>
#include <limits>
#include <cerrno>
#include <memory.h>

#include <net/if.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include <unistd.h>
#include <signal.h>

#include <udpclient.h>
#include <udpserver.h>
#include <tstream.h>

class UDPDuplex : public TStream
{
public:
    UDPDuplex();
    UDPDuplex(uint16_t clientPortNumber);
    UDPDuplex(const std::string &clientHostName);
    UDPDuplex(const std::string &clientHostName, uint16_t clientPortNumber);
    UDPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber);

    /*Client/Sender*/
    ssize_t writeByte(char toSend);
    ssize_t writeString(const std::string &str);
    ssize_t writeString(const char *str);

    void setClientHostName(const std::string &hostName);
    void setClientTimeout(unsigned int timeout);
    void setClientPortNumber(uint16_t portNumber);

    std::string clientHostName() const;
    unsigned int clientTimeout() const;
    uint16_t clientPortNumber() const;

    /*Host/Server*/
    char readByte();
    std::string readString();
    std::string readStringUntil(const std::string &until);
    std::string readStringUntil(const char *until);
    std::string readStringUntil(char until);
    unsigned int available() const;
    void startListening();
    void stopListening();
    bool isListening() const;
    
    unsigned int serverTimeout() const;
    void setServerPortNumber(uint16_t portNumber);
    uint16_t serverPortNumber() const;
    void setServerTimeout(unsigned int timeout);
    void flush();

    /*Both - TStream interface compliance*/
    void openPort();
    void closePort();
    bool isOpen() const;
    unsigned int timeout() const;
    std::string portName() const;
    void setTimeout(unsigned int timeout);


private:
    std::unique_ptr<UDPServer> m_udpServer;
    std::unique_ptr<UDPClient> m_udpClient;
    
    int resolveAddressHelper(const std::string &hostName, int family, const std::string &service, sockaddr_storage* addressPtr);

    void initialize();
    
    static const char *s_DEFAULT_CLIENT_HOST_NAME;

    static const constexpr uint16_t s_DEFAULT_CLIENT_PORT_NUMBER{8887};
    static const constexpr unsigned int s_DEFAULT_CLIENT_TIMEOUT{100};

    static const constexpr uint16_t s_DEFAULT_SERVER_PORT_NUMBER{8888};
    static const constexpr unsigned int s_DEFAULT_SERVER_TIMEOUT{100};

    static constexpr bool isValidPortNumber(int portNumber);
};

#endif //TJLUTILS_UDPDUPLEX_H