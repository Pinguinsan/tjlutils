/***********************************************************************
*    tcpclient.h:                                                      *
*    TCPClient, for sending data over a TCP port number                *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                          *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a TCPClient class             *
*    It is used to send datagrams over a specified TCP port number,    *
*    to a specified IP address, with DNS lookup supported              *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_TCPCLIENT_H
#define TJLUTILS_TCPCLIENT_H

#include <iostream>
#include <cstdlib>
#include <memory>
#include <limits>
#include <cerrno>
#include <vector>
#include <memory.h>

#include <net/if.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <signal.h>

#include <generalutilities.h>
#include <tstream.h>

class TCPDuplex;

class TCPClient
{
    friend class TCPDuplex;
public:
    TCPClient();
    TCPClient(uint16_t portNumber);
    TCPClient(const std::string &hostName);
    TCPClient(const std::string &hostName, uint16_t portNumber);
    TCPClient(const std::string &hostName, uint16_t portNumber, uint16_t clientReturnAddressPortNumber);
    ~TCPClient();

    ssize_t writeLine(const char *str);
    ssize_t writeLine(const std::string &str);
    uint16_t portNumber() const;
    std::string hostName() const;
    uint16_t returnAddressPortNumber() const;
    unsigned long int timeout() const;
    void setPortNumber(uint16_t portNumber);
    void setHostName(const std::string &hostName);
    void setReturnAddressPortNumber(uint16_t returnAddressPortNumber);
    void setTimeout(unsigned long int timeout);
    std::string lineEnding() const;
    void setLineEnding(const std::string &lineEnding);

    void openPort();
    void closePort();
    bool isOpen() const;

    static const char *DEFAULT_HOST_NAME;
    static const std::string DEFAULT_LINE_ENDING;
    static const constexpr uint16_t DEFAULT_PORT_NUMBER{8888};
    static const constexpr uint16_t DEFAULT_RETURN_ADDRESS_PORT_NUMBER{1234};
    static const constexpr unsigned int DEFAULT_TIMEOUT{100};
    static const constexpr unsigned int SEND_RETRY_COUNT{3};

    static uint16_t doUserSelectPortNumber();
    static std::string doUserSelectHostName();
    static uint16_t doUserSelectReturnAddressPortNumber();
    static std::shared_ptr<TCPClient> doUserSelectTCPClient();
private:
    sockaddr_in m_destinationAddress;
    sockaddr_in m_returnAddress;

    unsigned int m_timeout;
    int m_tcpSocketIndex;
    std::string m_lineEnding;
    
    ssize_t writeByte(char toSend);
    int resolveAddressHelper(const std::string &hostName, int family, const std::string &service, sockaddr_storage* addressPtr);
    void initialize(const std::string &hostName, uint16_t portNumber, uint16_t returnAddressPortNumber);

    
    static constexpr bool isValidPortNumber(int portNumber);


};

#endif //TJLUTILS_TCPCLIENT_H