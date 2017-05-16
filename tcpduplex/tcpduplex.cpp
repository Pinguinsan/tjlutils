/***********************************************************************
*    tcpduplex.cpp:                                                    *
*    TCPDuplex for sending/receiving data over a TCP port number       *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                          *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a TCPDuplex class           *
*    It is used to send and receive data over a specified TCP          *
*    port number, to a specified IP address, with DNS lookup supported *
*    This class also implements the TStream interface                  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "tcpduplex.h"


//Loopback
const char *TCPDuplex::DEFAULT_CLIENT_HOST_NAME{TCPClient::DEFAULT_HOST_NAME};

TCPDuplex::TCPDuplex(TCPObjectType tcpObjectType) :
    TCPDuplex(TCPDuplex::DEFAULT_CLIENT_HOST_NAME, 
              TCPDuplex::DEFAULT_CLIENT_PORT_NUMBER, 
              TCPDuplex::DEFAULT_SERVER_PORT_NUMBER,
              TCPDuplex::DEFAULT_CLIENT_RETURN_ADDRESS_PORT_NUMBER,
              tcpObjectType)
{

}

TCPDuplex::TCPDuplex(uint16_t serverPortNumber, TCPObjectType tcpObjectType) :
    TCPDuplex(TCPDuplex::DEFAULT_CLIENT_HOST_NAME, 
              TCPDuplex::DEFAULT_CLIENT_PORT_NUMBER, 
              serverPortNumber,
              TCPDuplex::DEFAULT_CLIENT_RETURN_ADDRESS_PORT_NUMBER,
              tcpObjectType)
{

}

TCPDuplex::TCPDuplex(const std::string &clientHostName, TCPObjectType tcpObjectType) :
    TCPDuplex(clientHostName, 
              TCPDuplex::DEFAULT_CLIENT_PORT_NUMBER, 
              TCPDuplex::DEFAULT_SERVER_PORT_NUMBER,
              TCPDuplex::DEFAULT_CLIENT_RETURN_ADDRESS_PORT_NUMBER,
              tcpObjectType)
{

}

TCPDuplex::TCPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, TCPObjectType tcpObjectType) :
    TCPDuplex(clientHostName, 
              clientPortNumber, 
              TCPDuplex::DEFAULT_SERVER_PORT_NUMBER,
              TCPDuplex::DEFAULT_CLIENT_RETURN_ADDRESS_PORT_NUMBER,
              tcpObjectType)
{

}

TCPDuplex::TCPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber, TCPObjectType tcpObjectType) :
    TCPDuplex(clientHostName, 
              clientPortNumber, 
              serverPortNumber,
              TCPDuplex::DEFAULT_CLIENT_RETURN_ADDRESS_PORT_NUMBER,
              tcpObjectType)

{

}


TCPDuplex::TCPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber, uint16_t clientReturnAddressPortNumber, TCPObjectType tcpObjectType) :
    m_tcpClient{nullptr},
    m_tcpServer{nullptr},
    m_tcpObjectType{tcpObjectType}
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        this->m_tcpClient = std::unique_ptr<TCPClient>{new TCPClient{clientHostName, 
                                                                     clientPortNumber,
                                                                     clientReturnAddressPortNumber}};
    }
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        if (this->m_tcpClient != nullptr) {
            this->m_tcpServer = std::unique_ptr<TCPServer>{new TCPServer{}};
        } else {
            this->m_tcpServer = std::unique_ptr<TCPServer>{new TCPServer{serverPortNumber}};
        }
    }
    this->setServerTimeout(TCPDuplex::DEFAULT_SERVER_TIMEOUT);
}

std::string TCPDuplex::lineEnding() const
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->lineEnding();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) {
        return this->m_tcpClient->lineEnding();
    } else {
        return this->m_tcpServer->lineEnding() + " " + this->m_tcpClient->lineEnding();
    }
}

void TCPDuplex::setLineEnding(const std::string &lineEnding)
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) {
        this->m_tcpClient->setLineEnding(lineEnding);
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        this->m_tcpServer->setLineEnding(lineEnding);
    } else {
        this->m_tcpClient->setLineEnding(lineEnding);
        this->m_tcpServer->setLineEnding(lineEnding);
    }
}

void TCPDuplex::openPort()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        this->m_tcpServer->openPort();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) {
        this->m_tcpClient->openPort();
    } else {
        this->m_tcpServer->openPort();
        this->m_tcpClient->openPort();
    }
}

void TCPDuplex::closePort()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        this->m_tcpServer->closePort();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) {
        this->m_tcpClient->closePort();
    } else {
        this->m_tcpServer->closePort();
        this->m_tcpClient->closePort();
    }
}

bool TCPDuplex::isOpen() const
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->isOpen();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) {
        return this->m_tcpClient->isOpen();
    } else {
        return ((this->m_tcpClient->isOpen()) && (this->m_tcpServer->isOpen()));
    }
}

void TCPDuplex::setClientPortNumber(uint16_t portNumber)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
       this->m_tcpClient->setPortNumber(portNumber);
    }
}

void TCPDuplex::setClientHostName(const std::string &hostName)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        this->m_tcpClient->setHostName(hostName);
    }
}

void TCPDuplex::setClientReturnAddressPortNumber(uint16_t portNumber)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
       this->m_tcpClient->setReturnAddressPortNumber(portNumber);
    }
}

void TCPDuplex::setClientTimeout(long timeout)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        this->m_tcpClient->setTimeout(timeout);   
    }
}

void TCPDuplex::setTimeout(long timeout)
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        this->m_tcpServer->setTimeout(timeout);
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) {
        this->m_tcpClient->setTimeout(timeout);
    } else {
        this->m_tcpClient->setTimeout(timeout);
        this->m_tcpServer->setTimeout(timeout);
    }
}

uint16_t TCPDuplex::clientPortNumber() const
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpClient->portNumber();
    } else {
        return 0;
    }
}

std::string TCPDuplex::clientHostName() const
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpClient->hostName();
    } else {
        return "";
    }
}
    
uint16_t TCPDuplex::clientReturnAddressPortNumber() const
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpClient->m_tcpSocketIndex;
    } else {
        return 0;
    }
}

long TCPDuplex::clientTimeout() const
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpClient->timeout();
    } else {
        return 0;
    }
}

uint16_t TCPDuplex::serverPortNumber() const
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpServer->portNumber();   
    } else {
        return 0;
    }
}

long TCPDuplex::serverTimeout() const
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpServer->timeout();
    } else {
        return 0;
    }
}

void TCPDuplex::setServerPortNumber(uint16_t portNumber)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        this->m_tcpServer->setPortNumber(portNumber);
    }
}

void TCPDuplex::setServerTimeout(long timeout)
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        this->m_tcpServer->setTimeout(timeout); 
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        this->m_tcpServer->setTimeout(this->m_tcpClient->m_tcpSocketIndex, timeout);
    }
}

long TCPDuplex::timeout() const
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->timeout();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) {
        return this->m_tcpClient->timeout();
    } else {
        return (this->m_tcpClient->timeout() > this->m_tcpServer->timeout() 
                ? this->m_tcpClient->timeout() : this->m_tcpServer->timeout());
    }
    
}

std::string TCPDuplex::portName() const
{   
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpClient->hostName();
    } else {
        return "server - no port name";
    }
}

void TCPDuplex::flushRXTX()
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        this->m_tcpServer->flushRXTX();
    }
} 

void TCPDuplex::flushRX()
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        this->m_tcpServer->flushRX();
    }
}

void TCPDuplex::flushTX()
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        this->m_tcpServer->flushTX();
    }
}  

ssize_t TCPDuplex::writeLine(const char *str)
{ 
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpClient->writeLine(str); 
    } else {
        return 0;
    }
}

ssize_t TCPDuplex::writeLine(const std::string &str)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_CLIENT) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpClient->writeLine(str);
    } else {
        return 0;
    }
}

TCPDatagram TCPDuplex::readDatagram()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->readDatagram();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->readDatagram(this->m_tcpClient->m_tcpSocketIndex);
    } else {
        return TCPDatagram{};
    }    
}

char TCPDuplex::readByte()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->readByte();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->readByte(this->m_tcpClient->m_tcpSocketIndex);
    } else {
        return 0;
    }
}

std::string TCPDuplex::readLine()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->readLine();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->readLine(this->m_tcpClient->m_tcpSocketIndex);
    } else {
        return "";
    }
}

std::string TCPDuplex::readUntil(const std::string &str)
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->readUntil(str);
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->readUntil(this->m_tcpClient->m_tcpSocketIndex, str);
    } else {
        return "";
    }
}

std::string TCPDuplex::readUntil(const char *str)
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->readUntil(str);
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->readUntil(this->m_tcpClient->m_tcpSocketIndex, str);
    }  else {
        return "";
    }
}

std::string TCPDuplex::readUntil(char until)
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->readUntil(until);
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->readUntil(this->m_tcpClient->m_tcpSocketIndex, until);
    } else {
        return "";
    }
}

void TCPDuplex::putBack(const TCPDatagram &datagram)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpServer->putBack(datagram);
    }
}

void TCPDuplex::putBack(const std::string &str)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpServer->putBack(str);
    }
}

void TCPDuplex::putBack(const char *str)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpServer->putBack(str);
    }
}

void TCPDuplex::putBack(char back)
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpServer->putBack(back);
    }
}

std::string TCPDuplex::peek()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->peek();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->peek(this->m_tcpClient->m_tcpSocketIndex);
    } else {
        return "";
    }
}

TCPDatagram TCPDuplex::peekDatagram()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->peekDatagram();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->peekDatagram(this->m_tcpClient->m_tcpSocketIndex);
    } else {
        return TCPDatagram{};
    }
}

char TCPDuplex::peekByte()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->peekByte();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->peekByte(this->m_tcpClient->m_tcpSocketIndex);
    } else {
        return 0;
    }
}
    
ssize_t TCPDuplex::available()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        return this->m_tcpServer->available();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return this->m_tcpServer->available(this->m_tcpClient->m_tcpSocketIndex);
    } else {
        return 0;
    }
}

void TCPDuplex::startListening()
{
    if (this->m_tcpObjectType == TCPObjectType::TCP_SERVER) {
        this->m_tcpServer->startListening();
    } else if (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        this->m_tcpServer->startListening(this->m_tcpClient->m_tcpSocketIndex);
    }
}

void TCPDuplex::stopListening()
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpServer->stopListening();
    }
}

bool TCPDuplex::isListening() const
{
    if ((this->m_tcpObjectType == TCPObjectType::TCP_SERVER) || (this->m_tcpObjectType == TCPObjectType::TCP_DUPLEX)) {
        return this->m_tcpServer->isListening();    
    } else {
        return 0;
    }
}

TCPObjectType TCPDuplex::tcpObjectType() const
{
    return this->m_tcpObjectType;
}

bool constexpr TCPDuplex::isValidPortNumber(int portNumber)
{
    return ((portNumber > 0) && (portNumber < std::numeric_limits<uint16_t>::max()));
}

uint16_t TCPDuplex::doUserSelectClientPortNumber()
{
    return TCPClient::doUserSelectPortNumber();
}

uint16_t TCPDuplex::doUserSelectClientReturnAddressPortNumber()
{
    return TCPClient::doUserSelectReturnAddressPortNumber();
}

std::string TCPDuplex::doUserSelectClientHostName()
{
    return TCPClient::doUserSelectHostName();
}

uint16_t TCPDuplex::doUserSelectServerPortNumber()
{
    return TCPServer::doUserSelectPortNumber();
}

TCPObjectType TCPDuplex::doUserSelectTCPObjectType()
{
    std::vector<std::string> tcpObjectTypeChoices{TCPDuplex::tcpObjectTypeToString(TCPObjectType::TCP_DUPLEX),
                                                  TCPDuplex::tcpObjectTypeToString(TCPObjectType::TCP_SERVER),
                                                  TCPDuplex::tcpObjectTypeToString(TCPObjectType::TCP_CLIENT)};
    return GeneralUtilities::doUserSelectParameter("TCP Object Type",
                                                    static_cast< std::function<TCPObjectType(const std::string &)> >(TCPDuplex::parseTCPObjectTypeFromRaw),
                                                    tcpObjectTypeChoices,
                                                    TCPDuplex::tcpObjectTypeToString(TCPObjectType::TCP_DUPLEX).c_str());

}

std::shared_ptr<TCPDuplex> TCPDuplex::doUserSelectTCPDuplex()
{
    std::shared_ptr<TCPDuplex> tcpDuplex{nullptr};
    TCPObjectType tcpObjectType{TCPDuplex::doUserSelectTCPObjectType()};
    if (tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        std::string clientHostName{TCPDuplex::doUserSelectClientHostName()};
        uint16_t clientPortNumber{TCPDuplex::doUserSelectClientPortNumber()};
        uint16_t clientReturnAddressPortNumber{TCPDuplex::doUserSelectClientReturnAddressPortNumber()};
        uint16_t serverPortNumber{TCPDuplex::doUserSelectServerPortNumber()};
        tcpDuplex = std::make_shared<TCPDuplex>(clientHostName, 
                                                clientPortNumber,
                                                serverPortNumber,
                                                clientReturnAddressPortNumber, 
                                                tcpObjectType);

    } else if (tcpObjectType == TCPObjectType::TCP_SERVER) {
        uint16_t serverPortNumber{TCPDuplex::doUserSelectServerPortNumber()};
        tcpDuplex = std::make_shared<TCPDuplex>(serverPortNumber, tcpObjectType);
    } else if (tcpObjectType == TCPObjectType::TCP_CLIENT) {
        std::string clientHostName{TCPDuplex::doUserSelectClientHostName()};
        uint16_t clientPortNumber{TCPDuplex::doUserSelectClientPortNumber()};
        uint16_t clientReturnAddressPortNumber{TCPDuplex::doUserSelectClientReturnAddressPortNumber()};
        tcpDuplex = std::make_shared<TCPDuplex>(clientHostName,
                                                clientPortNumber,
                                                clientReturnAddressPortNumber,
                                                tcpObjectType);
    } else {
        throw std::runtime_error("Unknown TCPObjectType passed to TCPDuplex::doUserSelectTCPDuplex()");
    }
    return tcpDuplex;
    
}

TCPObjectType TCPDuplex::parseTCPObjectTypeFromRaw(const std::string &tcpObjectType)
{
    using namespace GeneralUtilities;
    std::string copyString{tcpObjectType};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    std::string secondCopy{""};
    for (auto &it : copyString) {
        if (it != ' ') {
            secondCopy += it;
        }
    }
    if (secondCopy == "tcpduplex") {
        return TCPObjectType::TCP_DUPLEX;
    } else if (secondCopy == "tcpserver") {
        return TCPObjectType::TCP_SERVER;
    } else if (secondCopy == "tcpclient") {
        return TCPObjectType::TCP_CLIENT;
    } else {
        throw std::runtime_error("Could not parse TCPObjectType in TCPDuplex::parseTCPObjectTypeFromRaw(const std::string &): unknown identifier " + tQuoted(tcpObjectType));
    }
}

std::string TCPDuplex::tcpObjectTypeToString(TCPObjectType tcpObjectType)
{
    if (tcpObjectType == TCPObjectType::TCP_DUPLEX) {
        return "TCP Duplex";
    } else if (tcpObjectType == TCPObjectType::TCP_SERVER) {
        return "TCP Server";
    } else if (tcpObjectType == TCPObjectType::TCP_CLIENT) {
        return "TCP Client";
    } else {
        throw std::runtime_error("Unknown TCPObjectType passed to TCPDuplex::tcpObjectTypeToString(TCPObjectType)");
    }
}
