/***********************************************************************
*    udpduplex.cpp:                                                    *
*    UDPDuplex for sending/receiving datagrams over a UDP port number  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a UDPDuplex class           *
*    It is used to send and receive datagrams over a specified UDP     *
*    port number, to a specified IP address, with DNS lookup supported *
*    This class also implements the TStream interface                  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "udpduplex.h"


//Loopback
const char *UDPDuplex::s_DEFAULT_CLIENT_HOST_NAME{UDPClient::s_DEFAULT_HOST_NAME};

UDPDuplex::UDPDuplex() :
    UDPDuplex(UDPDuplex::s_DEFAULT_CLIENT_HOST_NAME, UDPDuplex::s_DEFAULT_CLIENT_PORT_NUMBER, UDPDuplex::s_DEFAULT_SERVER_PORT_NUMBER)
{

}

UDPDuplex::UDPDuplex(const std::string &clientHostname) :
    UDPDuplex(clientHostname, UDPDuplex::s_DEFAULT_CLIENT_PORT_NUMBER, UDPDuplex::s_DEFAULT_SERVER_PORT_NUMBER)
{

}

UDPDuplex::UDPDuplex(const std::string &clientHostname, uint16_t clientPortNumber) :
    UDPDuplex(clientHostname, clientPortNumber, UDPDuplex::s_DEFAULT_SERVER_PORT_NUMBER)
{

}

UDPDuplex::UDPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber) :
    m_udpClient{nullptr},
    m_udpServer{nullptr}
{
    if (!this->isValidPortNumber(serverPortNumber)) {
        throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<int16_t>::max()) 
                                 + "("
                                 + std::to_string(serverPortNumber) 
                                 + ")");
    }

    if (!this->isValidPortNumber(clientPortNumber)) {
        throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                 std::to_string(std::numeric_limits<int16_t>::max()) 
                                 + "("
                                 + std::to_string(clientPortNumber) 
                                 + ")");
    }

    this->m_udpClient = std::make_unique<UDPClient>(clientHostName, clientPortNumber);
    this->m_udpServer = std::make_unique<UDPServer>(serverPortNumber);
}

LineEnding UDPDuplex::lineEnding() const
{
    return this->m_udpClient->lineEnding();
}

void UDPDuplex::setLineEnding(LineEnding lineEnding)
{
    this->m_udpClient->setLineEnding(lineEnding);
}

void UDPDuplex::openPort()
{
    return this->m_udpServer->openPort();
    return this->m_udpClient->openPort();
}

void UDPDuplex::closePort()
{
    return this->m_udpServer->closePort();
    return this->m_udpClient->closePort();
}

bool UDPDuplex::isOpen() const
{
    return ((this->m_udpClient->isOpen()) && (this->m_udpServer->isOpen()));
}

void UDPDuplex::setClientPortNumber(uint16_t portNumber)
{
    this->m_udpClient->setPortNumber(portNumber);
}

void UDPDuplex::setClientHostName(const std::string &hostName)
{
    this->m_udpClient->setHostName(hostName);
}

void UDPDuplex::setClientTimeout(unsigned int timeout)
{
    this->m_udpClient->setTimeout(timeout);
}

void UDPDuplex::setTimeout(unsigned int timeout)
{
    this->m_udpClient->setTimeout(timeout);
    this->m_udpServer->setTimeout(timeout);
}

uint16_t UDPDuplex::clientPortNumber() const
{
    return this->m_udpClient->portNumber();
}

std::string UDPDuplex::clientHostName() const
{
    return this->m_udpClient->hostName();
}

unsigned int UDPDuplex::clientTimeout() const
{
    return this->m_udpClient->timeout();
}

uint16_t UDPDuplex::serverPortNumber() const
{
    return this->m_udpClient->portNumber();
}

unsigned int UDPDuplex::serverTimeout() const
{
    return this->m_udpServer->timeout();
}

void UDPDuplex::setServerPortNumber(uint16_t portNumber)
{
    this->m_udpServer->setPortNumber(portNumber);
}

void UDPDuplex::setServerTimeout(unsigned int timeout)
{
    this->m_udpServer->setTimeout(timeout);
}

unsigned int UDPDuplex::timeout() const
{
    return (this->m_udpClient->timeout() > this->m_udpServer->timeout() 
            ? this->m_udpClient->timeout() : this->m_udpServer->timeout());
}

std::string UDPDuplex::portName() const
{
    return this->m_udpClient->hostName();
};

void UDPDuplex::flush()
{
    this->m_udpServer->flush();
} 

ssize_t UDPDuplex::writeByte(char toSend) 
{ 
    return this->m_udpClient->writeByte(toSend); 
}

ssize_t UDPDuplex::writeString(const char *str)
{ 
    return this->m_udpClient->writeString(str); 
}

ssize_t UDPDuplex::writeString(const std::string &str)
{
    return this->m_udpClient->writeString(str);
}

char UDPDuplex::readByte()
{
    return this->m_udpServer->readByte();
}

std::string UDPDuplex::readString()
{
    return this->m_udpServer->readString();
}

std::string UDPDuplex::readStringUntil(const std::string &until)
{
    return this->m_udpServer->readStringUntil(until);
}

std::string UDPDuplex::readStringUntil(const char *until)
{
    return this->m_udpServer->readStringUntil(until);
}

std::string UDPDuplex::readStringUntil(char until)
{
    return this->m_udpServer->readStringUntil(until);
}

unsigned int UDPDuplex::available() const
{
    return this->m_udpServer->available();
}

void UDPDuplex::startListening()
{
    return this->m_udpServer->startListening();
}

void UDPDuplex::stopListening()
{
    return this->m_udpServer->stopListening();
}

bool UDPDuplex::isListening() const
{
    return this->m_udpServer->isListening();
}

bool constexpr UDPDuplex::isValidPortNumber(int portNumber)
{
    return ((portNumber > 0) && (portNumber < std::numeric_limits<int16_t>::max()));
}

std::string UDPDuplex::parseLineEnding(LineEnding lineEnding)
{
    return UDPClient::parseLineEnding(lineEnding);
}

LineEnding UDPDuplex::parseLineEndingFromRaw(const std::string &lineEnding)
{
    return UDPClient::parseLineEndingFromRaw(lineEnding);
}

std::string UDPDuplex::lineEndingToString(LineEnding lineEnding)
{
    return UDPClient::lineEndingToString(lineEnding);
}