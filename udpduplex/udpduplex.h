/***********************************************************************
*    udpDuplex.h:                                                      *
*    UDPDuplex for sending/receiving datagrams over a UDP port number  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a UDPDuplex class             *
*    It is used to send and receive datagrams over a specified UDP     *
*    port number, to a specified IP address, with DNS lookup supported *
*    This class also implements the TStream interface                  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_UDPDUPLEX_H
#define TJLUTILS_UDPDUPLEX_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <exception>
#include <sstream>
#include <limits>



#if defined (_WIN32)

#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <net/if.h>
#endif //defined(_WIN32)

#include "ibytestream.h"

enum class UDPObjectType {
    Duplex,
    Server,
    Client
};


#if defined(__ANDROID__)
    using platform_socklen_t = socklen_t;
#elif defined(_WIN32)
     using platform_socklen_t = int;
#else
    using platform_socklen_t = socklen_t;
#endif

class UDPDatagram
{
public:
    UDPDatagram(struct sockaddr_in socketAddress, const std::string &message) :
        m_message{message}
    { 
       this->m_socketAddress.sin_family = socketAddress.sin_family;
       this->m_socketAddress.sin_port = socketAddress.sin_port;
       this->m_socketAddress.sin_addr.s_addr = socketAddress.sin_addr.s_addr;
    }

    UDPDatagram() :
        m_message{""} 
    { 
       this->m_socketAddress.sin_family = 0;
       this->m_socketAddress.sin_port = 0;
       this->m_socketAddress.sin_addr.s_addr = 0;
    }

    struct sockaddr_in socketAddress () const 
    {
        sockaddr_in returnSocket;
        returnSocket.sin_family = this->m_socketAddress.sin_family;
        returnSocket.sin_port = this->m_socketAddress.sin_port;
        returnSocket.sin_addr.s_addr = this->m_socketAddress.sin_addr.s_addr; 
        return returnSocket;
    }

    uint16_t portNumber() const { return ntohs(this->m_socketAddress.sin_port); }
    std::string message() const { return this->m_message; }
    std::string hostName() const  { 
        char lowLevelTempBuffer[INET_ADDRSTRLEN];
        memset(lowLevelTempBuffer, '\0', INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(this->m_socketAddress.sin_addr), lowLevelTempBuffer, INET_ADDRSTRLEN);
        return std::string{lowLevelTempBuffer};
    }

private:
    struct sockaddr_in m_socketAddress;
    std::string m_message;
};


class UDPClient;
class UDPServer;

class UDPDuplex : public IByteStream
{
public:
    UDPDuplex(UDPObjectType udpObjectType = UDPObjectType::UDP_Duplex);
    UDPDuplex(uint16_t clientPortNumber, UDPObjectType udpObjectType = UDPObjectType::UDP_Duplex);
    UDPDuplex(const std::string &clientHostName, UDPObjectType udpObjectType = UDPObjectType::UDP_Duplex);
    UDPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, UDPObjectType udpObjectType = UDPObjectType::UDP_Duplex);
    UDPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber, UDPObjectType udpObjectType = UDPObjectType::UDP_Duplex);
    UDPDuplex(const std::string &clientHostName, uint16_t clientPortNumber, uint16_t serverPortNumber, uint16_t clientReturnAddressPortNumber, UDPObjectType udpObjectType = UDPObjectType::UDP_Duplex);

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
    UDPDatagram readDatagram();
    std::string readLine();
    std::string readUntil(const std::string &until);
    std::string readUntil(const char *until);
    std::string readUntil(char until);
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
    
    static UDPObjectType parseUDPObjectTypeFromRaw(const std::string &udpObjectType);
    static std::string udpObjectTypeToString(UDPObjectType udpObjectType);
    
    void setLineEnding(const std::string &lineEnding);
    std::string lineEnding() const;

    UDPObjectType udpObjectType() const;

    static uint16_t doUserSelectClientPortNumber();
    static std::string doUserSelectClientHostName();
    static uint16_t doUserSelectClientReturnAddressPortNumber();
    static uint16_t doUserSelectServerPortNumber();
    static UDPObjectType doUserSelectUDPObjectType();
    static std::shared_ptr<UDPDuplex> doUserSelectUDPDuplex();
    
    static const char *DEFAULT_CLIENT_HOST_NAME;

    static const constexpr uint16_t DEFAULT_CLIENT_PORT_NUMBER{UDPClient::DEFAULT_PORT_NUMBER};
    static const constexpr uint16_t DEFAULT_CLIENT_RETURN_ADDRESS_PORT_NUMBER{UDPClient::DEFAULT_RETURN_ADDRESS_PORT_NUMBER};
    static const constexpr long DEFAULT_CLIENT_TIMEOUT{UDPClient::DEFAULT_TIMEOUT};

    static const constexpr uint16_t DEFAULT_SERVER_PORT_NUMBER{UDPServer::DEFAULT_PORT_NUMBER};
    static const constexpr long DEFAULT_SERVER_TIMEOUT{UDPServer::DEFAULT_TIMEOUT};

private:
    std::unique_ptr<UDPServer> m_udpServer;
    std::unique_ptr<UDPClient> m_udpClient;
    UDPObjectType m_udpObjectType;
    
    int resolveAddressHelper(const std::string &hostName, int family, const std::string &service, sockaddr_storage* addressPtr);

    void initialize();

    static constexpr bool isValidPortNumber(int portNumber);
};

class UDPServer
{
friend class UDPDuplex;
public:
    UDPServer();
    UDPServer(uint16_t port);
    ~UDPServer();

    char readByte();
    UDPDatagram readDatagram();
    std::string readLine();
    std::string readUntil(const std::string &until);
    std::string readUntil(const char *until);
    std::string readUntil(char until);
    ssize_t available();
    void startListening();
    void stopListening();
    bool isListening() const;
    std::string lineEnding() const;

    long timeout() const;
    void setPortNumber(uint16_t portNumber);
    uint16_t portNumber() const;
    void setTimeout(long timeout);
    void flushRXTX();
    void flushRX();
    void flushTX();

    void openPort();
    void closePort();
    bool isOpen() const;

    void putBack(const std::string &str);
    void putBack(const char *str);
    void putBack(char back);
    void putBack(const UDPDatagram &datagram);

    void setLineEnding(const std::string &str);
    std::string peek();
    char peekByte();
    UDPDatagram peekDatagram();

    static uint16_t doUserSelectPortNumber();
    static std::shared_ptr<UDPServer> doUserSelectUDPServer();

    static const constexpr uint16_t DEFAULT_PORT_NUMBER{8888};
    static const constexpr unsigned int DEFAULT_TIMEOUT{100};

private:
    struct sockaddr_in m_socketAddress;
    int m_setSocketResult;
    bool m_isListening;
    long m_timeout;
    std::deque<UDPDatagram> m_datagramQueue;
    std::mutex m_ioMutex;
    bool m_shutEmDown;
    std::string m_lineEnding;

    void initialize(uint16_t portNumber);
#if defined(__ANDROID__)
    std::thread *m_asyncFuture;
#else
    std::future<void> m_asyncFuture;
#endif

    void asyncDatagramListener();
    void syncDatagramListener();

    char readByte(int socketNumber);
    UDPDatagram readDatagram(int socketNumber);
    std::string readLine(int socketNumber);
    std::string readUntil(int socketNumber, const std::string &until);
    std::string readUntil(int socketNumber, const char *until);
    std::string readUntil(int socketNumber, char until);
    ssize_t available(int socketNumber);
    
    std::string peek(int socketNumber);
    char peekByte(int socketNumber);
    UDPDatagram peekDatagram(int socketNumber);
    void asyncDatagramListener(int socketNumber);
    void syncDatagramListener(int socketNumber);
    void setTimeout(int socketNumber, long timeout);

    void startListening(int socketNumber);

    static const uint16_t BROADCAST;
    static const constexpr size_t RECEIVED_BUFFER_MAX{65535};
    static const constexpr size_t MAXIMUM_BUFFER_SIZE{65535};

    static constexpr bool isValidPortNumber(int portNumber);

};


class UDPClient
{
    friend class UDPDuplex;
public:
    UDPClient();
    UDPClient(uint16_t portNumber);
    UDPClient(const std::string &hostName);
    UDPClient(const std::string &hostName, uint16_t portNumber);
    UDPClient(const std::string &hostName, uint16_t portNumber, uint16_t clientReturnAddressPortNumber);
    ~UDPClient();

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
    static std::shared_ptr<UDPClient> doUserSelectUDPClient();
private:
    sockaddr_in m_destinationAddress;
    sockaddr_in m_returnAddress;

    unsigned int m_timeout;
    int m_udpSocketIndex;
    std::string m_lineEnding;
    
    ssize_t writeByte(char toSend);
    int resolveAddressHelper(const std::string &hostName, int family, const std::string &service, sockaddr_storage* addressPtr);
    void initialize(const std::string &hostName, uint16_t portNumber, uint16_t returnAddressPortNumber);

    
    static constexpr bool isValidPortNumber(int portNumber);
};

inline constexpr bool endsWith(const std::string &stringToCheck, const std::string &matchString)
{
    return (matchString.size() > stringToCheck.size()) ? false : std::equal(matchString.rbegin(), matchString.rend(), stringToCheck.rbegin());
}

inline constexpr bool endsWith(const std::string &stringToCheck, char matchChar)
{
    return endsWith(stringToCheck, std::string(1, matchChar));
}

template <typename T> static inline std::string toStdString(const T &t) { 
    return dynamic_cast<std::stringstream &>(std::stringstream{} << t).str(); 
}
template <typename T> static inline std::string tQuoted(const T &t) {
    return "\"" + toStdString(t) + "\"";
}

template<typename T, typename TLow, typename THigh>
T doUserEnterNumericParameter(const std::string &name,
                                const std::function<bool(T)> &validator,
                                TLow lowLimit,
                                THigh highLimit) {
    std::cout << std::endl;
    std::string userOption{""};
    while (true) {
        userOption = "";
        std::cout << "Enter a number to use for " << tQuoted(name) <<
        " between (inclusive) " << lowLimit << " and " << highLimit <<
        ", or press CTRL+C to quit: ";
        std::getline(std::cin, userOption);
        if (userOption == "") {
            continue;
        }
        int userEntry{0};
        try {
            userEntry = std::stoi(userOption);
            if (userEntry < lowLimit) {
                std::cout << tQuoted(userEntry) << " is less than the minimum value for " <<
                name << " (" << name << " < " << lowLimit << std::endl << std::endl;
                continue;
            } else if (userEntry > highLimit) {
                std::cout << tQuoted(userEntry) <<
                " is greater than the maximum value for " << name << " (" << name <<
                " > " << highLimit << std::endl << std::endl;
                continue;
            } else if (!validator(userEntry)) {
                std::cout << tQuoted(userEntry) << " is an invalid " << name << std::endl <<
                std::endl;
                continue;
            }
            return userEntry;
        } catch (std::exception &e) {
            std::cout << tQuoted(userOption) << " is not a number" << std::endl <<
            std::endl;
        }
    }
}

template<typename T>
std::string doUserEnterStringParameter(const T &name,
                                        const std::function<bool(std::string)> &validator) {
    std::cout << std::endl;
    std::string userOption{""};
    while (true) {
        userOption = "";
        std::cout << "Please enter a string to use for " << tQuoted(name) <<
        ", or press CTRL+C to quit: ";
        std::getline(std::cin, userOption);
        if (userOption == "") {
            continue;
        }
        try {
            if (!validator(userOption)) {
                std::cout << tQuoted(userOption) << " is an invalid " << name <<
                std::endl << std::endl;
                continue;
            }
            return userOption;
        } catch (std::exception &e) {
            std::cout << tQuoted(userOption) << " is an invalid " << name << std::endl <<
            std::endl;
        }
    }
}

template<typename T, typename TOps>
T doUserSelectParameter(const std::string &name,
                        const std::function<T(const std::string &)> &func,
                        const std::vector<TOps> &availableOptions,
                        const char *defaultOption) {
    std::cout << std::endl;
    if (availableOptions.size() == 0) {
        throw std::runtime_error("No " + name + " are available");
    } else if (availableOptions.size() == 1) {
        return func(availableOptions.at(0));
    }
    unsigned int quitOption{0};
    for (unsigned int selectionIndex = 1;
            selectionIndex <= availableOptions.size(); selectionIndex++) {
        std::cout << selectionIndex << ".) " << availableOptions.at(selectionIndex - 1);
        if (static_cast<std::string>(availableOptions.at(selectionIndex - 1)) ==
            static_cast<std::string>(defaultOption)) {
            std::cout << "    <----DEFAULT" << std::endl;
        } else {
            std::cout << std::endl;
        }
        quitOption = selectionIndex + 1;
    }
    std::cout << quitOption << ".) Quit" << std::endl << std::endl;
    std::string userOption{""};
    while (true) {
        userOption = "";
        std::cout << "Select a " << name <<
        " from the above options, or press CTRL+C to quit: ";
        std::getline(std::cin, userOption);
        if (userOption == "") {
            return func(defaultOption);
        }
        unsigned int userOptionIndex{0};
        bool userSelectedQuit{false};
        try {
            userOptionIndex = std::stoi(userOption);
            if (userOptionIndex > availableOptions.size() + 1) {
                std::cout << tQuoted(userOption) <<
                " wasn't one of the selections, please a number between (inclusive) 1 and " <<
                quitOption << ", or press CTRL+C to quit" << std::endl << std::endl;
                continue;
            }
            if (userOptionIndex == quitOption) {
                userSelectedQuit = true;
                throw std::invalid_argument(
                        "In GeneralUtilities::doUserSelectParameter(): User selected quit option");
            }
            return func(availableOptions.at(userOptionIndex - 1));
        } catch (std::exception &e) {
            if (userSelectedQuit) {
                throw e;
            }
            std::cout << tQuoted(userOption) <<
            " wasn't one of the selections, enter a number between (inclusive) 1 and " <<
            quitOption << ", or press CTRL+C to quit" << std::endl << std::endl;
        }
    }
}

#endif //TJLUTILS_UDPDUPLEX_H