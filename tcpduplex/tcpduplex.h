/***********************************************************************
*    tcpduplex.h:                                                      *
*    TCPDuplex for sending/receiving data over a TCP port number       *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                          *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a TCPDuplex class             *
*    It is used to send and receive data over a specified TCP          *
*    port number, to a specified IP address, with DNS lookup supported *
*    This class also implements the TStream interface                  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_TCPDUPLEX_H
#define TJLUTILS_TCPDUPLEX_H

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

#include <unistd.h>
#include <signal.h>

#include <tcpclient.h>
#include <tcpserver.h>
#include <tstream.h>

enum class TCPObjectType {
    TCP_DUPLEX,
    TCP_SERVER,
    TCP_CLIENT
};

class TCPDuplex : public TStream
{
public:
    TCPDuplex(TCPObjectType tcpObjectType = TCPObjectType::TCP_DUPLEX);
    TCPDuplex(uint16_t clientPortNumber, TCPObjectType tcpObjectType = TCPObjectType::TCP_DUPLEX);
    TCPDuplex(const std::string &clientHostName, TCPObjectType tcpObjectType = TCPObjectType::TCP_DUPLEX);
    TCPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, TCPObjectType tcpObjectType = TCPObjectType::TCP_DUPLEX);
    TCPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber, TCPObjectType tcpObjectType = TCPObjectType::TCP_DUPLEX);
    TCPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber, uint16_t clientReturnAddressPortNumber, TCPObjectType tcpObjectType = TCPObjectType::TCP_DUPLEX);

    /*Client/Sender*/
    ssize_t writeLine(const std::string &str);
    ssize_t writeLine(const char *str);

    void setClientHostName(const std::string &hostName);
    void setClientTimeout(long timeout);
    void setClientPortNumber(uint16_t portNumber);
    void setClientReturnAddressPortNumber(uint16_t returnAddressPortNumber);

    std::string clientHostName() const;
    long clientTimeout() const;
    uint16_t clientPortNumber() const;
    uint16_t clientReturnAddressPortNumber() const;

    /*Host/Server*/
    char readByte();
    TCPDatagram readDatagram();
    std::string readLine();
    std::string readUntil(const std::string &until);
    std::string readUntil(const char *until);
    std::string readUntil(char until);
    ssize_t available();
    void startListening();
    void stopListening();
    bool isListening() const;
    void putBack(const TCPDatagram &datagram);
    void putBack(const std::string &str);
    void putBack(const char *str);
    void putBack(char back);

    TCPDatagram peekDatagram();
    std::string peek();
    char peekByte();

    void flushRXTX();
    void flushRX();
    void flushTX();
    
    long serverTimeout() const;
    void setServerPortNumber(uint16_t portNumber);
    uint16_t serverPortNumber() const;
    void setServerTimeout(long timeout);
    void flush();

    /*Both - TStream interface compliance*/
    void openPort();
    void closePort();
    bool isOpen() const;
    long timeout() const;
    std::string portName() const;
    void setTimeout(long timeout);
    
    static TCPObjectType parseTCPObjectTypeFromRaw(const std::string &tcpObjectType);
    static std::string tcpObjectTypeToString(TCPObjectType tcpObjectType);
    
    void setLineEnding(const std::string &lineEnding);
    std::string lineEnding() const;

    TCPObjectType tcpObjectType() const;

    static uint16_t doUserSelectClientPortNumber();
    static std::string doUserSelectClientHostName();
    static uint16_t doUserSelectClientReturnAddressPortNumber();
    static uint16_t doUserSelectServerPortNumber();
    static TCPObjectType doUserSelectTCPObjectType();
    static std::shared_ptr<TCPDuplex> doUserSelectTCPDuplex();
    
    static const char *DEFAULT_CLIENT_HOST_NAME;

    static const constexpr uint16_t DEFAULT_CLIENT_PORT_NUMBER{TCPClient::DEFAULT_PORT_NUMBER};
    static const constexpr uint16_t DEFAULT_CLIENT_RETURN_ADDRESS_PORT_NUMBER{TCPClient::DEFAULT_RETURN_ADDRESS_PORT_NUMBER};
    static const constexpr long DEFAULT_CLIENT_TIMEOUT{TCPClient::DEFAULT_TIMEOUT};

    static const constexpr uint16_t DEFAULT_SERVER_PORT_NUMBER{TCPServer::DEFAULT_PORT_NUMBER};
    static const constexpr long DEFAULT_SERVER_TIMEOUT{TCPServer::DEFAULT_TIMEOUT};

private:
    std::unique_ptr<TCPServer> m_tcpServer;
    std::unique_ptr<TCPClient> m_tcpClient;
    TCPObjectType m_tcpObjectType;
    
    int resolveAddressHelper(const std::string &hostName, int family, const std::string &service, sockaddr_storage* addressPtr);

    void initialize();

    static constexpr bool isValidPortNumber(int portNumber);
};

#endif //TJLUTILS_TCPDUPLEX_H