/*    tcpclient.cpp:                                                    *
 *    TCPClient, for sending datagrams over a TCP port number           *
 *    Copyright (c) 2016 Tyler Lewis                                    *
 ************************************************************************
 *    This is a header file for tjlutils:                               *
 *    https://github.serial/tlewiscpp/tjlutils                          *
 *    This file may be distributed with the entire tjlutils library,    *
 *    but may also be distributed as a standalone file                  *
 *    The source code is released under the GNU LGPL                    *
 *    This file holds the implementation of a TCPClient class           *
 *    It is used to send datagrams over a specified TCP port number,    *
 *    to a specified IP address, with DNS lookup supported              *
 *                                                                      *
 *    You should have received a copy of the GNU Lesser General         *
 *    Public license along with tjlutils                                *
 *    If not, see <http://www.gnu.org/licenses/>                        *
 ***********************************************************************/

#include "tcpclient.h"

//Loopback
const char *TCPClient::DEFAULT_HOST_NAME{"127.0.0.1"};
const std::string TCPClient::DEFAULT_LINE_ENDING{"\r\n"};

TCPClient::TCPClient() :
    TCPClient(static_cast<std::string>(TCPClient::DEFAULT_HOST_NAME),
              TCPClient::DEFAULT_PORT_NUMBER,
              TCPClient::DEFAULT_RETURN_ADDRESS_PORT_NUMBER)
{
    
}

TCPClient::TCPClient(uint16_t portNumber) :
    TCPClient(static_cast<std::string>(TCPClient::DEFAULT_HOST_NAME),
              portNumber,
              TCPClient::DEFAULT_RETURN_ADDRESS_PORT_NUMBER)
{

}

TCPClient::TCPClient(const std::string &hostName) :
    TCPClient(hostName,
              TCPClient::DEFAULT_PORT_NUMBER,
              TCPClient::DEFAULT_RETURN_ADDRESS_PORT_NUMBER)
{

}

TCPClient::TCPClient(const std::string &hostName, uint16_t portNumber) :
    TCPClient(hostName,
              portNumber,
              TCPClient::DEFAULT_RETURN_ADDRESS_PORT_NUMBER)
{

}

TCPClient::TCPClient(const std::string &hostName, uint16_t portNumber, uint16_t returnAddressPortNumber) :
    m_destinationAddress{},
    m_returnAddress{},
    m_tcpSocketIndex{0},
    m_timeout{DEFAULT_TIMEOUT},
    m_lineEnding{DEFAULT_LINE_ENDING}
{
    this->initialize(hostName,
                     portNumber,
                     returnAddressPortNumber);

}

uint16_t TCPClient::returnAddressPortNumber() const
{
    return ntohs(this->m_returnAddress.sin_port);
}

void TCPClient::setReturnAddressPortNumber(uint16_t portNumber)
{
    if (!isValidPortNumber(portNumber)) {
        throw std::runtime_error("ERROR: Invalid port set for TCPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max()) 
                                 + "("
                                 + std::to_string(portNumber) 
                                 + ")");
    }
    this->m_returnAddress.sin_port = htons(portNumber);
}

void TCPClient::setPortNumber(uint16_t portNumber)
{
    if (!isValidPortNumber(portNumber)) {
        throw std::runtime_error("ERROR: Invalid port set for TCPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max()) 
                                 + "("
                                 + std::to_string(portNumber) 
                                 + ")");
    }
    this->m_destinationAddress.sin_port = htons(portNumber);
}

void TCPClient::setLineEnding(const std::string &lineEnding)
{
    this->m_lineEnding = lineEnding;
}

std::string TCPClient::lineEnding() const
{
    return this->m_lineEnding;
}

void TCPClient::setTimeout(unsigned long int timeout)
{
    this->m_timeout = timeout;
}

void TCPClient::setHostName(const std::string &hostName)
{
    using namespace GeneralUtilities;
    sockaddr_storage temp{};
    if (resolveAddressHelper (hostName, AF_INET, std::to_string(this->portNumber()), &temp) != 0) {
       throw std::runtime_error("ERROR: TCPClient could not resolve adress " + tQuoted(hostName));
    }
    inet_pton(AF_INET, hostName.c_str(), &(this->m_destinationAddress.sin_addr));
}

void TCPClient::openPort()
{
    
}

void TCPClient::closePort()
{

}

bool TCPClient::isOpen() const
{
    return true;
}

std::string TCPClient::hostName() const
{
    char lowLevelTempBuffer[INET_ADDRSTRLEN];
    memset(lowLevelTempBuffer, '\0', INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(this->m_destinationAddress.sin_addr), lowLevelTempBuffer, INET_ADDRSTRLEN);
    return std::string{lowLevelTempBuffer};
}

uint16_t TCPClient::portNumber() const
{
    return ntohs(this->m_destinationAddress.sin_port);
}

unsigned long int TCPClient::timeout() const
{
    return this->m_timeout;
}

void TCPClient::initialize(const std::string &hostName, uint16_t portNumber, uint16_t returnAddressPortNumber)
{
    using namespace GeneralUtilities;
    this->m_tcpSocketIndex = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


    if (!this->isValidPortNumber(portNumber)) {
        uint16_t temp{portNumber};
        portNumber = TCPClient::DEFAULT_PORT_NUMBER;
        throw std::runtime_error("ERROR: Invalid port set for TCPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max()) 
                                 + "("
                                 + std::to_string(temp) 
                                 + ")");
    }
    if (!this->isValidPortNumber(returnAddressPortNumber)) {
        uint16_t temp{returnAddressPortNumber};
        returnAddressPortNumber = TCPClient::DEFAULT_RETURN_ADDRESS_PORT_NUMBER;
        throw std::runtime_error("ERROR: Invalid port set for TCPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<uint16_t>::max()) 
                                 + "("
                                 + std::to_string(temp) 
                                 + ")");
    }

    this->m_destinationAddress.sin_family = AF_INET;
    this->m_destinationAddress.sin_port = htons(portNumber);
    
    this->m_returnAddress.sin_family = AF_INET;
    this->m_returnAddress.sin_port = htons(returnAddressPortNumber);
    //This cannot be set with TCP sockets:
    //inet_pton(AF_INET, returnAddressHostName.c_str(), &(this->m_returnAddress.sin_addr));
    
    std::unique_ptr<sockaddr_storage> temp{std::unique_ptr<sockaddr_storage>{new sockaddr_storage{}}};
    if (resolveAddressHelper (hostName, AF_INET, std::to_string(this->portNumber()), temp.get()) != 0) {
       throw std::runtime_error("ERROR: TCPClient could not resolve adress " + tQuoted(hostName));
    } else {
        inet_pton(AF_INET, inet_ntoa(reinterpret_cast<sockaddr_in *>(temp.get())->sin_addr), &(this->m_destinationAddress.sin_addr));
    }

    if (bind(this->m_tcpSocketIndex, reinterpret_cast<sockaddr*>(&this->m_returnAddress), sizeof(this->m_returnAddress)) != 0) {
       throw std::runtime_error("ERROR: TCPClient could not bind socket " + tQuoted(this->m_tcpSocketIndex) + " (is something else using it?)");
    }
    
}

int TCPClient::resolveAddressHelper(const std::string &hostName, int family, const std::string &service, sockaddr_storage* addressPtr)
{
    int result{0};
    addrinfo* resultList{nullptr};
    addrinfo hints{};
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    result = getaddrinfo(hostName.c_str(), service.c_str(), &hints, &resultList);
    if (result == 0) {
        memcpy(addressPtr, resultList->ai_addr, resultList->ai_addrlen);
        freeaddrinfo(resultList);
    }
    return result;
}

ssize_t TCPClient::writeByte(char toSend) 
{ 
    return this->writeLine(std::string{1, toSend}); 
}

ssize_t TCPClient::writeLine(const char *str) 
{ 
    return this->writeLine(static_cast<std::string>(str)); 
}

ssize_t TCPClient::writeLine(const std::string &str)
{
    using namespace GeneralUtilities;
    std::string copyString{str};
    if (!endsWith(copyString, this->m_lineEnding)) {
        copyString += this->m_lineEnding;
    }
    unsigned int retryCount{0};
    do {
        ssize_t bytesWritten{sendto(this->m_tcpSocketIndex, 
                            copyString.c_str(), 
                            strlen(copyString.c_str()),
                            MSG_DONTWAIT,
                            reinterpret_cast<sockaddr*>(&this->m_destinationAddress),
                            sizeof(this->m_destinationAddress)) };
        if ((bytesWritten != -1) && (errno != EAGAIN) && (errno != EWOULDBLOCK))  {
            return bytesWritten;
        }
    } while (retryCount++ < TCPClient::SEND_RETRY_COUNT);
    return 0;
}

bool constexpr TCPClient::isValidPortNumber(int portNumber)
{
    return ((portNumber > 0) && (portNumber < std::numeric_limits<uint16_t>::max()));
}

std::string TCPClient::doUserSelectHostName()
{
    return GeneralUtilities::doUserEnterStringParameter("Client Host Name", [](std::string str) -> bool { return true; });
                                                        //TODO: Add validation for host name
} 

uint16_t TCPClient::doUserSelectPortNumber()
{
    return GeneralUtilities::doUserEnterNumericParameter("Client Port Number",
                                                         static_cast<std::function<bool(uint16_t)>>(TCPClient::isValidPortNumber),
                                                         std::numeric_limits<uint16_t>::min()+1,
                                                         std::numeric_limits<uint16_t>::max());
}

uint16_t TCPClient::doUserSelectReturnAddressPortNumber()
{
    return GeneralUtilities::doUserEnterNumericParameter("Client Return Address Port Number",
                                                         static_cast<std::function<bool(uint16_t)>>(TCPClient::isValidPortNumber),
                                                         std::numeric_limits<uint16_t>::min()+1,
                                                         std::numeric_limits<uint16_t>::max());
}

std::shared_ptr<TCPClient> TCPClient::doUserSelectTCPClient()
{
    uint16_t portNumber{TCPClient::doUserSelectPortNumber()};
    std::string hostName{TCPClient::doUserSelectHostName()};
    uint16_t returnAddressPortNumber{TCPClient::doUserSelectReturnAddressPortNumber()};
    std::shared_ptr<TCPClient> tcpClient{std::make_shared<TCPClient>(hostName, 
                                                                     portNumber, 
                                                                     returnAddressPortNumber)};
    return tcpClient;
}

TCPClient::~TCPClient()
{
    shutdown(this->m_tcpSocketIndex, SHUT_RDWR);
}

