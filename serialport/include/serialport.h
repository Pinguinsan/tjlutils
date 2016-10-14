/***********************************************************************
*    serialport.h:                                                     *
*    SerialPort class, for connecting to an RS232 serial port          *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a SerialPort class            *
*    It is used to connect to RS232 compliant serial ports             *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_SERIALPORT_H
#define TJLUTILS_SERIALPORT_H

#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif


#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <utility>
#include <set>
#include <vector>
#include <chrono>
#include <algorithm>
#include <systemcommand.h>

#if (defined(_WIN32) || defined(__CYGWIN__))
    #include <Windows.h>
#else
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <limits.h>
    #include <sys/file.h>
    #include <errno.h>
#endif

enum class StopBits { ONE, TWO };
enum class DataBits { FIVE, SIX, SEVEN, EIGHT };
enum class Parity { EVEN, ODD, NONE };

#if (defined(_WIN32) || defined(__CYGWIN__))
enum class BaudRate { BAUD110, BAUD300, BAUD600, BAUD1200,
                      BAUD2400, BAUD4800, BAUD9600, BAUD19200,
                      BAUD38400, BAUD57600, BAUD115200,
                      BAUD128000, BAUD256000, BAUD500000, BAUD1000000 };
#else
enum class BaudRate { BAUD50, BAUD75, BAUD110, BAUD134, BAUD150,
                      BAUD200, BAUD300, BAUD600, BAUD1200, BAUD1800,
                      BAUD2400, BAUD4800, BAUD9600, BAUD19200, BAUD38400,
                      BAUD57600, BAUD115200, BAUD230400, BAUD460800,
                      BAUD500000, BAUD576000, BAUD921600, BAUD1000000,
                      BAUD1152000, BAUD1500000, BAUD2000000, BAUD2500000,
                      BAUD3000000, BAUD3500000, BAUD4000000 };
#endif

class SerialPort
{
public:
    SerialPort(const std::string &name);
    SerialPort(const std::string &name, BaudRate baudRate);
    SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits);
    SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits);
    SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, Parity parity);
    SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, Parity parity);
    SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, StopBits stopBits, Parity parity);

    SerialPort(const std::string &name, DataBits dataBits);
    SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits);
    SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits, Parity parity);
    SerialPort(const std::string &name, DataBits dataBits, Parity parity);
    SerialPort(const std::string &name, StopBits stopBits);
    SerialPort(const std::string &name, StopBits stopBits, Parity parity);
    SerialPort(const std::string &name, Parity parity);

    SerialPort(SerialPort &&other);
    friend bool operator==(const SerialPort &lhs, const SerialPort &rhs);

    SerialPort &operator=(const SerialPort &rhs) = delete;
    SerialPort(const SerialPort &other) = delete;

    void openPort();
    void closePort();
    std::string readString();
    std::string readStringUntil(const std::string &str);
    std::string readStringUntil(char identifier);
    std::string readStringUntil(const char *str);
    void writeString(const std::string &str);
    void writeString(const char *str);
    bool isDCDEnabled() const;
    bool isCTSEnabled() const;
    bool isDSREnabled() const;
    void enableDTR();
    void disableDTR();
    void enableRTS();
    void disableRTS();
    void flushRX();
    void flushTX();
    void flushRXTX();

    void setPortName(const std::string &name);
    void setBaudRate(BaudRate baudRate);
    void setStopBits(StopBits stopBits);
    void setParity(Parity parity);
    void setDataBits(DataBits dataBits);
    void setTimeout(long long int timeout);

    std::string portName() const;
    int portNumber() const;
    BaudRate baudRate() const;
    StopBits stopBits() const;
    DataBits dataBits() const;
    Parity parity() const;
    long long int timeout() const;
    bool isOpen() const;

    std::string baudRateToString() const;
    std::string stopBitsToString() const;
    std::string dataBitsToString() const;
    std::string parityToString() const;
    static std::string baudRateToString(BaudRate baudRate);
    static std::string stopBitsToString(StopBits stopBits);
    static std::string dataBitsToString(DataBits dataBits);
    static std::string parityToString(Parity parity);

    static const std::vector<std::string> SERIAL_PORT_NAMES;

    static BaudRate parseBaudRateFromRaw(const std::string &baudRate);
    static DataBits parseDataBitsFromRaw(const std::string &dataBits);
    static StopBits parseStopBitsFromRaw(const std::string &stopBits);
    static Parity parseParityFromRaw(const std::string &parity);

    static BaudRate parseBaudRateFromRaw(const char *baudRate);
    static DataBits parseDataBitsFromRaw(const char *dataBits);
    static StopBits parseStopBitsFromRaw(const char *stopBits);
    static Parity parseParityFromRaw(const char *parity);

    static const DataBits DEFAULT_DATA_BITS;
    static const StopBits DEFAULT_STOP_BITS;
    static const Parity DEFAULT_PARITY;
    static const BaudRate DEFAULT_BAUD_RATE;

    static const std::string DEFAULT_DATA_BITS_STRING;
    static const std::string DEFAULT_STOP_BITS_STRING;
    static const std::string DEFAULT_PARITY_STRING;
    static const std::string DEFAULT_BAUD_RATE_STRING;

    static std::vector<std::string> availableSerialPorts();
    static std::vector<const char *> availableBaudRates();
    static std::vector<const char *> availableStopBits();
    static std::vector<const char *> availableDataBits();
    static std::vector<const char *> availableParity();

    static const int DEFAULT_TIMEOUT;

private:
    #if (defined(_WIN32) || defined(__CYGWIN__))
        static const std::string DTR_RTS_ON_IDENTIFIER;
        static const int constexpr NUMBER_OF_POSSIBLE_SERIAL_PORTS{16};
        HANDLE m_serialPort[NUMBER_OF_POSSIBLE_SERIAL_PORTS];
    #else
        static const int constexpr NUMBER_OF_POSSIBLE_SERIAL_PORTS{38};
        int m_serialPort[NUMBER_OF_POSSIBLE_SERIAL_PORTS];
        struct termios m_oldPortSettings[NUMBER_OF_POSSIBLE_SERIAL_PORTS];
        struct termios m_newPortSettings;
    #endif
    std::string m_portName;
    int m_portNumber;
    BaudRate m_baudRate;
    StopBits m_stopBits;
    DataBits m_dataBits;
    Parity m_parity;
    long long int m_timeout;
    bool m_isOpen;

    static const int constexpr SERIAL_PORT_BUF_MAX{4025};
    static bool isAvailableSerialPort(const std::string &name);
    static std::pair<int, std::string> getPortNameAndNumber(const std::string &name);

    void writeCString(const char *str);
    int writeByte(unsigned char byteToSend);
    int writeBufferedBytes(unsigned char *buffer, int bufferSize);

    static int parseDataBits(DataBits dataBits);
    static int parseStopBits(StopBits stopBits);
    static int parseBaudRate(BaudRate baudRate);
    static std::pair<int, int> parseParity(Parity parity);

    static const std::vector<const char *> s_AVAILABLE_PARITY;
    static const std::vector<const char *> s_AVAILABLE_STOP_BITS;
    static const std::vector<const char *> s_AVAILABLE_DATA_BITS;
    static const std::vector<const char *> s_AVAILABLE_BAUD_RATE;

};

#endif //TJLUTILS_SERIALPORT_H
