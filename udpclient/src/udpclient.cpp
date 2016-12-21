/***********************************************************************
*    udpclient.cpp:                                                    *
*    UDPClient, for sending datagrams over a UDP port number           *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a UDPClient class           *
*    It is used to send datagrams over a specified UDP port number,    *
*    to a specified IP address, with DNS lookup supported              *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "udpclient.h"

//Loopback
const char *UDPClient::s_DEFAULT_HOST_NAME{"127.0.0.1"};

const std::vector<const char *> UDPClient::s_AVAILABLE_LINE_ENDINGS{"None", "CR", "LF", "CRLF"};
const std::vector<const char *> UDPClient::s_NO_LINE_ENDING_IDENTIFIERS{"n", "no", "none", "noline", "nolineending", "nolineendings"};
const std::vector<const char *> UDPClient::s_CARRIAGE_RETURN_IDENTIFIERS{"return", "carriagereturn", "carriage-return", "cr", "creturn"};
const std::vector<const char *> UDPClient::s_LINE_FEED_IDENTIFIERS{"feed", "line", "linefeed", "line-feed", "lf", "f", "lfeed"};
const std::vector<const char *> UDPClient::s_CARRIAGE_RETURN_LINE_FEED_IDENTIFIERS{"carriagereturnlinefeed", "crlf", "lfcr", "lfeedcreturn", "line-feed-carriage-return", "carriage-return-line-feed"};


UDPClient::UDPClient() :
    UDPClient(static_cast<std::string>(UDPClient::s_DEFAULT_HOST_NAME), UDPClient::s_DEFAULT_PORT_NUMBER)
{
    
}

UDPClient::UDPClient(uint16_t portNumber) :
    UDPClient(static_cast<std::string>(UDPClient::s_DEFAULT_HOST_NAME), portNumber)
{

}

UDPClient::UDPClient(const std::string &hostName) :
    UDPClient(hostName, UDPClient::s_DEFAULT_PORT_NUMBER)
{

}

UDPClient::UDPClient(const std::string &hostName, uint16_t portNumber) :
    m_hostName{hostName},
    m_portNumber{portNumber},
    m_listenAddress{},
    m_destinationAddress{},
    m_udpSocketIndex{},
    m_timeout{UDPClient::s_DEFAULT_TIMEOUT},
    m_lineEnding{""}
{
    if (!this->isValidPortNumber(this->m_portNumber)) {
        this->m_portNumber = UDPClient::s_DEFAULT_PORT_NUMBER;
        throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max()) 
                                 + "("
                                 + std::to_string(this->m_portNumber) 
                                 + ")");
    }
    this->initialize();
}

void UDPClient::setPortNumber(uint16_t portNumber)
{
    if (!isValidPortNumber(portNumber)) {
        throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max()) 
                                 + "("
                                 + std::to_string(portNumber) 
                                 + ")");
    }
    this->m_portNumber = portNumber;
    this->initialize();
}

void UDPClient::setLineEnding(LineEnding lineEnding)
{
    this->m_lineEnding = parseLineEnding(lineEnding);
}

LineEnding UDPClient::lineEnding() const
{
    return UDPClient::parseLineEndingFromRaw(this->m_lineEnding);
}

void UDPClient::setTimeout(unsigned long int timeout)
{
    this->m_timeout = timeout;
}

void UDPClient::setHostName(const std::string &hostName)
{
    using namespace GeneralUtilities;
    if (resolveAddressHelper (hostName, AF_INET, std::to_string(this->m_portNumber), &this->m_destinationAddress) != 0) {
       throw std::runtime_error("ERROR: UDPClient could not resolve adress " + tQuoted(hostName));
    }
    this->m_hostName = hostName;
    this->initialize();
}

void UDPClient::openPort()
{
    
}

void UDPClient::closePort()
{

}

bool UDPClient::isOpen() const
{
    return true;
}

std::string UDPClient::hostName() const
{
    return this->m_hostName;
}

uint16_t UDPClient::portNumber() const
{
    return this->m_portNumber;
}

unsigned long int UDPClient::timeout() const
{
    return this->m_timeout;
}

void UDPClient::initialize()
{
    using namespace GeneralUtilities;
    this->m_udpSocketIndex = socket(AF_INET, SOCK_DGRAM, 0);
    this->m_listenAddress.sin_family = AF_INET;
    if (bind(this->m_udpSocketIndex, reinterpret_cast<sockaddr*>(&this->m_listenAddress), sizeof(this->m_listenAddress)) != 0) {
       throw std::runtime_error("ERROR: UDPClient could not bind socket " + tQuoted(this->m_udpSocketIndex) + " (is something else using it?");
    }


    if (resolveAddressHelper (this->m_hostName, AF_INET, std::to_string(this->m_portNumber), &this->m_destinationAddress) != 0) {
       throw std::runtime_error("ERROR: UDPClient could not resolve adress " + tQuoted(this->m_hostName));
    }

}

int UDPClient::resolveAddressHelper(const std::string &hostName, int family, const std::string &service, sockaddr_storage* addressPtr)
{
    int result{0};
    addrinfo* resultList{nullptr};
    addrinfo hints{};
    hints.ai_family = family;
    hints.ai_socktype = SOCK_DGRAM; // without this flag, getaddrinfo will return 3x the number of addresses (one for each socket type).
    result = getaddrinfo(hostName.c_str(), service.c_str(), &hints, &resultList);
    if (result == 0) {
        //ASSERT(result_list->ai_addrlen <= sizeof(sockaddr_in));
        memcpy(addressPtr, resultList->ai_addr, resultList->ai_addrlen);
        freeaddrinfo(resultList);
    }
    return result;
}

ssize_t UDPClient::writeByte(char toSend) 
{ 
    return this->writeString(std::string{1, toSend}); 
}

ssize_t UDPClient::writeString(const char *str) 
{ 
    return this->writeString(static_cast<std::string>(str)); 
}

ssize_t UDPClient::writeString(const std::string &str)
{
    using namespace GeneralUtilities;
    std::string copyString{str};
    if (!endsWith(copyString, this->m_lineEnding)) {
        copyString += this->m_lineEnding;
    }
    unsigned int retryCount{0};
    do {
        ssize_t bytesWritten{sendto(this->m_udpSocketIndex, 
                            copyString.c_str(), 
                            strlen(copyString.c_str()),
                            MSG_DONTWAIT,
                            reinterpret_cast<sockaddr*>(&this->m_destinationAddress),
                            sizeof(this->m_destinationAddress)) };
        if ((bytesWritten != -1) && (errno != EAGAIN) && (errno != EWOULDBLOCK))  {
            this->initialize();
            return bytesWritten;
        }
    } while (retryCount++ < UDPClient::s_SEND_RETRY_COUNT);
    try {
        this->initialize();
    } catch (std::exception &e) {
        (void)e;
    }
    return 0;
}

bool constexpr UDPClient::isValidPortNumber(int portNumber)
{
    return ((portNumber > 0) && (portNumber < std::numeric_limits<uint16_t>::max()));
}


std::string UDPClient::parseLineEnding(LineEnding lineEnding)
{
    if (lineEnding == LineEnding::LE_CarriageReturn) {
        return "\r";
    } else if (lineEnding == LineEnding::LE_LineFeed) {
        return "\n";
    } else if (lineEnding == LineEnding::LE_CarriageReturnLineFeed) {
        return "\r\n";
    } else if (lineEnding == LineEnding::LE_None) {
        return "";
    } else {
        throw std::runtime_error("Unknown line ending passed to parseLineEnding(LineEnding): ");
    }
}

LineEnding UDPClient::parseLineEndingFromRaw(const std::string &lineEnding)
{
    std::string copyString{lineEnding};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    for (auto &it : UDPClient::s_NO_LINE_ENDING_IDENTIFIERS) {
        std::string tempString{static_cast<std::string>(it)};
        if ((copyString == tempString) && (copyString.length() == tempString.length()) && (copyString.find(tempString) == 0)) {
            return LineEnding::LE_None;
        }
    }
    for (auto &it : UDPClient::s_CARRIAGE_RETURN_IDENTIFIERS) {
        std::string tempString{static_cast<std::string>(it)};
        if ((copyString == tempString) && (copyString.length() == tempString.length()) && (copyString.find(tempString) == 0)) {
            return LineEnding::LE_CarriageReturn;
        }
    }
    for (auto &it : UDPClient::s_LINE_FEED_IDENTIFIERS) {
        std::string tempString{static_cast<std::string>(it)};
        if ((copyString == tempString) && (copyString.length() == tempString.length()) && (copyString.find(tempString) == 0)) {
            return LineEnding::LE_LineFeed;
        }
    }
    for (auto &it : UDPClient::s_CARRIAGE_RETURN_LINE_FEED_IDENTIFIERS) {
        std::string tempString{static_cast<std::string>(it)};
        if ((copyString == tempString) && (copyString.length() == tempString.length()) && (copyString.find(tempString) == 0)) {
            return LineEnding::LE_CarriageReturnLineFeed;
        }
    }
    throw std::runtime_error("Invalid line ending passed to parseLineEndingFromRaw(const std::string &): " + lineEnding);
}

std::string UDPClient::lineEndingToString(LineEnding lineEnding)
{
    if (lineEnding == LineEnding::LE_CarriageReturn) {
        return "\r";
    } else if (lineEnding == LineEnding::LE_LineFeed) {
        return "\n";
    } else if (lineEnding == LineEnding::LE_CarriageReturnLineFeed) {
        return "\r\n";
    } else {
        return "";
    }
}

std::string UDPClient::doUserSelectHostName()
{
    return GeneralUtilities::doUserEnterStringParameter("Client Host Name", [](std::string str) -> bool { return true; });
                                                        //TODO: Add validation for host name
} 

uint16_t UDPClient::doUserSelectPortNumber()
{
    return GeneralUtilities::doUserEnterNumericParameter("Client Port Number",
                                                         static_cast<std::function<bool(uint16_t)>>(UDPClient::isValidPortNumber),
                                                         std::numeric_limits<uint16_t>::min()+1,
                                                         std::numeric_limits<uint16_t>::max());
}

std::shared_ptr<UDPClient> UDPClient::doUserSelectUDPClient()
{
    uint16_t portNumber{UDPClient::doUserSelectPortNumber()};
    std::string hostName{UDPClient::doUserSelectHostName()};
    std::shared_ptr<UDPClient> udpClient{std::make_shared<UDPClient>(hostName, portNumber)};
    return udpClient;
}
