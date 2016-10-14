/***********************************************************************
*    serialport.cpp:                                                   *
*    SerialPort class, for connecting to an RS232 serial port          *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a SerialPort class          *
*    It is used to connect to RS232 serialpliant serial ports             *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "serialport.h"


#if (defined(_WIN32) || defined(__CYGWIN__))
    const std::string SerialPort::DTR_RTS_ON_IDENTIFIER{"dtr=on rts=on"};
    const std::vector<std::string> SerialPort::SERIAL_PORT_NAMES{"\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
                                                                 "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                                                                 "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                                                                 "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16"};
#else
    const std::vector<std::string> SerialPort::SERIAL_PORT_NAMES{"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4","/dev/ttyS5",
                                                                "/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9","/dev/ttyS10","/dev/ttyS11",
                                                                "/dev/ttyS12","/dev/ttyS13","/dev/ttyS14","/dev/ttyS15","/dev/ttyUSB0",
                                                                "/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4","/dev/ttyUSB5",
                                                                "/dev/ttyAMA0","/dev/ttyAMA1","/dev/ttyACM0","/dev/ttyACM1", "/dev/ttyACM2",
                                                                "/dev/ttyACM3", "/dev/ttyACM4", "/dev/ttyACM5", "/dev/ttyACM6", "/dev/ttyACM7",
                                                                "/dev/rfserialm0","/dev/rfserialm1","/dev/irserialm0","/dev/irserialm1",
                                                                "/dev/cuau0","/dev/cuau1","/dev/cuau2","/dev/cuau3",
                                                                "/dev/cuaU0","/dev/cuaU1","/dev/cuaU2","/dev/cuaU3"};
#endif

const DataBits SerialPort::DEFAULT_DATA_BITS{DataBits::EIGHT};
const StopBits SerialPort::DEFAULT_STOP_BITS{StopBits::ONE};
const Parity SerialPort::DEFAULT_PARITY{Parity::NONE};
const BaudRate SerialPort::DEFAULT_BAUD_RATE{BaudRate::BAUD115200};
const int SerialPort::DEFAULT_TIMEOUT{20};
const std::string SerialPort::DEFAULT_DATA_BITS_STRING{"8"};
const std::string SerialPort::DEFAULT_STOP_BITS_STRING{"1"};
const std::string SerialPort::DEFAULT_PARITY_STRING{"None"};
const std::string SerialPort::DEFAULT_BAUD_RATE_STRING{"115200"};
const std::vector<const char *> SerialPort::s_AVAILABLE_PARITY{"None", "Even", "Odd"};
const std::vector<const char *> SerialPort::s_AVAILABLE_STOP_BITS{"2", "1"};
const std::vector<const char *> SerialPort::s_AVAILABLE_DATA_BITS{"8", "7", "6", "5"};

#if defined(_WIN32) || defined(__CYGWIN__)
    const std::vector<const char *> SerialPort::s_AVAILABLE_BAUD_RATE{"110", "300", "600", "1200", "2400", "4800", 
                                                                    "9600", "19200", "38400", "57600", "115200", 
                                                                    "128000", "256000", "500000", "1000000"};
#else
    const std::vector<const char *> SerialPort::s_AVAILABLE_BAUD_RATE{"50", "75", "110", "134", "150", "200", "300",
                                                                    "600", "1200", "1800", "2400", "4800", "9600", "19200",
                                                                    "38400", "57600", "115200", "230400", "460800", "500000",
                                                                    "576000", "921600", "1000000", "1152000", "1500000",
                                                                    "2000000", "2500000", "3000000", "3500000", "4000000"};
#endif

SerialPort::SerialPort(const std::string &name) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{DEFAULT_BAUD_RATE},
    m_stopBits{DEFAULT_STOP_BITS},
    m_dataBits{DEFAULT_DATA_BITS},
    m_parity{DEFAULT_PARITY},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate) :    
   m_portName{name},
    m_portNumber{0},
    m_baudRate{baudRate},
    m_stopBits{DEFAULT_STOP_BITS},
    m_dataBits{DEFAULT_DATA_BITS},
    m_parity{DEFAULT_PARITY},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{baudRate},
    m_stopBits{DEFAULT_STOP_BITS},
    m_dataBits{dataBits},
    m_parity{DEFAULT_PARITY},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{baudRate},
    m_stopBits{stopBits},
    m_dataBits{DEFAULT_DATA_BITS},
    m_parity{DEFAULT_PARITY},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, Parity parity) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{baudRate},
    m_stopBits{DEFAULT_STOP_BITS},
    m_dataBits{dataBits},
    m_parity{parity},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, Parity parity) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{baudRate},
    m_stopBits{stopBits},
    m_dataBits{DEFAULT_DATA_BITS},
    m_parity{parity},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, StopBits stopBits, Parity parity) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{baudRate},
    m_stopBits{stopBits},
    m_dataBits{dataBits},
    m_parity{parity},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, DataBits dataBits) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{DEFAULT_BAUD_RATE},
    m_stopBits{DEFAULT_STOP_BITS},
    m_dataBits{dataBits},
    m_parity{DEFAULT_PARITY},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{DEFAULT_BAUD_RATE},
    m_stopBits{stopBits},
    m_dataBits{dataBits},
    m_parity{DEFAULT_PARITY},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits, Parity parity) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{DEFAULT_BAUD_RATE},
    m_stopBits{stopBits},
    m_dataBits{dataBits},
    m_parity{parity},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, DataBits dataBits, Parity parity) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{DEFAULT_BAUD_RATE},
    m_stopBits{DEFAULT_STOP_BITS},
    m_dataBits{dataBits},
    m_parity{parity},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, StopBits stopBits) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{DEFAULT_BAUD_RATE},
    m_stopBits{stopBits},
    m_dataBits{DEFAULT_DATA_BITS},
    m_parity{DEFAULT_PARITY},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, StopBits stopBits, Parity parity) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{DEFAULT_BAUD_RATE},
    m_stopBits{stopBits},
    m_dataBits{DEFAULT_DATA_BITS},
    m_parity{parity},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(const std::string &name, Parity parity) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{DEFAULT_BAUD_RATE},
    m_stopBits{DEFAULT_STOP_BITS},
    m_dataBits{DEFAULT_DATA_BITS},
    m_parity{parity},
    m_timeout{DEFAULT_TIMEOUT},
    m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

SerialPort::SerialPort(SerialPort &&other) :
    m_portName{std::move(other.m_portName)},
    m_portNumber{std::move(other.m_portNumber)},
    m_baudRate{std::move(other.m_baudRate)},
    m_stopBits{std::move(other.m_stopBits)},
    m_dataBits{std::move(other.m_dataBits)},
    m_parity{std::move(other.m_parity)},
    m_timeout{std::move(other.m_timeout)},
    m_isOpen{std::move(other.m_isOpen)}
{

}

bool operator==(const SerialPort &lhs, const SerialPort &rhs)
{
    return (lhs.portName() == rhs.portName());
}

void SerialPort::openPort()
{

#if (defined(_WIN32) || defined(__CYGWIN__))

    if (!isAvailableSerialPort(this->m_portName)) {
        throw std::runtime_error("ERROR: " + this->m_portName + " is not a currently available serial port (is something else using it?)");
    }
    std::string mode{""};
    mode += "baud=" + std::to_string(parseBaudRate(this->m_baudRate)) + " ";
    mode += "data=" + std::to_string(parseDataBits(this->m_dataBits)) + " ";
    mode += "parity=" + std::string(1, static_cast<char>(parseParity(this->m_parity).first)) + " ";
    mode += "stop=" + std::to_string(parseStopBits(this->m_stopBits)) + " ";
    mode += DTR_RTS_ON_IDENTIFIER;

    this->m_serialPort[this->m_portNumber] = CreateFileA(this->m_portName.c_str(),
                                                      GENERIC_READ|GENERIC_WRITE,
                                                      0,                          /* no share  */
                                                      NULL,                       /* no security */
                                                      OPEN_EXISTING,
                                                      0,                          /* no threads */
                                                      NULL);                      /* no templates */

    if(this->m_serialPort[this->m_portNumber] == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("ERROR: Unable to open serial port " + this->m_portName);
    }

    DCB portSettings;
    memset(&portSettings, 0, sizeof(portSettings));
    portSettings.DCBlength = sizeof(portSettings);

    if(!BuildCommDCBA(mode.c_str(), &portSettings)) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set dcb settings for serial port " + this->m_portName);
    }

    if(!SetCommState(this->m_serialPort[this->m_portNumber], &portSettings)) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set cfg settings for serial port " + this->m_portName);
    }

    COMMTIMEOUTS Cptimeouts;
    Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
    Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
    Cptimeouts.ReadTotalTimeoutConstant    = 0;
    Cptimeouts.WriteTotalTimeoutMultiplier = 0;
    Cptimeouts.WriteTotalTimeoutConstant   = 0;

    if(!SetCommTimeouts(this->m_serialPort[this->m_portNumber], &Cptimeouts)) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set timeout settings for serial port " + this->m_portName);
    }
    this->m_isOpen = true;
#else

    if (!isAvailableSerialPort(this->m_portName)) {
        throw std::runtime_error("ERROR: " + this->m_portName + " is not a currently available serial port (is something else using it?)");
    }
    int status{0};
    int error{0};
    int baudRate{parseBaudRate(this->m_baudRate)};
    int cbits{parseDataBits(this->m_dataBits)};
    std::pair<int, int> parityPair{parseParity(this->m_parity)};
    int cpar{parityPair.first};
    int ipar{parityPair.second};
    int bstop{parseStopBits(this->m_stopBits)};
    this->m_serialPort[this->m_portNumber] = open(this->m_portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(this->m_serialPort[this->m_portNumber] == -1) {
        throw std::runtime_error("ERROR: Unable to open serial port " + this->m_portName);
    }

    if(flock(this->m_serialPort[this->m_portNumber], LOCK_EX | LOCK_NB) != 0) {
        this->closePort();
        throw std::runtime_error("ERROR: Another process has locked serial port " + this->m_portName);
    }

    error = tcgetattr(this->m_serialPort[this->m_portNumber], this->m_oldPortSettings + this->m_portNumber);
    if (error == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to read port settings for serial port " + this->m_portName);
    }
    memset(&this->m_newPortSettings, 0, sizeof(this->m_newPortSettings)); 

    this->m_newPortSettings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
    this->m_newPortSettings.c_iflag = ipar;
    this->m_newPortSettings.c_oflag = 0;
    this->m_newPortSettings.c_lflag = 0;
    this->m_newPortSettings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
    this->m_newPortSettings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */

    cfsetispeed(&this->m_newPortSettings, baudRate);
    cfsetospeed(&this->m_newPortSettings, baudRate);

    error = tcsetattr(this->m_serialPort[this->m_portNumber], TCSANOW, &this->m_newPortSettings);
    if(error == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to adjust port settings for serial port " + this->m_portName);
    }

    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to get port status for serial port " + this->m_portName);
    }

    status |= TIOCM_DTR;    /* turn on DTR */
    status |= TIOCM_RTS;    /* turn on RTS */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set port status for serial port " + this->m_portName);
    }
    this->m_isOpen = true;
#endif
}


int SerialPort::parseBaudRate(BaudRate baudRate)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (baudRate == BaudRate::BAUD110) {
        return 110;
    } else if (baudRate == BaudRate::BAUD300) {
        return 300;
    } else if (baudRate == BaudRate::BAUD600) {
        return 600;
    } else if (baudRate == BaudRate::BAUD1200) {
        return 1200;
    } else if (baudRate == BaudRate::BAUD2400) {
        return 2400;
    } else if (baudRate == BaudRate::BAUD4800) {
        return 4800;
    } else if (baudRate == BaudRate::BAUD9600) {
        return 9600;
    } else if (baudRate == BaudRate::BAUD19200) {
        return 19200;
    } else if (baudRate == BaudRate::BAUD38400) {
        return 38400;
    } else if (baudRate == BaudRate::BAUD57600) {
        return 57600;
    } else if (baudRate == BaudRate::BAUD115200) {
        return 115200;
    } else if (baudRate == BaudRate::BAUD128000) {
        return 128000;
    } else if (baudRate == BaudRate::BAUD256000) {
        return 256000;
    } else if (baudRate == BaudRate::BAUD500000) {
        return 500000;
    } else if (baudRate == BaudRate::BAUD1000000) {
        return 1000000;
    } else {
        return parseBaudRate(DEFAULT_BAUD_RATE);
    }
    return parseBaudRate(DEFAULT_BAUD_RATE);
#else
    if (baudRate == BaudRate::BAUD50) {
        return B50;
    } else if (baudRate == BaudRate::BAUD75) {
        return B75;
    } else if (baudRate == BaudRate::BAUD110) {
        return B110;
    } else if (baudRate == BaudRate::BAUD134) {
        return B134;
    } else if (baudRate == BaudRate::BAUD150) {
        return B150; 
    } else if (baudRate == BaudRate::BAUD200) {
        return B200;
    } else if (baudRate == BaudRate::BAUD300) {
        return B300;
    } else if (baudRate == BaudRate::BAUD600) {
        return B600;
    } else if (baudRate == BaudRate::BAUD1200) {
        return B1200; 
    } else if (baudRate == BaudRate::BAUD1800) {
        return B1800;
    } else if (baudRate == BaudRate::BAUD2400) {
        return B2400;
    } else if (baudRate == BaudRate::BAUD4800) {
        return B4800;
    } else if (baudRate == BaudRate::BAUD9600) {
        return B9600; 
    } else if (baudRate == BaudRate::BAUD19200) {
        return B19200;
    } else if (baudRate == BaudRate::BAUD38400) {
        return B38400;
    } else if (baudRate == BaudRate::BAUD57600) {
        return B57600;
    } else if (baudRate == BaudRate::BAUD115200) {
        return B115200; 
    } else if (baudRate == BaudRate::BAUD230400) {
        return B230400;
    } else if (baudRate == BaudRate::BAUD460800) {
        return B460800;
    } else if (baudRate == BaudRate::BAUD500000) {
        return B500000;
    } else if (baudRate == BaudRate::BAUD576000) {
        return B576000; 
    } else if (baudRate == BaudRate::BAUD921600) {
        return B921600;
    } else if (baudRate == BaudRate::BAUD1000000) {
        return B1000000;
    } else if (baudRate == BaudRate::BAUD1152000) {
        return B1152000;
    } else if (baudRate == BaudRate::BAUD1500000) {
        return B1500000; 
    } else if (baudRate == BaudRate::BAUD2000000) {
        return B2000000;
    } else if (baudRate == BaudRate::BAUD2500000) {
        return B2500000;
    } else if (baudRate == BaudRate::BAUD3000000) {
        return B3000000;
    } else if (baudRate == BaudRate::BAUD3500000) {
        return B3500000; 
    } else if (baudRate == BaudRate::BAUD4000000) {
        return B4000000;
    } else {
        return parseBaudRate(DEFAULT_BAUD_RATE);
    }
    return parseBaudRate(DEFAULT_BAUD_RATE);
#endif
}

int SerialPort::parseDataBits(DataBits dataBits)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (dataBits == DataBits::EIGHT) {
        return 8;
    } else if (dataBits == DataBits::SEVEN) {
        return 7;
    } else if (dataBits == DataBits::SIX) {
        return 6;
    } else if (dataBits == DataBits::FIVE) {
        return 5;
    } else {
        return parseDataBits(DEFAULT_DATA_BITS);
    }
    return parseDataBits(DEFAULT_DATA_BITS);
#else
    if (dataBits == DataBits::EIGHT) {
        return CS8;
    } else if (dataBits == DataBits::SEVEN) {
        return CS7;
    } else if (dataBits == DataBits::SIX) {
        return CS6;
    } else if (dataBits == DataBits::FIVE) {
        return CS5;
    } else {
        return parseDataBits(DEFAULT_DATA_BITS);
    }
    return parseDataBits(DEFAULT_DATA_BITS);
#endif
} 

int SerialPort::parseStopBits(StopBits stopBits)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (stopBits == StopBits::ONE) {
        return 1;
    } else if (stopBits == StopBits::TWO) {
        return 2;
    } else {
        return parseStopBits(DEFAULT_STOP_BITS);
    }
    return parseStopBits(DEFAULT_STOP_BITS);
#else
    if (stopBits == StopBits::ONE) {
        return 0;
    } else if (stopBits == StopBits::TWO) {
        return CSTOPB;
    } else {
        return parseStopBits(DEFAULT_STOP_BITS);
    }
    return parseStopBits(DEFAULT_STOP_BITS);
#endif
}

std::pair<int, int> SerialPort::parseParity(Parity parity)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (parity == Parity::NONE) {
        return std::make_pair('n', 0);
    } else if (parity == Parity::EVEN) {
        return std::make_pair('e', 0);
    } else if (parity == Parity::ODD) {
        return std::make_pair('o', 0);
    } else {
        return parseParity(DEFAULT_PARITY);
    }
    return parseParity(DEFAULT_PARITY);
#else
    if (parity == Parity::NONE) {
        return std::make_pair(0, IGNPAR);
    } else if (parity == Parity::EVEN) {
        return std::make_pair(PARENB, INPCK);
    } else if (parity == Parity::ODD) {
        return std::make_pair( (PARENB | PARODD), INPCK);
    } else {
        return parseParity(DEFAULT_PARITY);
    }
    return parseParity(DEFAULT_PARITY);
#endif
}

std::string SerialPort::readString()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    /* added the void pointer cast, otherwise gcc will serialplain about */
    /* "warning: dereferencing type-punned pointer will break strict aliasing rules" */
    unsigned char *buffer = static_cast<unsigned char *>(malloc(SERIAL_PORT_BUF_MAX));
    long int returnedBytes{0};
    ReadFile(this->m_serialPort[this->m_portNumber], buffer, SERIAL_PORT_BUF_MAX, (LPDWORD)((void *)&returnedBytes), NULL);
    return static_cast<std::string>(reinterpret_cast<const char *>(buffer));
#else
    unsigned char *buffer = static_cast<unsigned char *>(malloc(SERIAL_PORT_BUF_MAX));
    long int returnedBytes{read(this->m_serialPort[this->m_portNumber], buffer, SERIAL_PORT_BUF_MAX)};
    if(returnedBytes < 0) {
        if(errno == EAGAIN)  {
            return "";
        }
    }
    return static_cast<std::string>(reinterpret_cast<const char *>(buffer));
#endif
}

std::string SerialPort::readStringUntil(const std::string &str)
{
    std::string returnString{""};
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
    long long int elapsedTime{0};
    do {
        returnString += readString();
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
    } while ((elapsedTime < this->m_timeout) && (returnString.find(str) == std::string::npos));
    return returnString.substr(0, returnString.find(str)+1);
}

int SerialPort::writeByte(unsigned char byteToSend)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    long int writtenBytes{0};
    WriteFile(this->m_serialPort[this->m_portNumber], &byteToSend, 1, (LPDWORD)((void *)&writtenBytes), NULL);
    return ( (writtenBytes < 0) ? 1 : 0);
#else
    long int writtenBytes{write(this->m_serialPort[this->m_portNumber], &byteToSend, 1)};
    if(writtenBytes < 0) {
        return (errno == EAGAIN ? 0 : 1);
    }
    return writtenBytes;
#endif
}

int SerialPort::writeBufferedBytes(unsigned char *buffer, int bufferSize)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    long int writtenBytes;
    return (WriteFile(this->m_serialPort[this->m_portNumber], buffer, bufferSize, (LPDWORD)((void *)&writtenBytes), NULL) ? writtenBytes : -1);
#else
    long int writtenBytes{write(this->m_serialPort[this->m_portNumber], buffer, bufferSize)};
    if(writtenBytes < 0) {
        return (errno == EAGAIN ? 0 : 1);
    }
    return writtenBytes;
#endif
}


void SerialPort::closePort()
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        CloseHandle(this->m_serialPort[this->m_portNumber]);
        this->m_isOpen = false;
    #else
        int status{0};
        if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
            std::cout << "WARNING: Unable to get port status while closing serial port " << this->m_portName << std::endl;
        }
        status &= ~TIOCM_DTR;    /* turn off DTR */
        status &= ~TIOCM_RTS;    /* turn off RTS */
        if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
            std::cout << "WARNING: Unable to get port status while closing serial port " << this->m_portName << std::endl;
        }

        tcsetattr(this->m_serialPort[this->m_portNumber], TCSANOW, this->m_oldPortSettings + this->m_portNumber);
        close(this->m_serialPort[this->m_portNumber]);
        flock(this->m_serialPort[this->m_portNumber], LOCK_UN);
        this->m_isOpen = false;
    #endif
}


void SerialPort::enableDTR()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPort[this->m_portNumber], SETDTR);
#else
    int status{0};
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
        std::cout << "WARNING: Unable to get port status while enabling DTR for serial port " << this->m_portName << std::endl;
    }
    status |= TIOCM_DTR;    /* turn on DTR */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while enabling DTR for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::disableDTR()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPort[this->m_portNumber], CLRDTR);
#else
    int status{0};
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
        std::cout << "WARNING: Unable to get port status while disabling DTR for serial port " << this->m_portName << std::endl;
    }
    status &= ~TIOCM_DTR;    /* turn off DTR */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while disabling DTR for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::enableRTS()
{

#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPort[this->m_portNumber], SETRTS);
#else
    int status{0};
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
          std::cout << "WARNING: Unable to get port status while enabling RTS for serial port " << this->m_portName << std::endl;
    }
    status |= TIOCM_RTS;    /* turn on RTS */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while enabling RTS for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::disableRTS()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPort[this->m_portNumber], CLRRTS);
#else
    int status;
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
        std::cout << "WARNING: Unable to get port status while disabling RTS for serial port " << this->m_portName << std::endl;
    }
    status &= ~TIOCM_RTS;    /* turn off RTS */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while disabling RTS for serial port " << this->m_portName << std::endl;
    }
#endif
}

bool SerialPort::isDCDEnabled() const
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        int status{0};
        GetCommModemStatus(this->m_serialPort[this->m_portNumber], (LPDWORD)((void *)&status));
        return ((status&MS_RLSD_ON) != 0 ? true : false);
    #else
        int status{0};
        ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status);
        return (status&TIOCM_CAR);
    #endif
}


bool SerialPort::isCTSEnabled() const
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        int status{0};
        GetCommModemStatus(this->m_serialPort[this->m_portNumber], (LPDWORD)((void *)&status));
        return ((status&MS_CTS_ON) != 0 ? true : false);
    #else
        int status{0};
        ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status);
        return (status&TIOCM_CTS);
    #endif
}

bool SerialPort::isDSREnabled() const
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        int status{0};
        GetCommModemStatus(this->m_serialPort[this->m_portNumber], (LPDWORD)((void *)&status));
        return ((status&MS_DSR_ON) != 0 ? true : false);
    #else
        int status{0};
        ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status);
        return (status&TIOCM_DSR);
    #endif
}

void SerialPort::flushRX()
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        PurgeComm(this->m_serialPort[this->m_portNumber], PURGE_RXCLEAR | PURGE_RXABORT);
    #else
        tcflush(this->m_serialPort[this->m_portNumber], TCIFLUSH);
    #endif
}


void SerialPort::flushTX()
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        PurgeComm(this->m_serialPort[this->m_portNumber], PURGE_TXCLEAR | PURGE_TXABORT);
    #else
        tcflush(this->m_serialPort[this->m_portNumber], TCOFLUSH);
    #endif
}


void SerialPort::flushRXTX()
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        PurgeComm(this->m_serialPort[this->m_portNumber], PURGE_RXCLEAR | PURGE_RXABORT);
        PurgeComm(this->m_serialPort[this->m_portNumber], PURGE_TXCLEAR | PURGE_TXABORT);
    #else
        tcflush(this->m_serialPort[this->m_portNumber], TCIOFLUSH);
    #endif
}

bool SerialPort::isAvailableSerialPort(const std::string &name)
{
    //TODO: Implement
    (void)name;
    return true;
}

std::pair<int, std::string> SerialPort::getPortNameAndNumber(const std::string &name)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    std::string str{name};
    int i{0};
    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
        if (it.find(str) != std::string::npos) {
            return std::make_pair(i, str);
        }
        i++;
    }
    throw std::runtime_error("ERROR: " + name + " is an invalid serial port name");
#else
    std::string str{name};
    if (str.find("/dev/") == std::string::npos) {
        if (str.find("tty") == std::string::npos) {
            str = "/dev/tty" + str;
        } else {
            str = "/dev/" + str;
        }
    }
    int i{0};
    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
        if (str == it) {
            return std::make_pair(i, str);
        }
        i++;
    }
    throw std::runtime_error("ERROR: " + name + " is an invalid serial port name");
#endif
}

bool SerialPort::isOpen() const
{
    return this->m_isOpen;
}

void SerialPort::writeCString(const char *str)
{
    while(*str != 0) {
        writeByte(*(str++));
    }
}

void SerialPort::writeString(const std::string &str)
{
    return writeCString(str.c_str());
}

void SerialPort::writeString(const char *str)
{
    return writeCString(str);
}

std::string SerialPort::readStringUntil(char identifier)
{
    return readStringUntil(std::string(1, identifier));
}

std::string SerialPort::readStringUntil(const char *str)
{
    return readStringUntil(static_cast<std::string>(str));
}


void SerialPort::setPortName(const std::string &name)
{
    if (!this->m_isOpen) {
        std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(name)};
        this->m_portNumber = truePortNameAndNumber.first;
        this->m_portName = truePortNameAndNumber.second;
    } else {
        throw std::runtime_error("ERROR: Cannot change port handle(name) while serial port " + this->m_portName + " is open");
    }
}

void SerialPort::setDataBits(DataBits dataBits)
{
    if (!this->m_isOpen) {
        this->m_dataBits = dataBits;
    } else {
        throw std::runtime_error("ERROR: Cannot change data bits while serial port " + this->m_portName + " is open");
    }
}

void SerialPort::setBaudRate(BaudRate baudRate)
{
    if (!this->m_isOpen) {
        this->m_baudRate = baudRate;
    } else {
        throw std::runtime_error("ERROR: Cannot change baud rate while serial port " + this->m_portName + " is open");
    }
}

void SerialPort::setStopBits(StopBits stopBits)
{
    if (!this->m_isOpen) {
        this->m_stopBits = stopBits;
    } else {
        throw std::runtime_error("ERROR: Cannot change stop bits while serial port " + this->m_portName + " is open");
    }
}

void SerialPort::setParity(Parity parity)
{
    if (!this->m_isOpen) {
        this->m_parity = parity;
    } else {
        throw std::runtime_error("ERROR: Cannot change parity while serial port " + this->m_portName + " is open");
    }
}

BaudRate SerialPort::baudRate() const
{
    return this->m_baudRate;
}

StopBits SerialPort::stopBits() const
{
    return this->m_stopBits;
}

DataBits SerialPort::dataBits() const
{
    return this->m_dataBits;
}

Parity SerialPort::parity() const
{
    return this->m_parity;
}

std::string SerialPort::portName() const
{
    return this->m_portName;
}

long long int SerialPort::timeout() const
{
    return this->m_timeout;
}

void SerialPort::setTimeout(long long int timeout) {
    if (timeout < 1) {
        throw std::runtime_error("ERROR: Serial timeout cannot be negative (" + std::to_string(timeout) + " < 0");
    } else {
        this->m_timeout = timeout;
    }
}

int SerialPort::portNumber() const
{
    return this->m_portNumber;
}

BaudRate SerialPort::parseBaudRateFromRaw(const std::string &baudRate)
{
    std::string copyString{baudRate};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (copyString == "110") {
        return BaudRate::BAUD110;
    } else if (copyString == "300") {
        return BaudRate::BAUD300;
    } else if (copyString == "600") {
        return BaudRate::BAUD600;
    } else if (copyString == "1200") {
        return BaudRate::BAUD1200;
    } else if (copyString == "2400") {
        return BaudRate::BAUD2400;
    } else if (copyString == "4800") {
        return BaudRate::BAUD4800;
    } else if (copyString == "9600") {
        return BaudRate::BAUD9600;
    } else if (copyString == "19200") {
        return BaudRate::BAUD19200;
    } else if (copyString == "38400") {
        return BaudRate::BAUD38400;
    } else if (copyString == "57600") {
        return BaudRate::BAUD57600;
    } else if (copyString == "115200") {
        return BaudRate::BAUD115200;
    } else if (copyString == "128000") {
        return BaudRate::BAUD128000;
    } else if (copyString == "256000") {
        return BaudRate::BAUD256000;
    } else if (copyString == "500000") {
        return BaudRate::BAUD500000;
    } else if (copyString == "1000000") {
        return BaudRate::BAUD1000000;
    } else {
        throw std::runtime_error("Invalid baud rate passed to parseBaudRateFromRaw(const std::string &): " + baudRate);
    }
#else
    if (copyString == "50") {
        return BaudRate::BAUD50;
    } else if (copyString == "75") {
        return BaudRate::BAUD75;
    } else if (copyString == "110") {
        return BaudRate::BAUD110;
    } else if (copyString == "134") {
        return BaudRate::BAUD134;
    } else if (copyString == "150") {
        return BaudRate::BAUD150; 
    } else if (copyString == "200") {
        return BaudRate::BAUD200;
    } else if (copyString == "300") {
        return BaudRate::BAUD300;
    } else if (copyString == "600") {
        return BaudRate::BAUD600;
    } else if (copyString == "1200") {
        return BaudRate::BAUD1200; 
    } else if (copyString == "1800") {
        return BaudRate::BAUD1800;
    } else if (copyString == "2400") {
        return BaudRate::BAUD2400;
    } else if (copyString == "4800") {
        return BaudRate::BAUD4800;
    } else if (copyString == "9600") {
        return BaudRate::BAUD9600; 
    } else if (copyString == "19200") {
        return BaudRate::BAUD19200;
    } else if (copyString == "38400") {
        return BaudRate::BAUD38400;
    } else if (copyString == "57600") {
        return BaudRate::BAUD57600;
    } else if (copyString == "115200") {
        return BaudRate::BAUD115200; 
    } else if (copyString == "230400") {
        return BaudRate::BAUD230400;
    } else if (copyString == "460800") {
        return BaudRate::BAUD460800;
    } else if (copyString == "500000") {
        return BaudRate::BAUD500000;
    } else if (copyString == "576000") {
        return BaudRate::BAUD576000; 
    } else if (copyString == "921600") {
        return BaudRate::BAUD921600;
    } else if (copyString == "1000000") {
        return BaudRate::BAUD1000000;
    } else if (copyString == "1152000") {
        return BaudRate::BAUD1152000;
    } else if (copyString == "1500000") {
        return BaudRate::BAUD1500000; 
    } else if (copyString == "2000000") {
        return BaudRate::BAUD2000000;
    } else if (copyString == "2500000") {
        return BaudRate::BAUD2500000;
    } else if (copyString == "3000000") {
        return BaudRate::BAUD3000000;
    } else if (copyString == "3500000") {
        return BaudRate::BAUD3500000; 
    } else if (copyString == "4000000") {
        return BaudRate::BAUD4000000;
    } else {
        throw std::runtime_error("Invalid baud rate passed to parseBaudRateFromRaw(const std::string &): " + baudRate);
    }
#endif
}

DataBits SerialPort::parseDataBitsFromRaw(const std::string &dataBits)
{
    std::string copyString{dataBits};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    if ((copyString == "eight") || (copyString == "8")) {
        return DataBits::EIGHT;
    } else if ((copyString == "seven") || (copyString == "7")) {
        return DataBits::SEVEN;
    } else if ((copyString == "six") || (copyString == "6")) {
        return DataBits::SIX;
    } else if ((copyString == "five") || (copyString == "5")) {
        return DataBits::FIVE;
    } else {
        throw std::runtime_error("Invalid data bits passed to parseDataBitsFromRaw(const std::string &): " + dataBits);
    }
} 

StopBits SerialPort::parseStopBitsFromRaw(const std::string &stopBits)
{
    std::string copyString{stopBits};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    if ((copyString == "one") || (copyString == "1")) {
        return StopBits::ONE;
    } else if ((copyString == "two" || copyString == "2")) {
        return StopBits::TWO;
    } else {
        throw std::runtime_error("Invalid stop bits passed to parseStopBitsFromRaw(const std::string &): " + stopBits);
    }
}

Parity SerialPort::parseParityFromRaw(const std::string &parity)
{
    std::string copyString{parity};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    if ((copyString == "none") || (copyString == "n")) {
        return Parity::NONE;
    } else if ((copyString == "even") || (copyString == "e")) {
        return Parity::EVEN;
    } else if ((copyString == "odd") || (copyString == "o")) {
        return Parity::ODD;
    } else {
        throw std::runtime_error("Invalid parity passed to parseParityFromRaw(const std::string &): " + parity);
    }
}

BaudRate SerialPort::parseBaudRateFromRaw(const char *baudRate) { return parseBaudRateFromRaw(static_cast<std::string>(baudRate)); }
DataBits SerialPort::parseDataBitsFromRaw(const char *dataBits) { return parseDataBitsFromRaw(static_cast<std::string>(dataBits)); }
StopBits SerialPort::parseStopBitsFromRaw(const char *stopBits) { return parseStopBitsFromRaw(static_cast<std::string>(stopBits)); }
Parity SerialPort::parseParityFromRaw(const char *parity) { return parseParityFromRaw(static_cast<std::string>(parity)); }

std::string SerialPort::baudRateToString(BaudRate baudRate)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (baudRate == BaudRate::BAUD110) {
        return "110";
    } else if (baudRate == BaudRate::BAUD300) {
        return "300";
    } else if (baudRate == BaudRate::BAUD600) {
        return "600";
    } else if (baudRate == BaudRate::BAUD1200) {
        return "1200";
    } else if (baudRate == BaudRate::BAUD2400) {
        return "2400";
    } else if (baudRate == BaudRate::BAUD4800) {
        return "4800";
    } else if (baudRate == BaudRate::BAUD9600) {
        return "9600";
    } else if (baudRate == BaudRate::BAUD19200) {
        return "19200";
    } else if (baudRate == BaudRate::BAUD38400) {
        return "38400";
    } else if (baudRate == BaudRate::BAUD57600) {
        return "57600";
    } else if (baudRate == BaudRate::BAUD115200) {
        return "115200";
    } else if (baudRate == BaudRate::BAUD128000) {
        return "128000";
    } else if (baudRate == BaudRate::BAUD256000) {
        return "256000";
    } else if (baudRate == BaudRate::BAUD500000) {
        return "500000";
    } else if (baudRate == BaudRate::BAUD1000000) {
        return "1000000";
    } else {
        return "";
    }
#else
    if (baudRate == BaudRate::BAUD50) {
        return "50";
    } else if (baudRate == BaudRate::BAUD75) {
        return "75";
    } else if (baudRate == BaudRate::BAUD110) {
        return "110";
    } else if (baudRate == BaudRate::BAUD134) {
        return "134";
    } else if (baudRate == BaudRate::BAUD150) {
        return "150"; 
    } else if (baudRate == BaudRate::BAUD200) {
        return "200";
    } else if (baudRate == BaudRate::BAUD300) {
        return "300";
    } else if (baudRate == BaudRate::BAUD600) {
        return "600";
    } else if (baudRate == BaudRate::BAUD1200) {
        return "1200"; 
    } else if (baudRate == BaudRate::BAUD1800) {
        return "1800";
    } else if (baudRate == BaudRate::BAUD2400) {
        return "2400";
    } else if (baudRate == BaudRate::BAUD4800) {
        return "4800";
    } else if (baudRate == BaudRate::BAUD9600) {
        return "9600"; 
    } else if (baudRate == BaudRate::BAUD19200) {
        return "19200";
    } else if (baudRate == BaudRate::BAUD38400) {
        return "38400";
    } else if (baudRate == BaudRate::BAUD57600) {
        return "57600";
    } else if (baudRate == BaudRate::BAUD115200) {
        return "115200"; 
    } else if (baudRate == BaudRate::BAUD230400) {
        return "230400";
    } else if (baudRate == BaudRate::BAUD460800) {
        return "460800";
    } else if (baudRate == BaudRate::BAUD500000) {
        return "500000";
    } else if (baudRate == BaudRate::BAUD576000) {
        return "576000"; 
    } else if (baudRate == BaudRate::BAUD921600) {
        return "921600";
    } else if (baudRate == BaudRate::BAUD1000000) {
        return "1000000";
    } else if (baudRate == BaudRate::BAUD1152000) {
        return "1152000";
    } else if (baudRate == BaudRate::BAUD1500000) {
        return "1500000"; 
    } else if (baudRate == BaudRate::BAUD2000000) {
        return "2000000";
    } else if (baudRate == BaudRate::BAUD2500000) {
        return "2500000";
    } else if (baudRate == BaudRate::BAUD3000000) {
        return "3000000";
    } else if (baudRate == BaudRate::BAUD3500000) {
        return "3500000"; 
    } else if (baudRate == BaudRate::BAUD4000000) {
        return "4000000";
    } else {
        return "";
    }
#endif
}
std::string SerialPort::stopBitsToString(StopBits stopBits)
{
    if (stopBits == StopBits::ONE) {
        return "1";
    } else if (stopBits == StopBits::TWO) {
        return "2";
    } else {
        return "";
    }
}

std::string SerialPort::dataBitsToString(DataBits dataBits)
{
    if (dataBits == DataBits::EIGHT) {
        return "8";
    } else if (dataBits == DataBits::SEVEN) {
        return "7";
    } else if (dataBits == DataBits::SIX) {
        return "6";
    } else if (dataBits == DataBits::FIVE) {
        return "5";
    } else {
        return "";
    }
}

std::string SerialPort::parityToString(Parity parity)
{
    if (parity == Parity::EVEN) {
        return "Even";
    } else if (parity == Parity::ODD) {
        return "Odd";
    } else if (parity == Parity::NONE) {
        return "None";
    } else {
        return "";
    }
}

std::string SerialPort::baudRateToString() const
{
    return SerialPort::baudRateToString(this->m_baudRate);
}

std::string SerialPort::stopBitsToString() const
{
    return SerialPort::stopBitsToString(this->m_stopBits);
}

std::string SerialPort::dataBitsToString() const
{
    return SerialPort::dataBitsToString(this->m_dataBits);
}

std::string SerialPort::parityToString() const
{
    return SerialPort::parityToString(this->m_parity);
}

std::vector<const char *> SerialPort::availableBaudRates()
{      
    return SerialPort::s_AVAILABLE_BAUD_RATE;
}

std::vector<const char *> SerialPort::availableParity()
{
    return SerialPort::s_AVAILABLE_PARITY;
}

std::vector<const char *> SerialPort::availableStopBits()
{
    return SerialPort::s_AVAILABLE_STOP_BITS;
}

std::vector<const char *> SerialPort::availableDataBits()
{
    return SerialPort::s_AVAILABLE_DATA_BITS;
}

std::vector<std::string> SerialPort::availableSerialPorts()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    static std::vector<std::string> portNames{"COM1", "COM2", "COM3", "COM4",
                                              "COM5", "COM6", "COM7", "COM8",
                                              "COM9", "COM10", "COM11", "COM12",
                                              "COM13", "COM14", "COM15", "COM16"};
    std::vector<std::string> returnVector;
    std::unique_ptr<SystemCommand> systemCommand{std::make_unique<SystemCommand>()};
    systemCommand->setCommand("wmic path Win32_SerialPort get /format:list");
    std::vector<std::string> returnString{systemCommand->executeAndWaitForOutputAsVector()};
    if (returnString.size() == 1) {
        return returnVector;
    }

    for (auto &it : returnString) {
        for (auto &innerIt : portNames) {
            if (it.find(innerIt) != std::string::npos) {
                returnVector.emplace_back(innerIt);
            }
        }
    }
    std::set<std::string> uniques;
    for (auto &it : returnVector) {
        uniques.emplace(it);
    }
    std::vector<std::string> realReturn;
    for (auto &it : uniques) {
        realReturn.emplace_back(it);
    }
    return realReturn;
#else
    static std::vector<std::string> portNames{"/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyACM2", "/dev/ttyACM3",
                                              "/dev/ttyACM4", "/dev/ttyACM5", "/dev/ttyACM6", "/dev/ttyACM7",
                                              "/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2", "/dev/ttyUSB3",
                                              "/dev/ttyUSB4", "/dev/ttyUSB5", "/dev/ttyUSB6", "/dev/ttyUSB7"};
    std::vector<std::string> returnVector;
    for (auto &it : portNames) {
        std::unique_ptr<SystemCommand> systemCommand{std::make_unique<SystemCommand>()};
        systemCommand->setCommand("ls " + it);
        systemCommand->execute();
        if (!systemCommand->hasError()) {
            returnVector.emplace_back(it);
        }
    }
    std::set<std::string> uniques;
    for (auto &it : returnVector) {
        uniques.emplace(it);
    }
    std::vector<std::string> realReturn;
    for (auto &it : uniques) {
        realReturn.emplace_back(it);
    }
    return realReturn;
#endif
}
