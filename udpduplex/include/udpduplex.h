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

#include <unistd.h>
#include <signal.h>

#include <udpclient.h>
#include <udpserver.h>
#include <tstream.h>

enum class UDPObjectType {
    UDP_DUPLEX,
    UDP_SERVER,
    UDP_CLIENT
};

class UDPDuplex : public TStream
{
public:
    UDPDuplex(UDPObjectType udpObjectType = UDPObjectType::UDP_DUPLEX);
    UDPDuplex(uint16_t clientPortNumber, UDPObjectType udpObjectType = UDPObjectType::UDP_DUPLEX);
    UDPDuplex(const std::string &clientHostName, UDPObjectType udpObjectType = UDPObjectType::UDP_DUPLEX);
    UDPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, UDPObjectType udpObjectType = UDPObjectType::UDP_DUPLEX);
    UDPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber, UDPObjectType udpObjectType = UDPObjectType::UDP_DUPLEX);

    /*Client/Sender*/
    ssize_t writeByte(char toSend);
    ssize_t writeString(const std::string &str);
    ssize_t writeString(const char *str);

    void setClientHostName(const std::string &hostName);
    void setClientTimeout(unsigned long timeout);
    void setClientPortNumber(uint16_t portNumber);

    std::string clientHostName() const;
    unsigned long clientTimeout() const;
    uint16_t clientPortNumber() const;

    /*Host/Server*/
    char readByte();
    UDPDatagram readDatagram();
    std::string readString(unsigned long maximumReadSize = TStream::NO_MAXIMUM_READ_SIZE);
    std::string readStringUntil(const std::string &until, unsigned long maximumReadSize = TStream::NO_MAXIMUM_READ_SIZE);
    std::string readStringUntil(const char *until, unsigned long maximumReadSize = TStream::NO_MAXIMUM_READ_SIZE);
    std::string readStringUntil(char until);
    ssize_t available();
    void startListening();
    void stopListening();
    bool isListening() const;
    void putBack(const UDPDatagram &datagram);
    void putBack(const std::string &str);
    void putBack(const char *str);
    void putBack(char back);

    UDPDatagram peekDatagram();
    std::string peek();
    char peekByte();

    void flushRXTX();
    void flushRX();
    void flushTX();
    
    unsigned long int serverTimeout() const;
    void setServerPortNumber(uint16_t portNumber);
    uint16_t serverPortNumber() const;
    void setServerTimeout(unsigned long timeout);
    void flush();

    /*Both - TStream interface compliance*/
    void openPort();
    void closePort();
    bool isOpen() const;
    unsigned long timeout() const;
    std::string portName() const;
    void setTimeout(unsigned long timeout);

    static std::string parseLineEnding(LineEnding lineEnding);
    static LineEnding parseLineEndingFromRaw(const std::string &lineEnding);
    static std::string lineEndingToString(LineEnding lineEnding);
    
    static UDPObjectType parseUDPObjectTypeFromRaw(const std::string &udpObjectType);
    static std::string udpObjectTypeToString(UDPObjectType udpObjectType);
    
    void setLineEnding(LineEnding lineEnding);
    LineEnding lineEnding() const;

    UDPObjectType udpObjectType() const;

    static uint16_t doUserSelectClientPortNumber();
    static std::string doUserSelectClientHostName();
    static uint16_t doUserSelectServerPortNumber();
    static UDPObjectType doUserSelectUDPObjectType();
    static std::shared_ptr<UDPDuplex> doUserSelectUDPDuplex();
    
    static const char *s_DEFAULT_CLIENT_HOST_NAME;

    static const constexpr uint16_t s_DEFAULT_CLIENT_PORT_NUMBER{8887};
    static const constexpr unsigned long s_DEFAULT_CLIENT_TIMEOUT{100};

    static const constexpr uint16_t s_DEFAULT_SERVER_PORT_NUMBER{8888};
    static const constexpr unsigned long s_DEFAULT_SERVER_TIMEOUT{100};

private:
    std::unique_ptr<UDPServer> m_udpServer;
    std::unique_ptr<UDPClient> m_udpClient;
    UDPObjectType m_udpObjectType;
    
    int resolveAddressHelper(const std::string &hostName, int family, const std::string &service, sockaddr_storage* addressPtr);

    void initialize();

    static constexpr bool isValidPortNumber(int portNumber);
};

#endif //TJLUTILS_UDPDUPLEX_H