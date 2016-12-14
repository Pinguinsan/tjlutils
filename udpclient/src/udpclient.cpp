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
    m_timeout{UDPClient::s_DEFAULT_TIMEOUT}
{
    if (!this->isValidPortNumber(this->m_portNumber)) {
        this->m_portNumber = UDPClient::s_DEFAULT_PORT_NUMBER;
        throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<int16_t>::max()) 
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
                                 std::to_string(std::numeric_limits<int16_t>::max()) 
                                 + "("
                                 + std::to_string(this->m_portNumber) 
                                 + ")");
    }
    this->initialize();
}

void UDPClient::setTimeout(unsigned int timeout)
{
    this->m_timeout = timeout;
}

void UDPClient::setHostName(const std::string &hostName)
{
    using namespace GeneralUtilities;
    if (resolveAddressHelper (this->m_hostName, AF_INET, std::to_string(this->m_portNumber), &this->m_destinationAddress) != 0) {
       throw std::runtime_error("ERROR: Error condition occurred while resolving address " + tQuoted(this->m_hostName) + ", with an error code of " + std::to_string(errno));
    }
    this->m_hostName = hostName;
    this->initialize();
}

uint16_t UDPClient::portNumber() const
{
    return this->m_portNumber;
}

unsigned int UDPClient::timeout() const
{
    return this->m_timeout;
}

void UDPClient::initialize()
{
    using namespace GeneralUtilities;
    this->m_udpSocketIndex = socket(AF_INET, SOCK_DGRAM, 0);
    this->m_listenAddress.sin_family = AF_INET;
    if (bind(this->m_udpSocketIndex, (sockaddr*)&this->m_listenAddress, sizeof(this->m_listenAddress)) != 0) {
       throw std::runtime_error("ERROR: An error condition occurred during binding of socket (error code " + std::to_string(errno) + ")");
    }


    if (resolveAddressHelper (this->m_hostName, AF_INET, std::to_string(this->m_portNumber), &this->m_destinationAddress) != 0) {
       throw std::runtime_error("ERROR: And error condition occurred while resolving address " + tQuoted(this->m_hostName) + ", with an error code of " + std::to_string(errno));
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

ssize_t UDPClient::writeString(const std::string &str)
{
    return ( sendto(this->m_udpSocketIndex, 
                    str.c_str(), 
                    static_cast<size_t>(str.length()),
                    0,
                    (sockaddr*)&this->m_destinationAddress,
                    sizeof(this->m_destinationAddress)) );
    
    /*
    ssize_t bytesWritten{0};
    for (auto &it : str) {
        bytesWritten += this->writeByte(it);
    }
    return bytesWritten;
    */
}

ssize_t UDPClient::writeByte(char toSend)
{
    std::string str{1, toSend};
    return ( sendto(this->m_udpSocketIndex, str.c_str(), 
             static_cast<size_t>(str.length()), 
             0, 
             (sockaddr*)&this->m_destinationAddress, 
             sizeof(this->m_destinationAddress)) );
}

bool constexpr UDPClient::isValidPortNumber(int portNumber)
{
    return ((portNumber > 0) && (portNumber < std::numeric_limits<int16_t>::max()));
}