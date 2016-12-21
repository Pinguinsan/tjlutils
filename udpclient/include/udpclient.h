/***********************************************************************
*    udpclient.h:                                                      *
*    UDPClient, for sending datagrams over a UDP port number           *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a UDPClient class             *
*    It is used to send datagrams over a specified UDP port number,    *
*    to a specified IP address, with DNS lookup supported              *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_UDPCLIENT_H
#define TJLUTILS_UDPCLIENT_H

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

class UDPClient
{
public:
    UDPClient();
    UDPClient(uint16_t portNumber);
    UDPClient(const std::string &hostName);
    UDPClient(const std::string &hostName, uint16_t portNumber);

    ssize_t writeByte(char toSend);
    ssize_t writeString(const char *str);
    ssize_t writeString(const std::string &str);
    uint16_t portNumber() const;
    std::string hostName() const;
    unsigned long int timeout() const;
    void setPortNumber(uint16_t portNumber);
    void setHostName(const std::string &hostName);
    void setTimeout(unsigned long int timeout);
    LineEnding lineEnding() const;
    void setLineEnding(LineEnding lineEnding);

    void openPort();
    void closePort();
    bool isOpen() const;

    static const char *s_DEFAULT_HOST_NAME;
    static const constexpr uint16_t s_DEFAULT_PORT_NUMBER{8888};
    static const constexpr unsigned int s_DEFAULT_TIMEOUT{100};
    static const constexpr unsigned int s_SEND_RETRY_COUNT{3};

    static uint16_t doUserSelectPortNumber();
    static std::string doUserSelectHostName();
    static std::shared_ptr<UDPClient> doUserSelectUDPClient();
    

    static std::string parseLineEnding(LineEnding lineEnding);
    static LineEnding parseLineEndingFromRaw(const std::string &lineEnding);
    static std::string lineEndingToString(LineEnding lineEnding);


private:
    std::string m_hostName;
    uint16_t m_portNumber;
    sockaddr_in m_listenAddress;
    sockaddr_storage m_destinationAddress;
    unsigned int m_timeout;
    int m_udpSocketIndex;
    std::string m_lineEnding;
    
    int resolveAddressHelper(const std::string &hostName, int family, const std::string &service, sockaddr_storage* addressPtr);

    void initialize();
    
    static constexpr bool isValidPortNumber(int portNumber);
    static const std::vector<const char *> s_AVAILABLE_LINE_ENDINGS;
    static const std::vector<const char *> s_NO_LINE_ENDING_IDENTIFIERS;
    static const std::vector<const char *> s_CARRIAGE_RETURN_IDENTIFIERS;
    static const std::vector<const char *> s_LINE_FEED_IDENTIFIERS;
    static const std::vector<const char *> s_CARRIAGE_RETURN_LINE_FEED_IDENTIFIERS;


};

#endif //TJLUTILS_UDPCLIENT_H