#include "arduino.h"

const std::vector<const char *> Arduino::VALID_DIGITAL_STATE_IDENTIFIERS{"1", "0", "HIGH", "high", "LOW", "low", "FALSE", "false", "TRUE", "true"};
const std::vector<char> Arduino::VALID_ANALOG_STATE_IDENTIFIERS{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};
const std::vector<const char *> Arduino::DIGITAL_STATE_HIGH_IDENTIFIERS{"1", "high", "true", "on"};
const std::vector<const char *> Arduino::DIGITAL_STATE_LOW_IDENTIFIERS{"0", "low", "false", "off"};
const char *Arduino::INVALID_HEADER{"{invalid"};
const char *Arduino::DIGITAL_READ_HEADER{"{dread"};
const char *Arduino::ANALOG_READ_HEADER{"{aread"};
const char *Arduino::DIGITAL_WRITE_HEADER{"{dwrite"};
const char *Arduino::ANALOG_WRITE_HEADER{"{awrite"};
const char *Arduino::SOFT_DIGITAL_READ_HEADER{"{sdread"};
const char *Arduino::SOFT_ANALOG_READ_HEADER{"{saread"};
const char *Arduino::PIN_TYPE_HEADER{"{ptype"};
const char *Arduino::PIN_TYPE_CHANGE_HEADER{"{ptchange"};
const char *Arduino::ARDUINO_TYPE_HEADER{"{ardtype"};
const char *Arduino::CAN_BUS_ENABLED_HEADER{"{canbus"};
const char *Arduino::FIRMWARE_VERSION_HEADER{"{version"};
const char *Arduino::HEARTBEAT_HEADER{"{heartbeat"};
const char *Arduino::IO_REPORT_HEADER{"{ioreport"};
const char *Arduino::IO_REPORT_END_HEADER{"{ioreportend"};
const char *Arduino::CHANGE_A_TO_D_THRESHOLD_HEADER{"{atodchange"};
const char *Arduino::CURRENT_A_TO_D_THRESHOLD_HEADER{"{atodthresh"};
const char *Arduino::ADD_POSITIVE_CAN_MASK_HEADER{"{addpcanmask"};
const char *Arduino::ADD_NEGATIVE_CAN_MASK_HEADER{"{rempcanmask"};
const char *Arduino::REMOVE_POSITIVE_CAN_MASK_HEADER{"{rempcanmask"};
const char *Arduino::REMOVE_NEGATIVE_CAN_MASK_HEADER{"{remncanmask"};
const char *Arduino::REMOVE_ALL_POSITIVE_CAN_MASKS_HEADER{"{remallpcanmasks"};
const char *Arduino::REMOVE_ALL_NEGATIVE_CAN_MASKS_HEADER{"{remallnanmasks"};
const char *Arduino::REMOVE_ALL_CAN_MASKS_HEADER{"{remallanmasks"};
const char *Arduino::UNO_A0_STRING{"A0"};
const char *Arduino::UNO_A1_STRING{"A1"};
const char *Arduino::UNO_A2_STRING{"A2"};
const char *Arduino::UNO_A3_STRING{"A3"};
const char *Arduino::UNO_A4_STRING{"A4"};
const char *Arduino::UNO_A5_STRING{"A5"};
const char *Arduino::UNO_A0_EQUIVALENT_STRING{"14"};
const char *Arduino::UNO_A1_EQUIVALENT_STRING{"15"};
const char *Arduino::UNO_A2_EQUIVALENT_STRING{"16"};
const char *Arduino::UNO_A3_EQUIVALENT_STRING{"17"};
const char *Arduino::UNO_A4_EQUIVALENT_STRING{"18"};
const char *Arduino::UNO_A5_EQUIVALENT_STRING{"19"};

const char *Arduino::NANO_A0_STRING{"A0"};
const char *Arduino::NANO_A1_STRING{"A1"};
const char *Arduino::NANO_A2_STRING{"A2"};
const char *Arduino::NANO_A3_STRING{"A3"};
const char *Arduino::NANO_A4_STRING{"A4"};
const char *Arduino::NANO_A5_STRING{"A5"};
const char *Arduino::NANO_A6_STRING{"A6"};
const char *Arduino::NANO_A7_STRING{"A7"};
const char *Arduino::NANO_A0_EQUIVALENT_STRING{"14"};
const char *Arduino::NANO_A1_EQUIVALENT_STRING{"15"};
const char *Arduino::NANO_A2_EQUIVALENT_STRING{"16"};
const char *Arduino::NANO_A3_EQUIVALENT_STRING{"17"};
const char *Arduino::NANO_A4_EQUIVALENT_STRING{"18"};
const char *Arduino::NANO_A5_EQUIVALENT_STRING{"19"};
const char *Arduino::NANO_A6_EQUIVALENT_STRING{"20"};
const char *Arduino::NANO_A7_EQUIVALENT_STRING{"21"};

const char *Arduino::MEGA_A0_STRING{"A0"};
const char *Arduino::MEGA_A1_STRING{"A1"};
const char *Arduino::MEGA_A2_STRING{"A2"};
const char *Arduino::MEGA_A3_STRING{"A3"};
const char *Arduino::MEGA_A4_STRING{"A4"};
const char *Arduino::MEGA_A5_STRING{"A5"};
const char *Arduino::MEGA_A6_STRING{"A6"};
const char *Arduino::MEGA_A7_STRING{"A7"};
const char *Arduino::MEGA_A8_STRING{"A8"};
const char *Arduino::MEGA_A9_STRING{"A9"};
const char *Arduino::MEGA_A10_STRING{"A10"};
const char *Arduino::MEGA_A11_STRING{"A11"};
const char *Arduino::MEGA_A12_STRING{"A12"};
const char *Arduino::MEGA_A13_STRING{"A13"};
const char *Arduino::MEGA_A14_STRING{"A14"};
const char *Arduino::MEGA_A15_STRING{"A15"};
const char *Arduino::MEGA_A0_EQUIVALENT_STRING{"54"};
const char *Arduino::MEGA_A1_EQUIVALENT_STRING{"55"};
const char *Arduino::MEGA_A2_EQUIVALENT_STRING{"56"};
const char *Arduino::MEGA_A3_EQUIVALENT_STRING{"57"};
const char *Arduino::MEGA_A4_EQUIVALENT_STRING{"58"};
const char *Arduino::MEGA_A5_EQUIVALENT_STRING{"59"};
const char *Arduino::MEGA_A6_EQUIVALENT_STRING{"60"};
const char *Arduino::MEGA_A7_EQUIVALENT_STRING{"61"};
const char *Arduino::MEGA_A8_EQUIVALENT_STRING{"62"};
const char *Arduino::MEGA_A9_EQUIVALENT_STRING{"63"};
const char *Arduino::MEGA_A10_EQUIVALENT_STRING{"64"};
const char *Arduino::MEGA_A11_EQUIVALENT_STRING{"65"};
const char *Arduino::MEGA_A12_EQUIVALENT_STRING{"66"};
const char *Arduino::MEGA_A13_EQUIVALENT_STRING{"67"};
const char *Arduino::MEGA_A14_EQUIVALENT_STRING{"68"};
const char *Arduino::MEGA_A15_EQUIVALENT_STRING{"69"};
const char *Arduino::DIGITAL_INPUT_IDENTIFIER{"din"};
const char *Arduino::DIGITAL_OUTPUT_IDENTIFIER{"dout"};
const char *Arduino::ANALOG_INPUT_IDENTIFIER{"ain"};
const char *Arduino::ANALOG_OUTPUT_IDENTIFIER{"aout"};
const char *Arduino::DIGITAL_INPUT_PULLUP_IDENTIFIER{"dinpup"};
const char *Arduino::OPERATION_FAILURE_STRING{"-1"};
const unsigned int Arduino::IO_STATE_RETURN_SIZE{3};
const unsigned int Arduino::ARDUINO_TYPE_RETURN_SIZE{2};
const unsigned int Arduino::PIN_TYPE_RETURN_SIZE{3};
const unsigned int Arduino::IO_REPORT_RETURN_SIZE{3};
const unsigned int Arduino::A_TO_D_THRESHOLD_RETURN_SIZE{2};
const unsigned int Arduino::REMOVE_CAN_MASKS_RETURN_SIZE{1};
const unsigned int Arduino::RETURN_SIZE_HIGH_LIMIT{1000};
const unsigned int Arduino::ADD_CAN_MASK_RETURN_SIZE{2};
const unsigned int Arduino::REMOVE_CAN_MASK_RETURN_SIZE{2};
const int Arduino::OPERATION_FAILURE{-1};
const int Arduino::STATE_FAILURE{-1};
const int Arduino::INVALID_PIN{-1};
const double Arduino::SERIAL_TIMEOUT{1000};
const int Arduino::BLUETOOTH_RETRY_COUNT{10};
const double Arduino::BOOTLOADER_BOOT_TIME{2000};
const double Arduino::BLUETOOTH_SERIAL_SEND_DELAY{100};
const int Arduino::IO_TRY_COUNT{3};
const int Arduino::ANALOG_MAX{1023};
const double Arduino::VOLTAGE_MAX{5.0};
const double Arduino::ANALOG_TO_VOLTAGE_SCALE_FACTOR{0.0049};
const int Arduino::CAN_BUS_PIN{9};
const unsigned int Arduino::RAW_CAN_MESSAGE_SIZE{9};
const unsigned char Arduino::CAN_MESSAGE_LENGTH{8};
const double Arduino::DEFAULT_BLUETOOTH_SEND_DELAY_MULTIPLIER{4.8};
const unsigned char Arduino::CAN_FRAME{0};
static const unsigned int ADD_CAN_MASK_RETURN_SIZE{2};
static const unsigned int REMOVE_CAN_MASK_RETURN_SIZE{2};
const BaudRate Arduino::FIRMWARE_BAUD{BaudRate::BAUD115200};
const DataBits Arduino::FIRMWARE_DATA_BITS{DataBits::EIGHT};
const StopBits Arduino::FIRMWARE_STOP_BITS{StopBits::ONE};
const Parity Arduino::FIRMWARE_PARITY{Parity::NONE};

const unsigned int Arduino::CAN_READ_RETURN_SIZE{10};
const unsigned int Arduino::CAN_WRITE_RETURN_SIZE{10};
const unsigned int Arduino::CAN_BUS_ENABLED_RETURN_SIZE{2};
const unsigned int Arduino::CAN_AUTO_UPDATE_RETURN_SIZE{2};
const unsigned int Arduino::CAN_READ_BLANK_RETURN_SIZE{1};
const unsigned int Arduino::CAN_INIT_RETURN_SIZE{1};
const unsigned int Arduino::CAN_ID_WIDTH{3};
const unsigned int Arduino::CAN_BYTE_WIDTH{2};
const unsigned int Arduino::SERIAL_REPORT_REQUEST_TIME_LIMIT{250};
const unsigned int Arduino::SERIAL_REPORT_OVERALL_TIME_LIMIT{550};

const char *Arduino::CAN_INIT_HEADER{"{caninit"};
const char *Arduino::CAN_READ_HEADER{"{canread"};
const char *Arduino::CAN_WRITE_HEADER{"{canwrite"};
const char *Arduino::CAN_LIVE_UPDATE_HEADER{"{canlup"};
const char *Arduino::CAN_CLEAR_MESSAGE_HEADER{"{canmsgclear"};

std::vector<ProtectedSerialPort> Arduino::s_serialPorts;
unsigned int Arduino::s_SERIAL_PORT_TRY_COUNT_HIGH_LIMIT{3};
double Arduino::bluetoothSendDelayMultiplier{Arduino::DEFAULT_BLUETOOTH_SEND_DELAY_MULTIPLIER};

void Arduino::setArduinoType(ArduinoType arduinoType)
{
    if (arduinoType == ArduinoType::UNO) {

    } else if (arduinoType == ArduinoType::NANO) {

    } else if (arduinoType == ArduinoType::MEGA) {

    }
}

int Arduino::voltageToAnalog(double state)
{
    return (state / Arduino::ANALOG_TO_VOLTAGE_SCALE_FACTOR);
}

double Arduino::analogToVoltage(int state)
{
    return state * Arduino::ANALOG_TO_VOLTAGE_SCALE_FACTOR;
}

unsigned int Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT()
{
    return Arduino::s_SERIAL_PORT_TRY_COUNT_HIGH_LIMIT;
}

int Arduino::numberOfSerialPorts() const
{
    return Arduino::s_serialPorts.size();
}

bool Arduino::isBluetooth(int serialPortIndex)
{
    using namespace ArduinoPCStrings;
    return (Arduino::serialPortAtIndex(serialPortIndex)->portName().find(BLUETOOTH_SERIAL_IDENTIFIER) != std::string::npos);
}

bool Arduino::isBluetooth(const std::string &name)
{
    using namespace ArduinoPCStrings;
    return (name.find(BLUETOOTH_SERIAL_IDENTIFIER) != std::string::npos);
}

bool Arduino::isBluetooth(std::shared_ptr<SerialPort> serialPort)
{
    if (!serialPort) {
        return false;
    }
    return Arduino::isBluetooth(serialPort->portName());
}

int Arduino::addSerialPort(std::shared_ptr<SerialPort> serialPort)
{
    Arduino::s_serialPorts.emplace_back(serialPort);
    return (Arduino::s_serialPorts.size()-1);
}

std::shared_ptr<SerialPort> Arduino::serialPortAtIndex(unsigned int serialPortIndex)
{
    using namespace ArduinoPCStrings;
    if (serialPortIndex > Arduino::s_serialPorts.size()-1) {
        throw std::runtime_error(SERIAL_PORT_INVALID_INDEX_STRING + std::to_string(serialPortIndex));
    } else {
        return Arduino::s_serialPorts.at(serialPortIndex).serialPort();
    }
}

std::shared_ptr<std::mutex> Arduino::ioMutexAtIndex(unsigned int ioMutexIndex)
{
    using namespace ArduinoPCStrings;
    if (ioMutexIndex > Arduino::s_serialPorts.size()-1) {
        throw std::runtime_error(IO_MUTEX_INVALID_INDEX_STRING + std::to_string(ioMutexIndex));
    } else {
        return (Arduino::s_serialPorts.at(ioMutexIndex).ioMutex());
    }
}


std::vector<std::string> Arduino::genericIOTask(const std::string &stringToSend, const std::string &header, int serialPortIndex, double delay)
{
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    return Arduino::genericIOTask(stringToSend, header, serialPort, delay);
}

std::vector<std::string> Arduino::genericIOTask(const std::string &stringToSend, const std::string &header, std::shared_ptr<SerialPort> serialPort, double delay)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    serialPort->flushRXTX();
    serialPort->writeString(stringToSend);
#if defined(__USE_SERIAL_WRITE_DELAY__)
    delayMilliseconds(isBluetooth(serialPort->portName()) ? delay*Arduino::bluetoothSendDelayMultiplier : delay);
#else
    if (isBluetooth(serialPort->portName())) {
        delayMilliseconds(delay);
    }
#endif
    std::unique_ptr<std::string> returnString{std::make_unique<std::string>("")};
    *returnString = serialPort->readStringUntil('}');
    if (startsWith(*returnString, header) && endsWith(*returnString, '}')) {
        *returnString = returnString->substr(static_cast<std::string>(header).length() + 1);
        *returnString = returnString->substr(0, returnString->length()-1);
    } else {
        return std::vector<std::string>{};
    }
    return parseToVector(*returnString, ':');
}

std::vector<std::string> Arduino::genericIOReportTask(const std::string &stringToSend, const std::string &header, const std::string &endHeader, int serialPortIndex, double delay)
{
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    return Arduino::genericIOReportTask(stringToSend, header, endHeader, serialPort, delay);
}

std::vector<std::string> Arduino::genericIOReportTask(const std::string &stringToSend, const std::string &header, const std::string &endHeader, std::shared_ptr<SerialPort> serialPort, double delay)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    serialPort->flushRXTX();
    serialPort->writeString(stringToSend);
#if defined(__USE_SERIAL_WRITE_DELAY__)
    delayMilliseconds(isBluetooth(serialPort->portName()) ? delay*Arduino::bluetoothSendDelayMultiplier : delay);
#else
    if (isBluetooth(serialPort->portName())) {
        delayMilliseconds(delay);
    }
#endif
    std::unique_ptr<std::string> returnString{std::make_unique<std::string>("")};
    *returnString = serialPort->readStringUntil(endHeader);
    if (startsWith(*returnString, header) && endsWith(*returnString, '}')) {
        *returnString = returnString->substr(static_cast<std::string>(header).length() + 1);
        *returnString = returnString->substr(0, returnString->length()-1);
    } else {
        return std::vector<std::string>{};
    }
    return parseToVector(*returnString, ';');
}

std::pair<IOStatus, std::string> Arduino::getArduinoType(std::shared_ptr<SerialPort> serialPort)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::ARDUINO_TYPE_HEADER) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::ARDUINO_TYPE_HEADER, serialPort)};
        if (states.size() != Arduino::ARDUINO_TYPE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, "");
            } else {
                continue;
            }
        }
        if (states.at(ArduinoType::OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, "");
            } else {
                continue;
            }
        }
        return std::make_pair(IOStatus::OPERATION_SUCCESS, states.at(ArduinoType::RETURN_STATE));
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, "");
}

std::pair<IOStatus, std::string> Arduino::getFirmwareVersion(std::shared_ptr<SerialPort> serialPort)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::FIRMWARE_VERSION_HEADER) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::FIRMWARE_VERSION_HEADER, serialPort)};
        if (states.size() != Arduino::ARDUINO_TYPE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, "");
            } else {
                continue;
            }
        }
        if (states.at(ArduinoType::OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, "");
            } else {
                continue;
            }
        }
        return std::make_pair(IOStatus::OPERATION_SUCCESS, states.at(ArduinoType::RETURN_STATE));
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, "");
}

std::pair<IOStatus, bool> Arduino::getCanCapability(std::shared_ptr<SerialPort> serialPort)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::CAN_BUS_ENABLED_HEADER) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::CAN_BUS_ENABLED_HEADER, serialPort)};
        if (states.size() != Arduino::CAN_BUS_ENABLED_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            }
        }
        if (states.at(CanEnabledStatus::CAN_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, (std::stoi(states.at(CanEnabledStatus::CAN_RETURN_STATE)) == 1));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
}

std::pair<IOStatus, int> Arduino::getAnalogToDigitalThreshold(std::shared_ptr<SerialPort> serialPort)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::CURRENT_A_TO_D_THRESHOLD_HEADER) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::CURRENT_A_TO_D_THRESHOLD_HEADER, serialPort)};
        if (states.size() != Arduino::CAN_BUS_ENABLED_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (states.at(ADThresholdReq::AD_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(ADThresholdReq::AD_RETURN_STATE)));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
}

void Arduino::writeRawString(const std::string &str, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    Arduino::serialPortAtIndex(serialPortIndex)->writeString(str);
}

std::string Arduino::readRawString(int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    return Arduino::serialPortAtIndex(serialPortIndex)->readString();
}


void Arduino::flushRX(int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    Arduino::serialPortAtIndex(serialPortIndex)->flushRX();
}

void Arduino::flushTX(int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    Arduino::serialPortAtIndex(serialPortIndex)->flushTX();
}

void Arduino::flushRXTX(int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    if (!Arduino::serialPortAtIndex(serialPortIndex)->isOpen()) {
        Arduino::serialPortAtIndex(serialPortIndex)->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    Arduino::serialPortAtIndex(serialPortIndex)->flushRXTX();
}

void Arduino::flushTXRX(int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    Arduino::serialPortAtIndex(serialPortIndex)->flushTXRX();
}

std::pair<IOStatus, bool> Arduino::canAutoUpdate(bool state, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::CAN_LIVE_UPDATE_HEADER) + ':' + std::to_string(state) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::CAN_LIVE_UPDATE_HEADER, serialPortIndex)};
        if (states.size() != Arduino::CAN_AUTO_UPDATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        if (std::to_string(state) != states.at(CanEnabledStatus::CAN_RETURN_STATE)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        if (states.at(CanEnabledStatus::CAN_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(CanEnabledStatus::CAN_OPERATION_RESULT)) == 1);
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
}

std::pair<IOStatus, bool> Arduino::initializeCanBus(int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::CAN_INIT_HEADER) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::CAN_INIT_HEADER, serialPortIndex, Arduino::BLUETOOTH_SERIAL_SEND_DELAY*3)};
        if (states.size() == 0) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        if (states.at(0) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        } else {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, true);
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
}

std::pair<IOStatus, CanMessage> Arduino::canRead(int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::CAN_READ_HEADER) + '}'};
    CanMessage emptyMessage{0, 0, 0, CanDataPacket()};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::CAN_READ_HEADER, serialPortIndex)};
        if ((states.size() == Arduino::CAN_READ_RETURN_SIZE) && (states.size() != Arduino::CAN_READ_BLANK_RETURN_SIZE)){
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
            } else {
                continue;
            }
        }
        if (states.size() == Arduino::CAN_READ_BLANK_RETURN_SIZE) {
            if (states.at(0) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
                } else {
                    continue;
                }
            } else {
                return std::make_pair(IOStatus::OPERATION_SUCCESS, emptyMessage);
            }
        }
        if (states.at(CanIOStatus::CAN_IO_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
            } else {
                continue;
            }
        }
        std::string message{states.at(CanIOStatus::MESSAGE_ID)};
        for (unsigned int i = CanIOStatus::BYTE_0; i < CanIOStatus::CAN_IO_OPERATION_RESULT; i++) {
            message += ":" + states.at(i);
        }
        return std::make_pair(IOStatus::OPERATION_SUCCESS, CanMessage::parseCanMessage(message));
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
}

std::pair<IOStatus, CanMessage> Arduino::canWrite(const CanMessage &message, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    CanMessage emptyMessage{0, 0, 0, CanDataPacket()};
    std::string stringToSend{static_cast<std::string>(Arduino::CAN_WRITE_HEADER) + ":" + message.toString() + "}" };
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::CAN_WRITE_HEADER, serialPortIndex)};
        if ((states.size() == Arduino::CAN_READ_RETURN_SIZE) && (states.size() != Arduino::CAN_READ_BLANK_RETURN_SIZE)){
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
            } else {
                continue;
            }
        }
        if (states.size() != Arduino::CAN_WRITE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
            } else {
                continue;
            }
        }
        if (states.at(CanIOStatus::CAN_IO_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
            } else {
                continue;
            }
        }
        std::string returnMessage{states.at(CanIOStatus::MESSAGE_ID) + ":"};
        for (unsigned int i = CanIOStatus::BYTE_0; i < CanIOStatus::CAN_IO_OPERATION_RESULT; i++) {
            returnMessage += ":" + states.at(i);
        }
        return std::make_pair(IOStatus::OPERATION_SUCCESS, CanMessage::parseCanMessage(returnMessage));
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
}

IOReport Arduino::ioReportRequest(int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::IO_REPORT_HEADER) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> allStates{Arduino::genericIOReportTask(stringToSend, Arduino::IO_REPORT_HEADER, static_cast<std::string>(Arduino::IO_REPORT_END_HEADER) + '}', serialPortIndex, 100)};
        IOReport ioReport;
        for (auto &it : allStates) {
            it = stripAllFromString(it, '}');
            it = stripAllFromString(it, '{');
            std::vector<std::string> states{parseToVector(it, ':')};
            std::string endCheck{stripAllFromString(Arduino::IO_REPORT_END_HEADER, '{')};
            endCheck = stripAllFromString(endCheck, '}');
            if (it.find(endCheck) != std::string::npos) {
                continue;
            }
            if ((states.size() != Arduino::IO_REPORT_RETURN_SIZE) && (states.size() != 0)) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    throw std::runtime_error(IO_REPORT_INVALID_DATA_STRING);
                } else {
                    break;
                }
            }
            if (states.size() == 0) {
                continue;
            }
            IOType ioType{parseIOTypeFromString(states.at(IOReportEnum::IO_TYPE))};
            if ((ioType == IOType::DIGITAL_INPUT) || (ioType == IOType::DIGITAL_INPUT_PULLUP)) {
                ioReport.addDigitalInputResult(std::make_pair(std::stoi(states.at(IOReportEnum::IO_PIN_NUMBER)), std::stoi(states.at(IOReportEnum::IO_STATE))));
            } else if (ioType == IOType::DIGITAL_OUTPUT) {
                ioReport.addDigitalOutputResult(std::make_pair(std::stoi(states.at(IOReportEnum::IO_PIN_NUMBER)), std::stoi(states.at(IOReportEnum::IO_STATE))));
            } else if (ioType == IOType::ANALOG_INPUT) {
                ioReport.addAnalogInputResult(std::make_pair(std::stoi(states.at(IOReportEnum::IO_PIN_NUMBER)), std::stoi(states.at(IOReportEnum::IO_STATE))));
            } else if (ioType == IOType::ANALOG_OUTPUT) {
                ioReport.addAnalogOutputResult(std::make_pair(std::stoi(states.at(IOReportEnum::IO_PIN_NUMBER)), std::stoi(states.at(IOReportEnum::IO_STATE))));
            }
        }
        return ioReport;
    }
    return IOReport{};
}

SerialReport Arduino::serialReportRequest(int serialPortIndex, const std::string &delimiter)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    SerialReport serialReport;
    std::unique_ptr<std::string> readString{std::make_unique<std::string>("")};
    std::unique_ptr<EventTimer> eventTimer{std::make_unique<EventTimer>()};
    std::unique_ptr<EventTimer> overallTimeout{std::make_unique<EventTimer>()};
    std::string returnString{""};
    eventTimer->start();
    overallTimeout->start();
    if (delimiter == "") {
        std::vector<std::string> stringsToPrint;
        if (returnString.find("}{") != std::string::npos) {
            while (returnString.find("}{") != std::string::npos) {
                stringsToPrint.emplace_back(returnString.substr(0, returnString.find("}{")+1));
                returnString = returnString.substr(returnString.find("}{") + static_cast<std::string>("}{").length()-1);
            }
        } else {
            stringsToPrint.emplace_back(returnString);
        }
        for (auto &it : stringsToPrint) {
            serialReport.addSerialResult(it);
        }
       return serialReport;
    } else {
        do {
            eventTimer->update();
            overallTimeout->update();
            *readString = serialPort->readString();
            if ((*readString == "") || (isWhitespace(*readString))) {
                continue;
            } else {
                eventTimer->restart();
                returnString += *readString;
            }
        } while ((eventTimer->totalMilliseconds() <= Arduino::SERIAL_REPORT_REQUEST_TIME_LIMIT) &&
                 (overallTimeout->totalMilliseconds() <= Arduino::SERIAL_REPORT_OVERALL_TIME_LIMIT) &&
                 (!endsWith(returnString, delimiter)));
    }
    std::vector<std::string> stringsToPrint;
    if (returnString.find("}{") != std::string::npos) {
        while (returnString.find("}{") != std::string::npos) {
            stringsToPrint.emplace_back(returnString.substr(0, returnString.find("}{")+1));
            returnString = returnString.substr(returnString.find("}{") + static_cast<std::string>("}{").length()-1);
        }
    } else {
        stringsToPrint.emplace_back(returnString);
    }
    for (auto &it : stringsToPrint) {
        serialReport.addSerialResult(it);
    }
    return serialReport;
}

CanReport Arduino::canReportRequest(int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        CanReport canReport;
        std::pair<IOStatus, CanMessage> result{Arduino::canListen(serialPortIndex)};
        if (result.first == IOStatus::OPERATION_FAILURE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                throw std::runtime_error(CAN_REPORT_INVALID_DATA_STRING);
            } else {
                continue;
            }
        } else {
            canReport.addCanMessageResult(result.second);
        }
        return canReport;
    }
    return CanReport{};
}

std::pair<IOStatus, CanMessage> Arduino::canListen(int serialPortIndex, double delay)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::lock_guard<std::mutex> ioLock{*Arduino::ioMutexAtIndex(serialPortIndex)};
    std::shared_ptr<SerialPort> serialPort{Arduino::serialPortAtIndex(serialPortIndex)};
    if (!serialPort->isOpen()) {
        serialPort->openPort();
        delayMilliseconds(Arduino::BOOTLOADER_BOOT_TIME);
    }
    std::string stringToSend{static_cast<std::string>(Arduino::CAN_READ_HEADER) + '}'};
    CanMessage emptyMessage{0, 0, 0, CanDataPacket()};
    serialPort->writeString(stringToSend);

#if defined(__USE_SERIAL_WRITE_DELAY__)
    delayMilliseconds(isBluetooth(serialPort->portName()) ? delay*Arduino::bluetoothSendDelayMultiplier : delay);
#else
    if (isBluetooth(serialPort)) {
        delayMilliseconds(delay);
    }
#endif
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::unique_ptr<std::string> returnString{std::make_unique<std::string>("")};
        *returnString = serialPort->readStringUntil('}');
        bool canRead{false};
        if ((returnString->find(CAN_EMPTY_READ_SUCCESS_STRING) != std::string::npos) && (returnString->length() > static_cast<std::string>(CAN_EMPTY_READ_SUCCESS_STRING).length() + 10)) {
            *returnString = returnString->substr(static_cast<std::string>(CAN_EMPTY_READ_SUCCESS_STRING).length());
        }
        if (startsWith(*returnString, Arduino::CAN_READ_HEADER) && endsWith(*returnString, '}')) {
            *returnString = returnString->substr(static_cast<std::string>(Arduino::CAN_READ_HEADER).length() + 1);
            *returnString = returnString->substr(0, returnString->length()-1);
            canRead = true;
        } else if (startsWith(*returnString, Arduino::CAN_WRITE_HEADER) && endsWith(*returnString, '}')) {
            *returnString = returnString->substr(static_cast<std::string>(Arduino::CAN_WRITE_HEADER).length() + 1);
            *returnString = returnString->substr(0, returnString->find('}')+1);
        } else {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
            } else {
                continue;
            }
        }
        if (canRead) {
            std::vector<std::string> states{parseToVector(*returnString, ':')};
            if ((states.size() != Arduino::CAN_READ_RETURN_SIZE) && (states.size() != Arduino::CAN_READ_BLANK_RETURN_SIZE)){
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
                } else {
                    continue;
                }
            }
            if (states.size() == Arduino::CAN_READ_BLANK_RETURN_SIZE) {
                if (states.at(0) == OPERATION_FAILURE_STRING) {
                    if (i+1 == Arduino::IO_TRY_COUNT) {
                        return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
                    } else {
                        continue;
                    }
                } else {
                    return std::make_pair(IOStatus::OPERATION_SUCCESS, emptyMessage);
                }
            }
            if (states.at(CanIOStatus::CAN_IO_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
                } else {
                    continue;
                }
            }
            std::string message{states.at(CanIOStatus::MESSAGE_ID)};
            for (unsigned int i = CanIOStatus::BYTE_0; i < CanIOStatus::CAN_IO_OPERATION_RESULT; i++) {
                message += ":" + states.at(i);
            }
            return std::make_pair(IOStatus::OPERATION_SUCCESS, CanMessage::parseCanMessage(message));
        } else {
            std::vector<std::string> states{parseToVector(*returnString, ':')};
            if (states.size() != Arduino::CAN_WRITE_RETURN_SIZE) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
                } else {
                    continue;
                }
            }
            if (states.at(CanIOStatus::CAN_IO_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
                } else {
                    continue;
                }
            }
            std::string message{states.at(CanIOStatus::MESSAGE_ID)};
            for (unsigned int i = CanIOStatus::BYTE_0; i < CanIOStatus::CAN_IO_OPERATION_RESULT; i++) {
                message += ":" + states.at(i);
            }
            return std::make_pair(IOStatus::OPERATION_SUCCESS, CanMessage::parseCanMessage(message));
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, emptyMessage);
}

std::pair<IOStatus, uint32_t> Arduino::addCanMask(CanMaskType canMaskType, const std::string &mask, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{""};
    if (canMaskType == CanMaskType::POSITIVE) {
        stringToSend = static_cast<std::string>(Arduino::ADD_POSITIVE_CAN_MASK_HEADER) + ':' + mask + '}';
    } else if (canMaskType == CanMaskType::NEGATIVE) {
        stringToSend = static_cast<std::string>(Arduino::ADD_NEGATIVE_CAN_MASK_HEADER) + ':' + mask + '}';
    } else {
        return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
    }
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        if (canMaskType == CanMaskType::POSITIVE) {
            std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::ADD_POSITIVE_CAN_MASK_HEADER, serialPortIndex)};
            if (states.size() != Arduino::ADD_CAN_MASK_RETURN_SIZE) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            if (mask != states.at(CanMask::CAN_MASK_RETURN_STATE)) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            if (states.at(CanMask::CAN_MASK_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            try {
                return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(CanMask::CAN_MASK_RETURN_STATE)));
            } catch (std::exception &e) {
                (void)e;
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
        } else if (canMaskType == CanMaskType::NEGATIVE) {
            std::string stringToSend{static_cast<std::string>(Arduino::ADD_NEGATIVE_CAN_MASK_HEADER) + ':' + mask + '}'};
            std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::ADD_NEGATIVE_CAN_MASK_HEADER, serialPortIndex)};
            if (states.size() != Arduino::ADD_CAN_MASK_RETURN_SIZE) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            if (mask != states.at(CanMask::CAN_MASK_RETURN_STATE)) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            if (states.at(CanMask::CAN_MASK_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            try {
                return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(CanMask::CAN_MASK_RETURN_STATE)));
            } catch (std::exception &e) {
                (void)e;
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
        } else {
            return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
}


std::pair<IOStatus, uint32_t> Arduino::removeCanMask(CanMaskType canMaskType, const std::string &mask, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{""};
    if (canMaskType == CanMaskType::POSITIVE) {
        stringToSend = static_cast<std::string>(Arduino::REMOVE_POSITIVE_CAN_MASK_HEADER) + ':' + mask + '}';
    } else if (canMaskType == CanMaskType::NEGATIVE) {
        stringToSend = static_cast<std::string>(Arduino::REMOVE_NEGATIVE_CAN_MASK_HEADER) + ':' + mask + '}';
    } else {
        return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
    }
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        if (canMaskType == CanMaskType::POSITIVE) {
            std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::REMOVE_POSITIVE_CAN_MASK_HEADER, serialPortIndex)};
            if (states.size() != Arduino::REMOVE_CAN_MASK_RETURN_SIZE) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            if (mask != states.at(CanMask::CAN_MASK_RETURN_STATE)) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            if (states.at(CanMask::CAN_MASK_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            try {
                return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(CanMask::CAN_MASK_RETURN_STATE)));
            } catch (std::exception &e) {
                (void)e;
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
        } else if (canMaskType == CanMaskType::NEGATIVE) {
            std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::REMOVE_NEGATIVE_CAN_MASK_HEADER, serialPortIndex)};
            if (states.size() != Arduino::REMOVE_CAN_MASK_RETURN_SIZE) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            if (mask != states.at(CanMask::CAN_MASK_RETURN_STATE)) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            if (states.at(CanMask::CAN_MASK_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
            try {
                return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(CanMask::CAN_MASK_RETURN_STATE)));
            } catch (std::exception &e) {
                (void)e;
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
                } else {
                    continue;
                }
            }
        } else {
            return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
}

std::pair<IOStatus, bool> Arduino::removeAllCanMasks(CanMaskType canMaskType, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{""};
    if (canMaskType == CanMaskType::POSITIVE) {
        stringToSend = static_cast<std::string>(Arduino::REMOVE_ALL_POSITIVE_CAN_MASKS_HEADER) + '}';
    } else if (canMaskType == CanMaskType::NEGATIVE) {
        stringToSend = static_cast<std::string>(Arduino::REMOVE_ALL_NEGATIVE_CAN_MASKS_HEADER) + '}';
    } else {
        stringToSend = static_cast<std::string>(Arduino::REMOVE_ALL_CAN_MASKS_HEADER) + '}';
    }

    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        if (canMaskType == CanMaskType::POSITIVE) {
            std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::REMOVE_ALL_POSITIVE_CAN_MASKS_HEADER, serialPortIndex)};
            if (states.size() != 1) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
                } else {
                    continue;
                }
            }
            if (states.at(0) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
                } else {
                    continue;
                }
            }
            return std::make_pair(IOStatus::OPERATION_SUCCESS, true);
        } else if (canMaskType == CanMaskType::NEGATIVE) {
            std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::REMOVE_ALL_NEGATIVE_CAN_MASKS_HEADER, serialPortIndex)};
            if (states.size() != 1) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
                } else {
                    continue;
                }
            }
            if (states.at(0) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
                } else {
                    continue;
                }
            }
            return std::make_pair(IOStatus::OPERATION_SUCCESS, true);
        } else {
            std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::REMOVE_ALL_CAN_MASKS_HEADER, serialPortIndex)};
            if (states.size() != 1) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
                } else {
                    continue;
                }
            }
            if (states.at(0) == OPERATION_FAILURE_STRING) {
                if (i+1 == Arduino::IO_TRY_COUNT) {
                    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
                } else {
                    continue;
                }
            }
            return std::make_pair(IOStatus::OPERATION_SUCCESS, true);
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
}

std::pair<IOStatus, int> Arduino::changeAnalogToDigitalThreshold(int threshold, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::CHANGE_A_TO_D_THRESHOLD_HEADER) + ':' + std::to_string(threshold) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::CHANGE_A_TO_D_THRESHOLD_HEADER, serialPortIndex)};
        if (states.size() != Arduino::A_TO_D_THRESHOLD_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (std::to_string(threshold) != states.at(ADThresholdReq::AD_RETURN_STATE)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (states.at(ADThresholdReq::AD_OPERATION_RESULT) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(ADThresholdReq::AD_RETURN_STATE)));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
}


std::pair<IOStatus, IOType> Arduino::pinMode(int pinNumber, IOType ioType, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::PIN_TYPE_CHANGE_HEADER) + ':' + std::to_string(pinNumber) + ':' + parseIOType(ioType) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::PIN_TYPE_CHANGE_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, parseIOTypeFromString(states.at(IOState::STATE)));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
}


std::pair<IOStatus, IOType> Arduino::currentPinMode(int pinNumber, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::PIN_TYPE_HEADER) + ':' + std::to_string(pinNumber) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::PIN_TYPE_HEADER, serialPortIndex)};
        if (states.size() != Arduino::PIN_TYPE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, parseIOTypeFromString(states.at(IOState::STATE)));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, IOType::UNSPECIFIED);
}

std::pair<IOStatus, bool> Arduino::digitalRead(int pinNumber, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::DIGITAL_READ_HEADER) + ":" + std::to_string(pinNumber) + '}' };
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::DIGITAL_READ_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(IOState::STATE)) == 1);
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
}

std::pair<IOStatus, bool> Arduino::digitalWrite(int pinNumber, bool state, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::DIGITAL_WRITE_HEADER) + ":" + std::to_string(pinNumber) + ":" + std::to_string(state) + '}' };
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::DIGITAL_WRITE_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(IOState::STATE)) == 1);
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, false);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, false);
}

std::pair<IOStatus, bool> Arduino::softDigitalRead(int pinNumber, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::SOFT_DIGITAL_READ_HEADER) + ":" + std::to_string(pinNumber) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::SOFT_DIGITAL_READ_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(IOState::STATE)));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
}

std::pair<IOStatus, double> Arduino::analogRead(int pinNumber, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::ANALOG_READ_HEADER) + ":" + std::to_string(pinNumber) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::ANALOG_READ_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, Arduino::analogToVoltage(std::stoi(states.at(IOState::STATE))));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
}

std::pair<IOStatus, int> Arduino::analogReadRaw(int pinNumber, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::ANALOG_READ_HEADER) + ":" + std::to_string(pinNumber) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::ANALOG_READ_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(IOState::STATE)));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
}

std::pair<IOStatus, double> Arduino::softAnalogRead(int pinNumber, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::SOFT_ANALOG_READ_HEADER) + ":" + std::to_string(pinNumber) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, SOFT_ANALOG_READ_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, Arduino::analogToVoltage(std::stoi(states.at(IOState::STATE))));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
}

std::pair<IOStatus, int> Arduino::softAnalogReadRaw(int pinNumber, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::SOFT_ANALOG_READ_HEADER) + ":" + std::to_string(pinNumber) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::SOFT_ANALOG_READ_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(IOState::STATE)));
        } catch (std::exception &e) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
}

std::pair<IOStatus, double> Arduino::analogWrite(int pinNumber, double state, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::ANALOG_WRITE_HEADER) + ":" + std::to_string(Arduino::voltageToAnalog(pinNumber)) + ":" + std::to_string(state) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::ANALOG_WRITE_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stod(states.at(IOState::STATE)));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0.00);
}

std::pair<IOStatus, int> Arduino::analogWriteRaw(int pinNumber, int state, int serialPortIndex)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::string stringToSend{static_cast<std::string>(Arduino::ANALOG_WRITE_HEADER) + ":" + std::to_string(pinNumber) + ":" + std::to_string(state) + '}'};
    for (int i = 0; i < Arduino::IO_TRY_COUNT; i++) {
        std::vector<std::string> states{Arduino::genericIOTask(stringToSend, Arduino::ANALOG_WRITE_HEADER, serialPortIndex)};
        if (states.size() != Arduino::IO_STATE_RETURN_SIZE) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (std::to_string(pinNumber) != states.at(IOState::PIN_NUMBER)) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        if (states.at(IOState::RETURN_CODE) == OPERATION_FAILURE_STRING) {
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
        try {
            return std::make_pair(IOStatus::OPERATION_SUCCESS, std::stoi(states.at(IOState::STATE)));
        } catch (std::exception &e) {
            (void)e;
            if (i+1 == Arduino::IO_TRY_COUNT) {
                return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
            } else {
                continue;
            }
        }
    }
    return std::make_pair(IOStatus::OPERATION_FAILURE, 0);
}

std::pair<IOStatus, std::string> Arduino::getArduinoType(int serialPortIndex)
{
    return Arduino::getArduinoType(Arduino::serialPortAtIndex(serialPortIndex));
}

bool Arduino::parseToDigitalState(const std::string &state) const
{
    using namespace ArduinoPCStrings;
    std::string copyString{state};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    for (auto &it : DIGITAL_STATE_HIGH_IDENTIFIERS) {
        if (copyString == static_cast<std::string>(it)) {
            return HIGH;
        }
    }

    for (auto &it : DIGITAL_STATE_LOW_IDENTIFIERS) {
        if (copyString == static_cast<std::string>(it)) {
            return LOW;
        }
    }
    throw std::runtime_error(INVALID_STATE_TO_PARSE_TO_DIGITAL_STATE_STRING + state);
}

double Arduino::parseToAnalogState(const std::string &state) const
{
    using namespace ArduinoPCStrings;
    try {
        double temp{std::stod(state.c_str())};
        if (temp > Arduino::VOLTAGE_MAX) {
            temp = Arduino::VOLTAGE_MAX;
        }
        return temp;
    } catch (std::exception &e) {
        throw std::runtime_error(INVALID_STATE_TO_PARSE_TO_ANALOG_STATE_STRING + state);
    }
}

int Arduino::parseToAnalogStateRaw(const std::string &state) const
{
    using namespace ArduinoPCStrings;
    try {
        int temp{std::stoi(state.c_str())};
        if (temp > Arduino::ANALOG_MAX) {
            temp = Arduino::ANALOG_MAX;
        }
        return temp;
    } catch (std::exception &e) {
        throw std::runtime_error(INVALID_STATE_TO_PARSE_TO_ANALOG_STATE_RAW_STRING + state);
    }
}

bool Arduino::isValidDigitalStateIdentifier(const std::string &state) const
{
    using namespace ArduinoPCStrings;
    std::string copyString{state};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    for (auto &it : DIGITAL_STATE_HIGH_IDENTIFIERS) {
        if (copyString == static_cast<std::string>(it)) {
            return true;
        }
    }

    for (auto &it : DIGITAL_STATE_LOW_IDENTIFIERS) {
        if (copyString == static_cast<std::string>(it)) {
            return true;
        }
    }
    return false;
}

bool Arduino::isValidAnalogStateIdentifier(const std::string &state) const
{
    using namespace ArduinoPCStrings;
    for (auto &it : VALID_ANALOG_STATE_IDENTIFIERS) {
        if (state == std::string(1, it)) {
            return true;
        }
    }
    return false;
}

bool Arduino::isValidAnalogRawStateIdentifier(const std::string &state) const
{
    bool match{false};
    for (auto &it : state) {
        match = false;
        for (auto &charIt : Arduino::VALID_ANALOG_STATE_IDENTIFIERS) {
            if ((it == charIt) && (charIt != '.')) {
                match = true;
            }
        }
        if (!match) {
            return false;
        }
    }
    return true;
}

std::string Arduino::parseIOType(IOType ioType)
{
    using namespace ArduinoPCStrings;
    if (ioType == IOType::DIGITAL_INPUT) {
        return DIGITAL_INPUT_IDENTIFIER;
    } else if (ioType == IOType::DIGITAL_OUTPUT) {
        return DIGITAL_OUTPUT_IDENTIFIER;
    } else if (ioType == IOType::ANALOG_INPUT) {
        return ANALOG_INPUT_IDENTIFIER;
    } else if (ioType == IOType::ANALOG_OUTPUT) {
        return ANALOG_OUTPUT_IDENTIFIER;
    } else if (ioType == IOType::DIGITAL_INPUT_PULLUP) {
        return DIGITAL_INPUT_PULLUP_IDENTIFIER;
    } else {
        throw std::runtime_error(UNSPECIFIED_IO_TYPE_TO_PARSE_IO_TYPE_STRING);
    }
}

IOType Arduino::parseIOTypeFromString(const std::string &ioType)
{
    using namespace ArduinoPCStrings;
    if (ioType == DIGITAL_INPUT_IDENTIFIER) {
        return IOType::DIGITAL_INPUT;
    } else if (ioType == DIGITAL_OUTPUT_IDENTIFIER) {
        return IOType::DIGITAL_OUTPUT;
    } else if (ioType == ANALOG_INPUT_IDENTIFIER) {
        return IOType::ANALOG_INPUT;
    } else if (ioType == ANALOG_OUTPUT_IDENTIFIER) {
        return IOType::ANALOG_OUTPUT;
    } else if (ioType == DIGITAL_INPUT_PULLUP_IDENTIFIER) {
        return IOType::DIGITAL_INPUT_PULLUP;
    } else {
        throw std::runtime_error(INVALID_IO_TYPE_BASE_STRING + ioType + INVALID_IO_TYPE_TAIL_STRING);
    }
}

bool Arduino::setPinAlias(int pinNumber, const std::string &newAlias)
{
    if (this->m_gpioPinIterationAliasMap.find(pinNumber) == this->m_gpioPinIterationAliasMap.end()) {
        return false;
    } else {
        this->m_gpioPinIterationAliasMap.find(pinNumber)->second = newAlias;
        return true;
    }
}

bool Arduino::setPinAlias(std::shared_ptr<GPIO> gpio, const std::string &newAlias)
{
    if (!gpio) {
        return false;
    }
    if (this->m_gpioPinIterationAliasMap.find(gpio->pinNumber()) == this->m_gpioPinIterationAliasMap.end()) {
        return false;
    } else {
        this->m_gpioPinIterationAliasMap.find(gpio->pinNumber())->second = newAlias;
        return true;
    }
}


bool Arduino::setPinAlias(const GPIO &gpio, const std::string &newAlias)
{
    if (this->m_gpioPinIterationAliasMap.find(gpio.pinNumber()) == this->m_gpioPinIterationAliasMap.end()) {
        return false;
    } else {
        this->m_gpioPinIterationAliasMap.find(gpio.pinNumber())->second = newAlias;
        return true;
    }
}


bool Arduino::isBluetooth() const
{
    return Arduino::isBluetooth(this->m_serialPortIndex);
}

void Arduino::initializeIO(std::shared_ptr<GlobalLogger> globalLogger)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    for (auto &it : this->m_availablePins) {
        this->m_gpioPins.emplace(it, std::make_shared<GPIO>(it, IOType::UNSPECIFIED));
    }

    for (auto &it : this->m_gpioPins) {
        if (it.first > s_numberOfDigitalPins) {
            if (it.first == CAN_BUS_PIN) {
                m_canPinAlias = std::to_string(it.first);
            }
            this->m_gpioPinsAlias.emplace(analogPinFromNumber(it.first), it.second);
            this->m_gpioPinIterationAliasMap.emplace(it.first, analogPinFromNumber(it.first));
        } else {
            this->m_gpioPinsAlias.emplace(std::to_string(it.first), it.second);
            this->m_gpioPinIterationAliasMap.emplace(it.first, std::to_string(it.first));
        }
    }
    if (this->m_canCapability.second) {
        this->eraseCanPin();
    }

    ConfigurationFileReader cfr;
    for (auto &it : cfr.nanoAliasesMap()) {
        if (confirmValidAlias(it)) {
            std::cout << USING_ALIAS_STRING << tQuoted(it.second) << FOR_PIN_NUMBER_STRING << it.first << std::endl;
            if (globalLogger) {
                *globalLogger << USING_ALIAS_STRING << tQuoted(it.second) << FOR_PIN_NUMBER_STRING << it.first << tEndl();
            }
        }
        if (this->m_gpioPinsAlias.find(std::to_string(it.first)) != this->m_gpioPinsAlias.end()) {
            auto iter = this->m_gpioPinsAlias.find(std::to_string(it.first));
            std::swap(this->m_gpioPinsAlias[std::to_string(it.first)], iter->second);
            this->m_gpioPinsAlias.erase(iter);
            this->m_gpioPinIterationAliasMap.find(it.first)->second = it.second;
        }
    }
    for (auto &it : cfr.nanoIOTypesMap()) {
        if (confirmValidIOType(it)) {
            std::cout << USING_IO_TYPE_STRING << tQuoted(it.second) << FOR_PIN_NUMBER_STRING << it.first << std::endl;
            if (globalLogger) {
                *globalLogger << USING_IO_TYPE_STRING << tQuoted(it.second) << FOR_PIN_NUMBER_STRING << it.first << tEndl();
            }
        }
        auto found{this->m_gpioPins.find(it.first)};
        if (found != this->m_gpioPins.end()) {
            if (found->second->ioType() == IOType::UNSPECIFIED) {
                found->second->setIOType(parseIOTypeFromString(it.second));
            }
        }
    }
    assignIOTypes(globalLogger);
    for (auto &it : cfr.nanoInitialStatesMap()) {
        std::string logString{toString(USING_INITIAL_STATE_STRING) + tQuoted(it.second) + toString(FOR_PIN_NUMBER_STRING) + toString(it.first) + toString(ELIPSES_STRING)};
        try {
            if (confirmValidStates(it)) {
                auto found{this->m_gpioPins.find(it.first)};
                if (found != this->m_gpioPins.end()) {
                    if (found->second->ioType() == IOType::DIGITAL_OUTPUT) {
                        std::pair<IOStatus, bool>  result{this->digitalWrite(found->second, parseToDigitalState(it.second))};
                        if (result.first == IOStatus::OPERATION_FAILURE) {
                            logString.append(GENERIC_FAILED_STRING);
                        } else {
                            logString.append(GENERIC_SUCCESS_STRING);
                        }
                    } else if (found->second->ioType() == IOType::ANALOG_OUTPUT) {
                        std::pair<IOStatus, int> result{this->analogWriteRaw(found->second, parseToAnalogStateRaw(it.second))};
                        if (result.first == IOStatus::OPERATION_FAILURE) {
                            logString.append(GENERIC_FAILED_STRING);
                        } else {
                            logString.append(GENERIC_SUCCESS_STRING);
                        }
                    }
                }
                std::cout << logString << std::endl;
                if (globalLogger) {
                    *globalLogger << logString << tEndl();
                }
            } else {
                //TODO:Failure message
            }
        } catch (std::exception &e) {
            logString.append(GENERIC_FAILED_STRING);
            std::cout << logString << std::endl;
            if (globalLogger) {
                *globalLogger << logString << tEndl();
            }
            (void)e;
        }
    }
    if (cfr.bluetoothMultiplier() != -1) {
        Arduino::bluetoothSendDelayMultiplier = cfr.bluetoothMultiplier();
    }
    std::cout << USING_BLUETOOTH_MULTIPLIER_STRING << Arduino::bluetoothSendDelayMultiplier << std::endl;
    *globalLogger << USING_BLUETOOTH_MULTIPLIER_STRING << Arduino::bluetoothSendDelayMultiplier << tEndl();
}



void Arduino::writeRawString(const std::string &str)
{
    return Arduino::writeRawString(str, this->m_serialPortIndex);
}

std::string Arduino::readRawString()
{
    return Arduino::readRawString(this->m_serialPortIndex);
}

void Arduino::flushRX()
{
    return Arduino::flushRX(this->m_serialPortIndex);
}

void Arduino::flushTX()
{
    return Arduino::flushTX(this->m_serialPortIndex);
}

void Arduino::flushRXTX()
{
    return Arduino::flushRXTX(this->m_serialPortIndex);
}

void Arduino::flushTXRX()
{
    return Arduino::flushTXRX(this->m_serialPortIndex);
}

void Arduino::setCanCapability(const std::pair<bool, bool> &capability)
{
    this->m_canCapability = capability;
    if ((this->m_canCapability.first) && (this->m_canCapability.second)) {
        eraseCanPin();
    }
}

void Arduino::setFirmwareVersion(const std::string &firmwareVersion)
{
    this->m_firmwareVersion = firmwareVersion;
}

std::string Arduino::firmwareVersion()
{
    using namespace ArduinoPCStrings;
    if (this->m_firmwareVersion == "") {
        std::pair<IOStatus, std::string> firmware{Arduino::getFirmwareVersion(Arduino::serialPortAtIndex(this->m_serialPortIndex))};
        if (firmware.first == IOStatus::OPERATION_FAILURE) {
            this->m_firmwareVersion = FIRMWARE_VERSION_UNKNOWN_STRING;
            return this->m_firmwareVersion;
        } else {
            return (this->m_firmwareVersion = firmware.second);
        }
    } else {
        return this->m_firmwareVersion;
    }
}

void Arduino::setAnalogToDigitalThreshold(int state)
{
    this->m_analogToDigitalThreshold = state;
}

int Arduino::analogToDigitalThreshold()
{
    using namespace ArduinoPCStrings;
    if (this->m_analogToDigitalThreshold == -1) {
        std::pair<IOStatus, int> threshold{Arduino::getAnalogToDigitalThreshold(Arduino::serialPortAtIndex(this->m_serialPortIndex))};
        if (threshold.first == IOStatus::OPERATION_FAILURE) {
            return this->m_analogToDigitalThreshold;
        } else {
            return (this->m_analogToDigitalThreshold = threshold.second);
        }
    } else {
        return this->m_analogToDigitalThreshold;
    }
}


void Arduino::eraseCanPin()
{
    this->m_availablePins.erase(this->m_availablePins.find(CAN_BUS_PIN));
    this->m_availablePwmPins.erase(this->m_availablePwmPins.find(CAN_BUS_PIN));
    this->m_gpioPins.erase(this->m_gpioPins.find(CAN_BUS_PIN));
    this->m_gpioPinsAlias.erase(this->m_gpioPinsAlias.find(std::to_string(CAN_BUS_PIN)));
    this->m_numberOfDigitalPins--;

}

std::pair<bool, bool> Arduino::canCapability()
{
    using namespace ArduinoPCStrings;
    if (this->m_canCapability.first == false) {
        std::pair<IOStatus, bool> capability{Arduino::getCanCapability(Arduino::serialPortAtIndex(this->m_serialPortIndex))};
        if (capability.first == IOStatus::OPERATION_FAILURE) {
            return std::make_pair(false, this->m_canCapability.second);
        } else {
            this->m_canCapability = std::make_pair(true, capability.second);
            if (capability.second) {
                eraseCanPin();
            }
            return std::make_pair(true, this->m_canCapability.second);
        }
    } else {
        return std::make_pair(true, this->m_canCapability.second);
    }
}

std::shared_ptr<SerialPort> Arduino::serialPort() const
{
    return Arduino::serialPortAtIndex(this->m_serialPortIndex);
}

bool Arduino::confirmValidAlias(const std::pair<int, std::string> &alias) const
{
    (void)alias;
    return true;
}

bool Arduino::confirmValidIOType(const std::pair<int, std::string> &ioType) const
{
    IOType io{IOType::UNSPECIFIED};
    (void)io;
    try {
        io = parseIOTypeFromString(ioType.second);
        return true;
    } catch (std::exception &e) {
        return false;
    }
}

bool Arduino::confirmValidStates(const std::pair<int, std::string> &state) const
{
    (void)state;
    return true;
}

void Arduino::assignIOTypes(std::shared_ptr<GlobalLogger> globalLogger)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    std::string logString{""};
    for (auto &it : this->m_gpioPins) {
        if (it.second->ioType() == IOType::UNSPECIFIED) {
            it.second->setIOType(this->currentPinMode(it.second).second);
        } else {
            logString = toString(SETTING_IO_TYPE_STRING) + toString(it.first) + toString(TO_IO_TYPE_STRING) + Arduino::parseIOType(it.second->ioType()) + toString(ELIPSES_STRING);
            std::pair<IOStatus, IOType> result{this->pinMode(it.second, it.second->ioType())};
            if (result.first == IOStatus::OPERATION_FAILURE) {
                logString.append(GENERIC_FAILED_STRING);
            } else {
                logString.append(GENERIC_SUCCESS_STRING);
            }
            std::cout << logString << std::endl;
            if (globalLogger) {
                *globalLogger << logString << tEndl();
            }
        }
    }
}

std::shared_ptr<GPIO> Arduino::gpioPinByPinAlias(const std::string &pinAlias) const
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return (this->m_gpioPinsAlias.find(pinAlias)->second);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias);
    }
}

std::shared_ptr<GPIO> Arduino::gpioPinByPinNumber(int pinNumber) const
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(pinNumber) != this->m_gpioPins.end()) {
        return (this->m_gpioPins.find(pinNumber)->second);
    } else {
        throw std::runtime_error(INVALID_PIN_NUMBER_STRING + pinNumber);
    }
}

std::string Arduino::gpioAliasByPinNumber(int number) const
{
    if (this->m_gpioPinIterationAliasMap.find(number) != this->m_gpioPinIterationAliasMap.end()) {
        return this->m_gpioPinIterationAliasMap.find(number)->second;
    } else {
        throw std::runtime_error("");
    }
}

std::pair<IOStatus, bool> Arduino::digitalRead(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return digitalRead(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_DIGITAL_READ_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> Arduino::digitalWrite(const std::string &pinAlias, bool state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return digitalWrite(this->m_gpioPinsAlias.find(pinAlias)->first, state);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_DIGITAL_WRITE_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, double> Arduino::analogRead(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return analogRead(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_ANALOG_READ_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, int> Arduino::analogReadRaw(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return analogReadRaw(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_ANALOG_READ_RAW_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, double> Arduino::analogWrite(const std::string &pinAlias, double state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return analogWrite(this->m_gpioPinsAlias.find(pinAlias)->first, state);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_ANALOG_WRITE_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, int> Arduino::analogWriteRaw(const std::string &pinAlias, int state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return analogWriteRaw(this->m_gpioPinsAlias.find(pinAlias)->first, state);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_ANALOG_WRITE_RAW_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> Arduino::softDigitalRead(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return softDigitalRead(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_SOFT_DIGITAL_READ_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, double> Arduino::softAnalogRead(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return softAnalogRead(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_SOFT_ANALOG_READ_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, int> Arduino::softAnalogReadRaw(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return softAnalogReadRaw(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_SOFT_ANALOG_READ_RAW_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, IOType> Arduino::pinMode(const std::string &pinAlias, IOType ioType)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return pinMode(this->m_gpioPinsAlias.find(pinAlias)->first, ioType);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_PIN_MODE_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, IOType> Arduino::currentPinMode(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return currentPinMode(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_CURRENT_PIN_MODE_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> Arduino::digitalRead(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::digitalRead(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_DIGITAL_READ_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> Arduino::digitalWrite(const GPIO &gpio, bool state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::digitalWrite(gpio.pinNumber(), state, this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_DIGITAL_WRITE_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, double> Arduino::analogRead(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::analogRead(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_ANALOG_READ_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, int> Arduino::analogReadRaw(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::analogReadRaw(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_ANALOG_READ_RAW_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, double> Arduino::analogWrite(const GPIO &gpio, double state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::analogWrite(gpio.pinNumber(), state, this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_ANALOG_WRITE_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, int> Arduino::analogWriteRaw(const GPIO &gpio, int state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::analogWriteRaw(gpio.pinNumber(), state, this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_ANALOG_WRITE_RAW_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> Arduino::softDigitalRead(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::softDigitalRead(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_SOFT_DIGITAL_READ_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, double> Arduino::softAnalogRead(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::softAnalogRead(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_SOFT_ANALOG_READ_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, int> Arduino::softAnalogReadRaw(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::softAnalogReadRaw(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_SOFT_ANALOG_READ_RAW_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, IOType> Arduino::pinMode(GPIO &gpio, IOType ioType)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        std::pair<IOStatus, IOType> result{Arduino::pinMode(gpio.pinNumber(), ioType, this->m_serialPortIndex)};
        if (result.first == IOStatus::OPERATION_SUCCESS) {
            gpio.setIOType(ioType);
        }
        return result;
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_PIN_MODE_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, IOType> Arduino::currentPinMode(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::currentPinMode(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_CURRENT_PIN_MODE_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> Arduino::digitalRead(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return digitalRead(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_DIGITAL_READ_STRING);
    }
}

std::pair<IOStatus, bool> Arduino::digitalWrite(std::shared_ptr<GPIO> gpioPtr, bool state)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return digitalWrite(*gpioPtr, state);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_DIGITAL_WRITE_STRING);
    }
}

std::pair<IOStatus, double> Arduino::analogRead(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return analogRead(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_ANALOG_READ_STRING);
    }
}

std::pair<IOStatus, int> Arduino::analogReadRaw(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return analogReadRaw(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_ANALOG_READ_RAW_STRING);
    }
}

std::pair<IOStatus, double> Arduino::analogWrite(std::shared_ptr<GPIO> gpioPtr, double state)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return analogWrite(*gpioPtr, state);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_ANALOG_WRITE_STRING);
    }
}

std::pair<IOStatus, int> Arduino::analogWriteRaw(std::shared_ptr<GPIO> gpioPtr, int state)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return analogWriteRaw(*gpioPtr, state);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_ANALOG_WRITE_RAW_STRING);
    }
}

std::pair<IOStatus, bool> Arduino::softDigitalRead(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return softDigitalRead(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_SOFT_DIGITAL_READ_STRING);
    }
}

std::pair<IOStatus, double> Arduino::softAnalogRead(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return softAnalogRead(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_SOFT_ANALOG_READ_STRING);
    }
}

std::pair<IOStatus, int> Arduino::softAnalogReadRaw(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return softAnalogReadRaw(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_SOFT_ANALOG_READ_RAW_STRING);
    }
}

std::pair<IOStatus, IOType> Arduino::pinMode(std::shared_ptr<GPIO> gpioPtr, IOType ioType)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return pinMode(*gpioPtr, ioType);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_PIN_MODE_STRING);
    }
}

std::pair<IOStatus, IOType> Arduino::currentPinMode(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return currentPinMode(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_CURRENT_PIN_MODE_STRING);
    }
}


void Arduino::onPinModeImplicitlyChanged(int pinNumber, IOType newIOType)
{
    (void)pinNumber;
    (void)newIOType;
}

void Arduino::onPinModeImplicitlyChanged(const std::string &pinAlias, IOType newIOType)
{
    (void)pinAlias;
    (void)newIOType;
}

void Arduino::onPinModeExplicitlyChanged(int pinNumber, IOType newIOType)
{
    (void)pinNumber;
    (void)newIOType;
}

void Arduino::onPinModeExplicitlyChanged(const std::string &pinAlias, IOType newIOType)
{
    (void)pinAlias;
    (void)newIOType;
}

bool Arduino::isValidAnalogPinIdentifier(const std::string &state) const
{
    for (auto &it : Arduino::s_availableAnalogPins) {
        if (state == analogPinFromNumber(it)) {
            return true;
        }
    }
    return false;
}

bool Arduino::isValidDigitalOutputPin(int pinNumber) const
{
    for (auto &it : Arduino::s_availableAnalogPins) {
        if (pinNumber == it) {
            return true;
        }
    }
    for (auto &it : this->m_availablePins) {
        if (pinNumber == it) {
            return true;
        }
    }
    return false;
}

bool Arduino::isValidDigitalInputPin(int pinNumber) const
{
    for (auto &it : Arduino::s_availableAnalogPins) {
        if (pinNumber == it) {
            return true;
        }
    }
    for (auto &it : this->m_availablePins) {
        if (pinNumber == it) {
            return true;
        }
    }
    return false;
}

bool Arduino::isValidAnalogOutputPin(int pinNumber) const
{
    for (auto &it : Arduino::s_availablePwmPins) {
        if (pinNumber == it) {
            return true;
        }
    }
    return false;
}

bool Arduino::isValidAnalogInputPin(int pinNumber) const
{
    for (auto &it : Arduino::s_availableAnalogPins) {
        if (pinNumber == it) {
            return true;
        }
    }
    return false;
}

std::set<int> Arduino::AVAILABLE_ANALOG_PINS() const
{
    return Arduino::s_availableAnalogPins;
}

std::set<int> Arduino::AVAILABLE_PWM_PINS() const
{
    return Arduino::s_availablePwmPins;
}

std::set<int> Arduino::AVAILABLE_PINS() const
{
    return Arduino::s_availablePins;
}

const char *Arduino::ARDUINO_TYPE_IDENTIFIER_FUNC() const
{
    return Arduino::ARDUINO_TYPE_IDENTIFIER;
}

int Arduino::NUMBER_OF_DIGITAL_PINS() const
{
    return Arduino::s_numberOfDigitalPins;
}

int Arduino::parseAnalogPin(const std::string &pinAlias) const
{
    return Arduino::staticParseAnalogPin(pinAlias);
}

std::string Arduino::analogPinFromNumber(int pinNumber) const
{
    return Arduino::staticAnalogPinFromNumber(pinNumber);
}


