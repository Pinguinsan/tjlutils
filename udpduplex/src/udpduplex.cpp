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

UDPDuplex::UDPDuplex(UDPObjectType udpObjectType) :
    UDPDuplex(UDPDuplex::s_DEFAULT_CLIENT_HOST_NAME, 
              UDPDuplex::s_DEFAULT_CLIENT_PORT_NUMBER, 
              UDPDuplex::s_DEFAULT_SERVER_PORT_NUMBER,
              m_udpObjectType)
{

}

UDPDuplex::UDPDuplex(const std::string &clientHostname, UDPObjectType m_udpObjectType) :
    UDPDuplex(clientHostname, 
              UDPDuplex::s_DEFAULT_CLIENT_PORT_NUMBER, 
              UDPDuplex::s_DEFAULT_SERVER_PORT_NUMBER,
              m_udpObjectType)
{

}

UDPDuplex::UDPDuplex(const std::string &clientHostname, uint16_t clientPortNumber, UDPObjectType udpObjectType) :
    UDPDuplex(clientHostname, 
    clientPortNumber, 
    UDPDuplex::s_DEFAULT_SERVER_PORT_NUMBER,
    m_udpObjectType)
{

}

UDPDuplex::UDPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber, UDPObjectType udpObjectType) :
    m_udpClient{nullptr},
    m_udpServer{nullptr},
    m_udpObjectType{udpObjectType}
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        if (!this->isValidPortNumber(serverPortNumber)) {
            throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                    std::to_string(std::numeric_limits<uint16_t>::max()) 
                                    + "("
                                    + std::to_string(serverPortNumber) 
                                    + ")");
        }
        this->m_udpServer = std::make_unique<UDPServer>(serverPortNumber);
    }


    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        if (!this->isValidPortNumber(clientPortNumber)) {
            throw std::runtime_error("ERROR: Invalid port set for UDPServer, must be between 1 and " +
                                    std::to_string(std::numeric_limits<uint16_t>::max()) 
                                    + "("
                                    + std::to_string(clientPortNumber) 
                                    + ")");
        }
        this->m_udpClient = std::make_unique<UDPClient>(clientHostName, clientPortNumber);
    }
}

LineEnding UDPDuplex::lineEnding() const
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpClient->lineEnding();
    } else {
        return LineEnding::LE_None;
    }
}

void UDPDuplex::setLineEnding(LineEnding lineEnding)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        this->m_udpClient->setLineEnding(lineEnding);
    }
}

void UDPDuplex::openPort()
{
    if (this->m_udpObjectType  == UDPObjectType::UDP_SERVER) {
        this->m_udpServer->openPort();
    } else if (this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) {
        this->m_udpClient->openPort();
    } else {
        this->m_udpServer->openPort();
        this->m_udpClient->openPort();
    }
}

void UDPDuplex::closePort()
{
    if (this->m_udpObjectType  == UDPObjectType::UDP_SERVER) {
        this->m_udpServer->closePort();
    } else if (this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) {
        this->m_udpClient->closePort();
    } else {
        this->m_udpServer->closePort();
        this->m_udpClient->closePort();
    }
}

bool UDPDuplex::isOpen() const
{
    if (this->m_udpObjectType  == UDPObjectType::UDP_SERVER) {
        return this->m_udpServer->isOpen();
    } else if (this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) {
        return this->m_udpClient->isOpen();
    } else {
        return ((this->m_udpClient->isOpen()) && (this->m_udpServer->isOpen()));
    }
}

void UDPDuplex::setClientPortNumber(uint16_t portNumber)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
       this->m_udpClient->setPortNumber(portNumber);
    }
}

void UDPDuplex::setClientHostName(const std::string &hostName)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        this->m_udpClient->setHostName(hostName);
    }
}

void UDPDuplex::setClientTimeout(unsigned int timeout)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        this->m_udpClient->setTimeout(timeout);   
    }
}

void UDPDuplex::setTimeout(unsigned int timeout)
{
    if (this->m_udpObjectType  == UDPObjectType::UDP_SERVER) {
        this->m_udpServer->setTimeout(timeout);
    } else if (this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) {
        this->m_udpClient->setTimeout(timeout);
    } else {
        this->m_udpClient->setTimeout(timeout);
        this->m_udpServer->setTimeout(timeout);
    }
}

uint16_t UDPDuplex::clientPortNumber() const
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpClient->portNumber();
    } else {
        return 0;
    }
}

std::string UDPDuplex::clientHostName() const
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpClient->hostName();
    } else {
        return "";
    }
}

unsigned int UDPDuplex::clientTimeout() const
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpClient->timeout();
    } else {
        return 0;
    }
}

uint16_t UDPDuplex::serverPortNumber() const
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpClient->portNumber();   
    } else {
        return 0;
    }
}

unsigned int UDPDuplex::serverTimeout() const
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->timeout();
    } else {
        return 0;
    }
}

void UDPDuplex::setServerPortNumber(uint16_t portNumber)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        this->m_udpServer->setPortNumber(portNumber);
    }
}

void UDPDuplex::setServerTimeout(unsigned int timeout)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        this->m_udpServer->setTimeout(timeout);
    }
}

unsigned int UDPDuplex::timeout() const
{
    if (this->m_udpObjectType  == UDPObjectType::UDP_SERVER) {
        return this->m_udpServer->timeout();
    } else if (this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) {
        return this->m_udpClient->timeout();
    } else {
        return (this->m_udpClient->timeout() > this->m_udpServer->timeout() 
                ? this->m_udpClient->timeout() : this->m_udpServer->timeout());
    }
    
}

std::string UDPDuplex::portName() const
{   
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpClient->hostName();
    } else {
        return "server - no port name";
    }
}

void UDPDuplex::flush()
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        this->m_udpServer->flush();
    }
} 

ssize_t UDPDuplex::writeByte(char toSend) 
{ 
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpClient->writeByte(toSend); 
    } else {
        return 0;
    }
}

ssize_t UDPDuplex::writeString(const char *str)
{ 
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpClient->writeString(str); 
    } else {
        return 0;
    }
}

ssize_t UDPDuplex::writeString(const std::string &str)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_CLIENT) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpClient->writeString(str);
    } else {
        return 0;
    }
}

char UDPDuplex::readByte()
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->readByte();
    } else {
        return 0;
    }
}

std::string UDPDuplex::readString(int maximumReadSize)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->readString(maximumReadSize);
    } else {
        return "";
    }
}

std::string UDPDuplex::readStringUntil(const std::string &str, int maximumReadSize)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->readStringUntil(str, maximumReadSize);
    } else {
        return "";
    }
}

std::string UDPDuplex::readStringUntil(const char *str, int maximumReadSize)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->readStringUntil(str, maximumReadSize);
    } else {
        return "";
    }
}

std::string UDPDuplex::readStringUntil(char until)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->readStringUntil(until);
    } else {
        return "";
    }
}

void UDPDuplex::putBack(const std::string &str)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->putBack(str);
    }
}

void UDPDuplex::putBack(const char *str)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->putBack(str);
    }
}

void UDPDuplex::putBack(char back)
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->putBack(back);
    }
}

std::string UDPDuplex::peek()
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->peek();
    } else {
        return "";
    }
}

char UDPDuplex::peekByte()
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->peekByte();
    } else {
        return 0;
    }
}
    
unsigned int UDPDuplex::available() const
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->available();
    } else {
        return 0;
    }
}

void UDPDuplex::startListening()
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->startListening();
    }
}

void UDPDuplex::stopListening()
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->stopListening();
    }
}

bool UDPDuplex::isListening() const
{
    if ((this->m_udpObjectType  == UDPObjectType::UDP_SERVER) || (this->m_udpObjectType  == UDPObjectType::UDP_DUPLEX)) {
        return this->m_udpServer->isListening();    
    } else {
        return 0;
    }
}

UDPObjectType UDPDuplex::udpObjectType() const
{
    return this->m_udpObjectType;
}

bool constexpr UDPDuplex::isValidPortNumber(int portNumber)
{
    return ((portNumber > 0) && (portNumber < std::numeric_limits<uint16_t>::max()));
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