#include <Arduino.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "include/arduinoinit.h"
#include "include/gpio.h"
#include "include/serialportinfo.h"
#include "include/firmwareutilities.h"
#include "include/arduinopcstrings.h"

/* 
 * Standard C++ headers
 * Special thanks to maniacbug for the
 * nice port of the c++ stdlib 
 * https://github.com/maniacbug/StandardCplusplus
 */
#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <serstream>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#if defined(__HAVE_CAN_BUS__)
    #include "include/mcp_can.h"
    #include "include/candatapacket.h"
    #include "include/canmessage.h"
#endif

using namespace ArduinoPCStrings;
using namespace FirmwareUtilities;

#define DEFAULT_NATIVE_PORT std::cout
#define ID_WIDTH 3
#define MESSAGE_WIDTH 2
#define MAXIMUM_SERIAL_READ_SIZE 175
#define PIN_OFFSET 2
#define NEXT_SERIAL_PORT_UNAVAILABLE -1
#define MAXIMUM_SERIAL_PORTS 8

#define SERIAL_BAUD 115200L
#define SERIAL_TIMEOUT 1000

#define OPERATION_FAILURE -1
#define INVALID_PIN -1
#define STATE_FAILURE -1
#define OPERATION_SUCCESS 1
#define OPERATION_KIND_OF_SUCCESS 2
#define OPERATION_PIN_USED_BY_SERIAL_PORT 3
#define PIN_PLACEHOLDER 1
#define EEPROM_ADVANCE 7
#define SOFT 1

void printString(const std::string &str, int coutIndex);
void printStringWithNewline(const std::string &str, int coutIndex);
void printResult(const std::string &header, const std::string &pinNumber, bool state, int resultCode, int coutIndex);
void printResult(const std::string &header, int pinNumber, bool state, int resultCode, int coutIndex);
void printResult(const std::string &header, const std::string &pinNumber, int state, int resultCode, int coutIndex);
void printResult(const std::string &header, int pinNumber, int state, int resultCode, int coutIndex);
void printResult(const std::string &header, const std::string &pinNumber, const std::string &state, int resultCode, int coutIndex);
void printResult(const std::string &header, int pinNumber, const std::string &state, int resultCode, int coutIndex);
void printSingleResult(const std::string &header, int resultCode, int coutIndex);
void printTypeResult(const std::string &header, const std::string &type, int resultCode, int coutIndex);
void printTypeResult(const std::string &header, int type, int resultCode, int coutIndex);
#if defined(__HAVE_CAN_BUS__)
    void printCanResult(const std::string &header, const std::string &str, int resultCode, int coutIndex);
    void printCanResult(const std::string &header, const CanMessage &msg, int resultCode, int coutIndex);
    void printBlankCanResult(const std::string &header, int resultCode, int coutIndex); 
#endif

enum EEPROMWriteOffset { PIN = 0,
                         IO_TYPE = 1,
                         STATE0 = 2,
                         STATE1 = 3,
                         STATE2 = 4,
                         STATE3 = 5,
                         STATE4 = 6 };

void handleSerialString(const std::string &str, int coutIndex);
void handleSerialString(const char *str, int coutIndex);
void handleAPrefixedString(const std::string &str, int coutIndex);
void handleCPrefixedString(const std::string &str, int coutIndex);
void handleDPrefixedString(const std::string &str, int coutIndex);
void handleHPrefixedString(const std::string &str, int coutIndex);
void handleIPrefixedString(const std::string &str, int coutIndex);
void handleLPrefixedString(const std::string &str, int coutIndex);
void handlePPrefixedString(const std::string &str, int coutIndex);
void handleRPrefixedString(const std::string &str, int coutIndex);
void handleSPrefixedString(const std::string &str, int coutIndex);
void handleVPrefixedString(const std::string &str, int coutIndex);

void digitalReadRequest(const std::string &str, bool soft, int coutIndex);
void digitalWriteRequest(const std::string &str, int coutIndex);
void digitalWriteAllRequest(const std::string &str, int coutIndex);
void analogReadRequest(const std::string &str, int coutIndex);
void analogWriteRequest(const std::string &str, int coutIndex);
void softAnalogReadRequest(const std::string &str, int coutIndex);
void changeSerialModeRequest(const std::string &str, int coutIndex);

void pinTypeRequest(const std::string &str, int coutIndex);
void pinTypeChangeRequest(const std::string &str, int coutIndex);

void changeAToDThresholdRequest(const std::string &str, int coutIndex);
void currentAToDThresholdRequest(int coutIndex);

void arduinoTypeRequest(int coutIndex);
void firmwareVersionRequest(int coutIndex);
void canBusEnabledRequest(int coutIndex);
void heartbeatRequest(int coutIndex);
void ioReportRequest(int coutIndex);
void storeSystemStateRequest(int coutIndex);
void loadSystemStateRequest(int coutIndex);

bool isValidAnalogPinIdentifier(const std::string &str);
bool isValidPinIdentifier(const std::string &str);
bool isValidDigitalStateIdentifier(const std::string &str);
bool isValidAnalogStateIdentifier(const std::string &str);
bool isValidPwmPinIdentifier(const std::string &str);
bool isValidPinTypeIdentifier(const std::string &str);

bool checkValidIOChangeRequest(IOType ioType, int pinNumber);
bool checkValidRequestString(const std::string header); 

bool isValidDigitalOutputPin(int pinNumber);
bool isValidDigitalInputPin(int pinNumber);
bool isValidAnalogOutputPin(int pinNumber);
bool isValidAnalogInputPin(int pinNumber);
int parseAnalogPin(const std::string &str);
int parseToState(const std::string &str);

int parseToAnalogState(const std::string &str);
IOType parseIOType(const std::string &str);
int parseToDigitalState(const std::string &str);
int parsePin(const std::string &str);
void populateGpioMap();
IOType getEEPROMIOTypeFromIndex(unsigned char index);
unsigned char getEEPROMIOTypeFromEnumClass(IOType ioType);

std::string getIOTypeString(IOType type);
std::string analogPinFromNumber(int number);
GPIO *gpioPinByPinNumber(int pinNumber);
bool pinInUseBySerialPort(int pinNumber);
int nextAvailableSerialSlotNumber();
void initializeSerialPorts(long long int baud, int timeout);
std::ohserialstream *getHardwareCout(int coutIndex);
std::ohserialstream *getSoftwareCout(int coutIndex);

static std::map<int, GPIO*> gpioPins;

#if defined(ARDUINO_AVR_UNO)
    static const int AVAILABLE_ANALOG_PINS[]{A0, A1, A2, A3, A4, A5, -1};
    static const char AVAILABLE_GENERAL_PINS[]{2, 4, 7, 8, 12, 13, -1};
    #define NUMBER_OF_ANALOG_PINS 6
    #define ANALOG_PIN_OFFSET 13
    #if defined(__HAVE_CAN_BUS__)
        #define NUMBER_OF_PINS 17
        static const char AVAILABLE_PWM_PINS[]{3, 5, 6, 10, 11, -1};
    #else
        #define NUMBER_OF_PINS 18
        static const char AVAILABLE_PWM_PINS[]{3, 5, 6, 9, 10, 11, -1};
    #endif
#elif defined(ARDUINO_AVR_NANO)
    static const char AVAILABLE_ANALOG_PINS[]{A0, A1, A2, A3, A4, A5, A6, A7, -1};                                                
    static const char AVAILABLE_GENERAL_PINS[]{2, 4, 7, 8, 12, 13, -1};
    #define NUMBER_OF_ANALOG_PINS 8
    #define ANALOG_PIN_OFFSET 13
    #if defined(__HAVE_CAN_BUS__)
        #define NUMBER_OF_PINS 19
        static const char AVAILABLE_PWM_PINS[]{3, 5, 6, 10, 11, -1};
    #else
        #define NUMBER_OF_PINS 20
        static const char AVAILABLE_PWM_PINS[]{3, 5, 6, 9, 10, 11, -1};
    #endif
#elif defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560)
    static const char AVAILABLE_ANALOG_PINS[]{A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, -1};
    static const char AVAILABLE_GENERAL_PINS[]{14, 15, 16, 17, 18, 19, 20,21, 22, 23, 24, 
                                                  25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
                                                  38, 39, 40, 41, 42, 43, 47, 48, 49, 50, 51, 52, 53,-1};                                                
    #define NUMBER_OF_ANALOG_PINS 16
    #define ANALOG_PIN_OFFSET 53
    #if defined(__HAVE_CAN_BUS__)
        #define NUMBER_OF_PINS 67
        static const char AVAILABLE_PWM_PINS[]{2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 44, 45, 46, -1};
    #else
        #define NUMBER_OF_PINS 68
        static const char AVAILABLE_PWM_PINS[]{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45, 46, -1};
    #endif
#endif

#if defined(__HAVE_CAN_BUS__)
    #include <set>
    #ifndef INT32U
        #define INT32U unsigned long
    #endif
    #ifndef INT8U
        #define INT8U byte
    #endif
    void canInitRequest(int coutIndex);
    void canReadRequest(bool autoUp = false, int coutIndex);
    void canWriteRequest(const std::string &canMessage, bool once = false, int coutIndex);
    void addNegativeCanMaskRequest(const std::string &canMask, int coutIndex);
    void removePositiveCanMaskRequest(const std::string &canMask, int coutIndex);
    void canLiveUpdateRequest(const std::string &str, int coutIndex);
    void clearCurrentMessageByIdRequest(const std::string &str, int coutIndex);
    void currentCachedCanMessageByIdRequest(const std::string &str, int coutIndex);
    void clearCanMessagesRequest(int coutIndex);
    void currentPositiveCanMasksRequest(int coutIndex);
    void currentNegativeCanMasksRequest(int coutIndex);
    void allCurrentCanMasksRequest(int coutIndex);
    void clearPositiveCanMasksRequest(int coutIndex);
    void clearNegativeCanMasksRequest(int coutIndex);
    void clearCanMasksRequest(int coutIndex);
    void currentCachedCanMessagesRequest(int coutIndex);
    void clearAllCanMasksRequest(int coutIndex);
    std::pair<int, std::string> parseToCanState(const std::string &str);
    void sendCanMessage(const CanMessage &msg);
    #define SPI_CS_PIN 9 
    MCP_CAN *canController{new MCP_CAN(SPI_CS_PIN)};
    #define CAN_CONNECTION_TIMEOUT 1000
    #define CAN_WRITE_REQUEST_SIZE 9
    static bool canBusInitialized{false};
    static bool canLiveUpdate{false};
    CanMessage parseCanMessage(const std::string &str);
    static std::map<uint32_t, CanMessage> lastCanMessages;
    static std::set<uint32_t> positiveCanMasks;
    static std::set<uint32_t> negativeCanMasks;
    void addLastCanMessage(const CanMessage &msg);
    #define CAN_MESSAGE_LENGTH 8
    #define CAN_FRAME 0
    #define CAN_COMMUNICATION_DOWN_TIME 1000
    static int canCommunicationStartTime{0};
    static int canCommunicationEndTime{0};
#endif
    
using HWSerial = SerialPortInfo<HardwareSerial>;
using SWSerial = SerialPortInfo<SoftwareSerial>;

#if defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560)
    namespace std { ohserialstream cout{Serial}; }
    namespace std { ohserialstream cout1{Serial1}; }
    namespace std { ohserialstream cout2{Serial2}; }
    namespace std { ohserialstream cout3{Serial3}; }
    enum SerialIndex {
        BIT_BUCKET = -1,
        NATIVE_PORT_0 = 0,
        NATIVE_PORT_1 = 1,
        NATIVE_PORT_2 = 2,
        NATIVE_PORT_3 = 3,
        SOFTWARE_PORT_0 = 4,
        SOFTWARE_PORT_1 = 5,
        SOFTWARE_PORT_2 = 6,
        SOFTWARE_PORT_3 = 7,
        ALL_NATIVE_PORTS = 8,
        ALL_SOFTWARE_PORTS = 9,
        ALL_PORTS = 10
    };
    static std::vector<HWSerial *> hardwareSerialPorts {
        new HWSerial{&Serial,  &std::cout,  0,  1, SERIAL_BAUD, SERIAL_TIMEOUT, true},
        new HWSerial{&Serial1,  &std::cout1,  19,  18, SERIAL_BAUD, SERIAL_TIMEOUT, true},
        new HWSerial{&Serial2,  &std::cout2,  17,  16, SERIAL_BAUD, SERIAL_TIMEOUT, true},
        new HWSerial{&Serial3,  &std::cout3,  15,  14, SERIAL_BAUD, SERIAL_TIMEOUT, true}
    };

#else
    namespace std { ohserialstream cout{Serial}; }
    enum SerialIndex {
        BIT_BUCKET = -1,
        NATIVE_PORT_0 = 0,
        SOFTWARE_PORT_0 = 1,
        SOFTWARE_PORT_1 = 2,
        SOFTWARE_PORT_2 = 3,
        SOFTWARE_PORT_3 = 4,
        ALL_SOFTWARE_PORTS = 5,
        ALL_PORTS = 6
    };
    static std::vector<HWSerial *> hardwareSerialPorts {
        new HWSerial{&Serial,  &std::cout,  0,  1, SERIAL_BAUD, SERIAL_TIMEOUT, true},
    };

#endif

static std::vector<SWSerial *> softwareSerialPorts {

};

bool isSoftwareCoutStream(int coutIndex);

std::ohserialstream *bitBucket{nullptr};

int main()
{
    arduinoInit();
    initializeSerialPorts();
    populateGpioMap();

    while (true) {
        if (Serial.available()) {
            String serialRead{Serial.readStringUntil('}')};
            if (serialRead.length() > MAXIMUM_SERIAL_READ_SIZE) {
                printString(INVALID_LENGTH_EXCEEDED_HEADER, SerialIndex::NATIVE_PORT_0);
            } else {
                handleSerialString(serialRead.c_str(), SerialIndex::NATIVE_PORT_0);
            }
        }
        #if defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560)
            if (Serial1.available()) {
                String serialRead{Serial1.readStringUntil('}')};
                if (serialRead.length() > MAXIMUM_SERIAL_READ_SIZE) {
                    printString(INVALID_LENGTH_EXCEEDED_HEADER, SerialIndex::NATIVE_PORT_1);
                } else {
                    handleSerialString(serialRead.c_str(), SerialIndex::NATIVE_PORT_1);
                }
            }
            if (Serial2.available()) {
                String serialRead{Serial2.readStringUntil('}')};
                if (serialRead.length() > MAXIMUM_SERIAL_READ_SIZE) {
                    printString(INVALID_LENGTH_EXCEEDED_HEADER, SerialIndex::NATIVE_PORT_2);
                } else {
                    handleSerialString(serialRead.c_str(), SerialIndex::NATIVE_PORT_2);
                }
            }
            if (Serial3.available()) {
                String serialRead{Serial3.readStringUntil('}')};
                if (serialRead.length() > MAXIMUM_SERIAL_READ_SIZE) {
                    printString(INVALID_LENGTH_EXCEEDED_HEADER, SerialIndex::NATIVE_PORT_3);
                } else {
                    handleSerialString(serialRead.c_str(), SerialIndex::NATIVE_PORT_3);
                }
            }
        #endif 
        #if defined(__HAVE_CAN_BUS__)
            if (canLiveUpdate) {
                canReadRequest(canLiveUpdate, SerialIndex::ALL_NATIVE_PORTS);
            }
            bool canSend{false};
            for (auto &it : lastCanMessages) {
                if (it.second->toString() != "") {
                    canSend = true;
                }
            }
            if (canSend) {
                if (canCommunicationStartTime == 0) {
                    canCommunicationStartTime = millis();
                }
                canCommunicationEndTime = millis();
                if (static_cast<unsigned int>((canCommunicationEndTime - canCommunicationStartTime)) >= CAN_COMMUNICATION_DOWN_TIME) {    
                    for (auto &it : lastCanMessages) {
                        sendCanMessage(it.second);
                    }
                    canCommunicationStartTime = millis();
                }
            }
        #endif
        if (serialEventRun) serialEventRun();
    }
    for (auto &it : gpioPins) {
        delete it.second;
    }
    for (auto &it : hardwareSerialPorts) {
        delete it;
    }
    for (auto &it : softwareSerialPorts) {
        delete it;
    }
    #if defined(__HAVE_CAN_BUS__)
        delete canController;
    #endif
    return 0;
}

void handleSerialString(const char *str, int coutIndex)
{
    return handleSerialString(static_cast<std::string>(str), coutIndex);
}

void handleSerialString(const std::string &str, int coutIndex)
{
    if (str.length() == 0) {
        return;
    } else if (isWhitespace(str)) {
        return;
    } else if (startsWith(str, "{a")) {
        handleAPrefixedString(str, coutIndex);
    } else if (startsWith(str, "{c")) {
        handleCPrefixedString(str, coutIndex);
    } else if (startsWith(str, "{d")) {
        handleDPrefixedString(str, coutIndex);
    } else if (startsWith(str, "{h")) {
        handleHPrefixedString(str, coutIndex);
    } else if (startsWith(str, "{i")) {
        handleIPrefixedString(str, coutIndex);
    } else if (startsWith(str, "{l")) {
        handleLPrefixedString(str, coutIndex);
    } else if (startsWith(str, "{p")) {
        handlePPrefixedString(str, coutIndex);
    } else if (startsWith(str, "{r")) {
        handleRPrefixedString(str, coutIndex);
    } else if (startsWith(str, "{s")) {
        handleSPrefixedString(str, coutIndex);
    } else if (startsWith(str, "{v")) {
        handleVPrefixedString(str, coutIndex);
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handleAPrefixedString(const std::string &str, int coutIndex)
{
    if (startsWith(str, ANALOG_READ_HEADER)) {
        if (checkValidRequestString(ANALOG_READ_HEADER, str)) {
            analogReadRequest(str.substr(static_cast<std::string>(ANALOG_READ_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, ANALOG_WRITE_HEADER)) {
        if (checkValidRequestString(ANALOG_WRITE_HEADER, str)) {
            analogWriteRequest(str.substr(static_cast<std::string>(ANALOG_WRITE_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, CHANGE_A_TO_D_THRESHOLD_HEADER)) {
        if (checkValidRequestString(CHANGE_A_TO_D_THRESHOLD_HEADER, str)) {
            changeAToDThresholdRequest(str.substr(static_cast<std::string>(CHANGE_A_TO_D_THRESHOLD_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, CURRENT_A_TO_D_THRESHOLD_HEADER)) {
        currentAToDThresholdRequest(coutIndex);
    } else if (startsWith(str, ARDUINO_TYPE_HEADER)) {
        arduinoTypeRequest(coutIndex);
#if defined(__HAVE_CAN_BUS__)
    } else if (startsWith(str, ADD_POSITIVE_CAN_MASK_HEADER)) {
        if (checkValidRequestString(ADD_POSITIVE_CAN_MASK_HEADER, str)) {
            addPositiveCanMaskRequest(str.substr(static_cast<std::string>(ADD_POSITIVE_CAN_MASK_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, ADD_NEGATIVE_CAN_MASK_HEADER)) {
        if (checkValidRequestString(ADD_NEGATIVE_CAN_MASK_HEADER, str)) {
            addNegativeCanMaskRequest(str.substr(static_cast<std::string>(ADD_NEGATIVE_CAN_MASK_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, ALL_CURRENT_CAN_MASKS_HEADER)) {
        allCurrentCanMasksRequest(coutIndex);
#endif
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handleCPrefixedString(const std::string &str, int coutIndex)
{
    if (startsWith(str, CHANGE_SERIAL_MODE_HEADER)) {
        if (checkValidRequestString(CHANGE_SERIAL_MODE_HEADER, str)) {
            changeSerialModeRequest(str.substr(static_cast<std::string>(CHANGE_SERIAL_MODE_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, CAN_BUS_ENABLED_HEADER)) {
        canBusEnabledRequest(coutIndex); 
#if defined(__HAVE_CAN_BUS__)
    } else if (startsWith(str, CAN_INIT_HEADER)) {
        canInitRequest(coutIndex);
    } else if (startsWith(str, CAN_READ_HEADER)) {
        canReadRequest(coutIndex);
    } else if (startsWith(str, CAN_WRITE_ONCE_HEADER)) {
        if (checkValidRequestString(CAN_WRITE_ONCE_HEADER, str)) {
            canWriteRequest(str.substr(static_cast<std::string>(CAN_WRITE_HEADER).length()+1), true, coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, CAN_WRITE_HEADER)) {
        if (checkValidRequestString(CAN_WRITE_HEADER, str)) {
            canWriteRequest(str.substr(static_cast<std::string>(CAN_WRITE_HEADER).length()+1), false, coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }     
    } else if (startsWith(str, CAN_LIVE_UPDATE_HEADER)) {
        if (checkValidRequestString(CAN_LIVE_UPDATE_HEADER, str)) {
            canLiveUpdateRequest(str.substr(static_cast<std::string>(CAN_LIVE_UPDATE_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, CLEAR_CAN_MESSAGE_BY_ID_HEADER)) {
        if (checkValidRequestString(CLEAR_CAN_MESSAGE_BY_ID_HEADER, str)) {
            clearCurrentMessageByIdRequest(str.substr(static_cast<std::string>(CLEAR_CAN_MESSAGE_BY_ID_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }   
    } else if (startsWith(str, CURRENT_CAN_MESSAGE_BY_ID_HEADER)) {
        if (checkValidRequestString(CURRENT_CAN_MESSAGE_BY_ID_HEADER, str)) {
            currentCachedCanMessageByIdRequest(str.substr(static_cast<std::string>(CURRENT_CAN_MESSAGE_BY_ID_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, CURRENT_CAN_MESSAGES_HEADER)) {
        currentCachedCanMessagesRequest(coutIndex);
    } else if (startsWith(str, CLEAR_CAN_MESSAGES_HEADER)) {
        clearCanMessagesRequest(coutIndex);
    }   else if (startsWith(str, CURRENT_POSITIVE_CAN_MASKS_HEADER)) {
        currentPositiveCanMasksRequest(coutIndex);
    } else if (startsWith(str, CURRENT_NEGATIVE_CAN_MASKS_HEADER)) {
        currentNegativeCanMasksRequest(coutIndex);
    } else if (startsWith(str, CLEAR_POSITIVE_CAN_MASKS_HEADER)) {
        clearPositiveCanMasksRequest(coutIndex);
    } else if (startsWith(str, CLEAR_NEGATIVE_CAN_MASKS_HEADER)) {
        clearNegativeCanMasksRequest(coutIndex);
    } else if (startsWith(str, CLEAR_ALL_CAN_MASKS_HEADER)) {
        clearAllCanMasksRequest(coutIndex);
#endif
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handleDPrefixedString(const std::string &str, int coutIndex)
{
    if (startsWith(str, DIGITAL_READ_HEADER)) {
        if (checkValidRequestString(DIGITAL_READ_HEADER, str)) {
            digitalReadRequest(str.substr(static_cast<std::string>(DIGITAL_READ_HEADER).length()+1), false, coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, DIGITAL_WRITE_HEADER)) {
        if (checkValidRequestString(DIGITAL_WRITE_HEADER, str)) {
            digitalWriteRequest(str.substr(static_cast<std::string>(DIGITAL_WRITE_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, DIGITAL_WRITE_ALL_HEADER)) {
        if (checkValidRequestString(DIGITAL_WRITE_ALL_HEADER, str)) {
            digitalWriteAllRequest(str.substr(static_cast<std::string>(DIGITAL_WRITE_ALL_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(DIGITAL_WRITE_ALL_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handleHPrefixedString(const std::string &str, int coutIndex)
{
    if (startsWith(str, HEARTBEAT_HEADER)) {
        heartbeatRequest(coutIndex);
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handleIPrefixedString(const std::string &str, int coutIndex)
{
    if (startsWith(str, IO_REPORT_HEADER)) {
        ioReportRequest(coutIndex);
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handleLPrefixedString(const std::string &str, int coutIndex)
{
    if (startsWith(str, LOAD_SYSTEM_STATE_HEADER)) {
        loadSystemStateRequest(coutIndex);
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handlePPrefixedString(const std::string &str, int coutIndex)
{
    if (startsWith(str, PIN_TYPE_HEADER)) {
        if (checkValidRequestString(PIN_TYPE_HEADER, str)) {
            pinTypeRequest(str.substr(static_cast<std::string>(PIN_TYPE_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, PIN_TYPE_CHANGE_HEADER)) {
        if (checkValidRequestString(PIN_TYPE_CHANGE_HEADER, str)) {
            pinTypeChangeRequest(str.substr(static_cast<std::string>(PIN_TYPE_CHANGE_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handleRPrefixedString(const std::string &str, int coutIndex)
{
    if (str.length() == 0) {
        return;
#if defined(__HAVE_CAN_BUS__)
    } else if (startsWith(str, REMOVE_NEGATIVE_CAN_MASK_HEADER)) {
        if (checkValidRequestString(REMOVE_NEGATIVE_CAN_MASK_HEADER, str)) {
            removeNegativeCanMaskRequest(str.substr(static_cast<std::string>(REMOVE_NEGATIVE_CAN_MASK_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, REMOVE_POSITIVE_CAN_MASK_HEADER)) {
        if (checkValidRequestString(REMOVE_POSITIVE_CAN_MASK_HEADER, str)) {
            removePositiveCanMaskRequest(str.substr(static_cast<std::string>(REMOVE_POSITIVE_CAN_MASK_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
#endif
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handleSPrefixedString(const std::string &str, int coutIndex)
{
    if (startsWith(str, SOFT_DIGITAL_READ_HEADER)) {
        if (checkValidRequestString(SOFT_DIGITAL_READ_HEADER, str)) {
            digitalReadRequest(str.substr(static_cast<std::string>(SOFT_DIGITAL_READ_HEADER).length()+1), SOFT, coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, SOFT_ANALOG_READ_HEADER)) {
        if (checkValidRequestString(SOFT_ANALOG_READ_HEADER, str)) {
            softAnalogReadRequest(str.substr(static_cast<std::string>(SOFT_ANALOG_READ_HEADER).length()+1), coutIndex);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
    } else if (startsWith(str, STORE_SYSTEM_STATE_HEADER)) {
        storeSystemStateRequest(coutIndex);
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void handleVPrefixedString(const std::string &str, int coutIndex)
{
    if (startsWith(str, FIRMWARE_VERSION_HEADER)) {
        firmwareVersionRequest(coutIndex);
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE, coutIndex);
    }
}

void changeSerialModeRequest(const std::string &str, int coutIndex)
{
    (void)str;
    (void)coutIndex;
}

void storeSystemStateRequest(int coutIndex)
{
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    int eepromAddress{0};
    unsigned char readPin{0};
    unsigned char readIOType{0};
    int readState{0};
    *outputStream << STORE_SYSTEM_STATE_HEADER << '}';
    for (auto &it : gpioPins) {
        std::string stringToLog{"{"};
        readPin = EEPROM.read(eepromAddress + EEPROMWriteOffset::PIN);
        readIOType = EEPROM.read(eepromAddress+1);
        int currentState{it.second->getIOAgnosticState()};
        std::vector<unsigned char> rawState{GPIO::toEEPROMWritableState(currentState)};
        
        stringToLog += toString(it.first) + ':';
        stringToLog += getIOTypeString(it.second->ioType()) + ':';
        
        for (int i = EEPROMWriteOffset::STATE0; i < EEPROMWriteOffset::STATE4 + 1; i++) {
            readState += static_cast<int>(EEPROM.read(eepromAddress + i));
        }
        if (readPin != static_cast<unsigned char>(it.first)) {
            EEPROM.write(eepromAddress + EEPROMWriteOffset::PIN, static_cast<unsigned char>(it.first));
        }
        if (readIOType != getEEPROMIOTypeFromEnumClass(it.second->ioType())) {
            EEPROM.write(eepromAddress + EEPROMWriteOffset::IO_TYPE, getEEPROMIOTypeFromEnumClass(it.second->ioType()));
        }
        if (currentState != readState) {
            for (int i = EEPROMWriteOffset::STATE0; i < EEPROMWriteOffset::STATE4 + 1; i++) {
                EEPROM.write(eepromAddress + i, rawState.at(i-2));
                stringToLog += toString(static_cast<int>(rawState.at(i-2)));
                if ((i + 1) != (EEPROMWriteOffset::STATE4+1)) {
                    stringToLog += ':';
                } else {
                    stringToLog += '}';
                }
            }
        }
        *outputStream << stringToLog << ';';
        eepromAddress += EEPROM_ADVANCE;
    }
    *outputStream << STORE_SYSTEM_STATE_END_HEADER << '}';
}

void loadSystemStateRequest(int coutIndex)
{
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    int eepromAddress{0};
    unsigned char readPin{0};
    unsigned char readIOType{0};
    int readState{0};
    *outputStream << LOAD_SYSTEM_STATE_HEADER << '}';
    for (auto &it : gpioPins) {
        std::string stringToLog{"{"};
        readPin = EEPROM.read(eepromAddress + EEPROMWriteOffset::PIN);
        readIOType = EEPROM.read(eepromAddress+1);
        int currentState{it.second->getIOAgnosticState()};
        std::vector<unsigned char> rawState{GPIO::toEEPROMWritableState(currentState)};
        
        for (int i = EEPROMWriteOffset::STATE0; i < EEPROMWriteOffset::STATE4 + 1; i++) {
            readState += static_cast<int>(EEPROM.read(eepromAddress + i));
        }

        if (readPin != static_cast<unsigned char>(it.first)) {
            std::cout << "{" << static_cast<int>(readPin) << "!=" << it.first << ":" << OPERATION_FAILURE << ':' << OPERATION_FAILURE << "};";
            continue;
        } else {
            stringToLog += toString(static_cast<int>(readPin)) + ':';
        }

        IOType oldIOType{it.second->ioType()};
        IOType newIOType{getEEPROMIOTypeFromIndex(readIOType)};
        if (newIOType != oldIOType) {
            if (pinInUseBySerialPort(it.first)) {
            *outputStream << "{" << static_cast<int>(readPin) << ":!" << getIOTypeString(newIOType) << "!:" << OPERATION_PIN_USED_BY_SERIAL_PORT << "};";
                continue;
            }
            if (!checkValidIOChangeRequest(newIOType, it.first)) {
            *outputStream << "{" << static_cast<int>(readPin) << ":!" << getIOTypeString(newIOType) << "!:" << OPERATION_FAILURE << "};";
                continue;
            } else {
                it.second->setIOType(newIOType);
                stringToLog += getIOTypeString(oldIOType) + "->" + getIOTypeString(newIOType);
            }
        } else {
            stringToLog += getIOTypeString(oldIOType) + ':';
        }

        if (newIOType == IOType::DIGITAL_OUTPUT) {
            readState = (readState == 0 ? 0 : 1);
            if (readState != currentState) {
                it.second->g_digitalWrite(readState);
                stringToLog += toString(currentState) + "->" + toString(readState);
            } else {
                stringToLog += toString(currentState);
            }
        } else if (newIOType == IOType::ANALOG_OUTPUT) {
            if (readState != currentState) {
                it.second->g_analogWrite(readState);
                stringToLog += toString(currentState) + "->" + toString(readState);
            } else {
                stringToLog += toString(currentState);
            }
        } else {
            if (readState != currentState) {
                stringToLog += toString(currentState) + "<->" + toString(readState);
            } else {
                stringToLog += toString(currentState);
            }
        }
        stringToLog += '}';
        *outputStream << stringToLog << ';';
        eepromAddress += EEPROM_ADVANCE;
    }
    *outputStream << LOAD_SYSTEM_STATE_END_HEADER << '}';
}

void changeAToDThresholdRequest(const std::string &str, int coutIndex)
{
    if (str.length() == 0) {
        printTypeResult(CHANGE_A_TO_D_THRESHOLD_HEADER, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    int maybeState{parseToAnalogState(str)};
    if (maybeState == OPERATION_FAILURE) {
        printTypeResult(CHANGE_A_TO_D_THRESHOLD_HEADER, str, OPERATION_FAILURE, coutIndex);
        return;
    } 
    GPIO::setAnalogToDigitalThreshold(maybeState);
    printTypeResult(CHANGE_A_TO_D_THRESHOLD_HEADER, GPIO::analogToDigitalThreshold(), OPERATION_SUCCESS, coutIndex);
}

void currentAToDThresholdRequest(int coutIndex)
{
    printTypeResult(CURRENT_A_TO_D_THRESHOLD_HEADER, GPIO::analogToDigitalThreshold(), OPERATION_SUCCESS, coutIndex);
}

void ioReportRequest(int coutIndex)
{
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << IO_REPORT_HEADER << '}';
    for (auto &it : gpioPins) {
        GPIO *gpioPin{it.second};
        int state{0};
        if ((gpioPin->ioType() == IOType::DIGITAL_INPUT) || (gpioPin->ioType() == IOType::DIGITAL_INPUT_PULLUP)) {
            state = gpioPin->g_digitalRead();
        } else if (gpioPin->ioType() == IOType::DIGITAL_OUTPUT) {
            state = gpioPin->g_softDigitalRead();
        } else if (gpioPin->ioType() == IOType::ANALOG_INPUT) {
            state = gpioPin->g_analogRead();
        } else if (gpioPin->ioType() == IOType::ANALOG_OUTPUT) {
            state = gpioPin->g_softAnalogRead();
        }
    *outputStream << '{' << gpioPin->pinNumber() << ':' << getIOTypeString(gpioPin->ioType()) << ':' << state << "};";
    }
    *outputStream << IO_REPORT_END_HEADER << '}';
}

void digitalReadRequest(const std::string &str, bool soft, int coutIndex)
{
    if (str.length() == 0) {
        printResult(DIGITAL_READ_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    int pinNumber = parsePin(str);
    if (pinNumber == INVALID_PIN) {
        printResult((soft ? SOFT_DIGITAL_READ_HEADER : DIGITAL_READ_HEADER), str, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    if (pinInUseBySerialPort(pinNumber)) {
        printResult((soft ? SOFT_DIGITAL_READ_HEADER : DIGITAL_READ_HEADER), pinNumber, STATE_FAILURE, OPERATION_PIN_USED_BY_SERIAL_PORT, coutIndex);
        return;
    }
    if (!isValidDigitalInputPin(pinNumber)) {
        printResult((soft ? SOFT_DIGITAL_READ_HEADER : DIGITAL_READ_HEADER), pinNumber, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    bool state{(soft ? gpioPinByPinNumber(pinNumber)->g_softDigitalRead() : gpioPinByPinNumber(pinNumber)->g_digitalRead())};
    printResult((soft ? SOFT_DIGITAL_READ_HEADER : DIGITAL_READ_HEADER), pinNumber, state, OPERATION_SUCCESS, coutIndex);
}

void digitalWriteRequest(const std::string &str, int coutIndex)
{
    if (str.length() == 0) {
        printResult(DIGITAL_WRITE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    size_t foundPosition{str.find(":")};
    std::string maybePin{str.substr(0, foundPosition).c_str()};
    int pinNumber{parsePin(maybePin)};
    if (pinNumber == INVALID_PIN) {
        printResult(DIGITAL_WRITE_HEADER, maybePin, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    if (pinInUseBySerialPort(pinNumber)) {
        printResult(DIGITAL_WRITE_HEADER, pinNumber, STATE_FAILURE, OPERATION_PIN_USED_BY_SERIAL_PORT, coutIndex);
        return;
    }
    if (!isValidDigitalOutputPin(pinNumber)) {
        printResult(DIGITAL_WRITE_HEADER, pinNumber, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    std::string maybeState{str.substr(foundPosition+1).c_str()};
    int state{parseToDigitalState(maybeState)};
    if (state == OPERATION_FAILURE) {
        printResult(DIGITAL_WRITE_HEADER, pinNumber, maybeState, OPERATION_FAILURE, coutIndex);
        return;
    } 
    gpioPinByPinNumber(pinNumber)->g_digitalWrite(state);
    printResult(DIGITAL_WRITE_HEADER, pinNumber, maybeState, OPERATION_SUCCESS, coutIndex);
}

void digitalWriteAllRequest(const std::string &str, int coutIndex)
{
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << DIGITAL_WRITE_ALL_HEADER;
    if (str.length() == 0) {
        printResult(DIGITAL_WRITE_ALL_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    size_t foundPosition{str.find(":")};
    std::string maybeState{str.substr(0, foundPosition).c_str()};
    int state{parseToDigitalState(maybeState)};
    if (state == OPERATION_FAILURE) {
        printTypeResult(DIGITAL_WRITE_ALL_HEADER, maybeState, OPERATION_FAILURE, coutIndex);
        return;
    }
    for (auto &it : gpioPins) {
        GPIO *gpioPin{it.second};
        if (gpioPin->ioType() == IOType::DIGITAL_OUTPUT) {
            gpioPin->g_digitalWrite(state);
        *outputStream << ':' << gpioPin->pinNumber();
        }
    }
    *outputStream << ':' << state << ':' << OPERATION_SUCCESS << '}';
}

void analogReadRequest(const std::string &str, int coutIndex)
{
    if (str.length() == 0) {
        printResult(ANALOG_READ_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    int pinNumber{parsePin(str)};
    if (pinNumber == INVALID_PIN) {
        printResult(ANALOG_READ_HEADER, str, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    if (pinInUseBySerialPort(pinNumber)) {
        printResult(ANALOG_READ_HEADER, pinNumber, STATE_FAILURE, OPERATION_PIN_USED_BY_SERIAL_PORT, coutIndex);
        return;
    }
    if (!isValidAnalogInputPin(pinNumber)) {
        printResult(ANALOG_READ_HEADER, pinNumber, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    printResult(ANALOG_READ_HEADER, pinNumber, gpioPinByPinNumber(pinNumber)->g_analogRead(), OPERATION_SUCCESS, coutIndex);
}

void analogWriteRequest(const std::string &str, int coutIndex)
{
    if (str.length() == 0) {
        printResult(ANALOG_WRITE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    size_t foundPosition{str.find(":")};
    std::string maybePin{str.substr(0, foundPosition).c_str()};
    int pinNumber{parsePin(maybePin)};

    if (pinNumber == INVALID_PIN) {
        printResult(ANALOG_WRITE_HEADER, maybePin, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    if (pinInUseBySerialPort(pinNumber)) {
        printResult(ANALOG_WRITE_HEADER, pinNumber, STATE_FAILURE, OPERATION_PIN_USED_BY_SERIAL_PORT, coutIndex);
        return;
    }
    if (!isValidAnalogOutputPin(pinNumber)) {
        printResult(ANALOG_WRITE_HEADER, pinNumber, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    std::string maybeState{str.substr(foundPosition+1).c_str()};
    int state{parseToAnalogState(maybeState)};
    if (state == OPERATION_FAILURE) {
        printResult(ANALOG_WRITE_HEADER, pinNumber, maybeState, OPERATION_FAILURE, coutIndex);
    } else {
        gpioPinByPinNumber(pinNumber)->g_analogWrite(state);
        printResult(ANALOG_WRITE_HEADER, pinNumber, state, OPERATION_SUCCESS, coutIndex);
    }
}

void pinTypeRequest(const std::string &str, int coutIndex)
{
    if (str.length() == 0) {
        printResult(PIN_TYPE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    int pinNumber = parsePin(str);
    if (pinNumber == INVALID_PIN) {
        printResult(PIN_TYPE_HEADER, str, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    printResult(PIN_TYPE_HEADER, pinNumber, getIOTypeString(gpioPinByPinNumber(pinNumber)->ioType()), OPERATION_SUCCESS, coutIndex);
}

void pinTypeChangeRequest(const std::string &str, int coutIndex)
{   
    if (str.length() == 0) {
        printResult(PIN_TYPE_CHANGE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    size_t foundPosition{str.find(":")};
    
    std::string maybePin{str.substr(0, foundPosition)};
    int pinNumber{parsePin(maybePin)};

    if (pinNumber == INVALID_PIN) {
        printResult(PIN_TYPE_CHANGE_HEADER, maybePin, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }

    std::string maybeType{str.substr(foundPosition+1)};
    IOType type{parseIOType(maybeType)};
    if (type == IOType::UNSPECIFIED) {
        printResult(PIN_TYPE_CHANGE_HEADER, pinNumber, maybeType, OPERATION_FAILURE, coutIndex);
        return;
    }
    if (pinInUseBySerialPort(pinNumber)) {
        printResult(DIGITAL_WRITE_HEADER, pinNumber, getIOTypeString(type), OPERATION_PIN_USED_BY_SERIAL_PORT, coutIndex);
        return;
    }
    if (!checkValidIOChangeRequest(type, pinNumber)) {
        printResult(PIN_TYPE_CHANGE_HEADER, pinNumber, getIOTypeString(type), OPERATION_FAILURE, coutIndex);
        return;
    }
    gpioPinByPinNumber(pinNumber)->setIOType(type);
    printResult(PIN_TYPE_CHANGE_HEADER, pinNumber, getIOTypeString(type), OPERATION_SUCCESS, coutIndex);
}

void softAnalogReadRequest(const std::string &str, int coutIndex)
{
    if (str.length() == 0) {
        printResult(SOFT_ANALOG_READ_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    int pinNumber{parsePin(str)};
    if (pinNumber == INVALID_PIN) {
        printResult(SOFT_ANALOG_READ_HEADER, str, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    if (!isValidAnalogOutputPin(pinNumber)) {
        printResult(SOFT_ANALOG_READ_HEADER, pinNumber, STATE_FAILURE, OPERATION_FAILURE, coutIndex);
        return;
    }
    int state{gpioPinByPinNumber(pinNumber)->g_softAnalogRead()};
    printResult(SOFT_ANALOG_READ_HEADER, pinNumber, state, OPERATION_SUCCESS, coutIndex);
}

void heartbeatRequest(int coutIndex)
{
    printString(static_cast<std::string>(HEARTBEAT_HEADER) + "}", coutIndex);
}

void arduinoTypeRequest(int coutIndex)
{
    printTypeResult(ARDUINO_TYPE_HEADER, ARDUINO_TYPE, OPERATION_SUCCESS, coutIndex);
}

void firmwareVersionRequest(int coutIndex)
{
    printTypeResult(FIRMWARE_VERSION_HEADER, FIRMWARE_VERSION, OPERATION_SUCCESS, coutIndex);
}

void canBusEnabledRequest(int coutIndex)
{
    #if defined(__HAVE_CAN_BUS__)
        printTypeResult(CAN_BUS_ENABLED_HEADER, 1, OPERATION_SUCCESS, coutIndex);
    #else
        printTypeResult(CAN_BUS_ENABLED_HEADER, 0, OPERATION_SUCCESS, coutIndex);
    #endif
} 

void populateGpioMap()
{
    int i{0};
    while (AVAILABLE_PWM_PINS[i] > 0) {
        gpioPins.insert(std::pair<int, GPIO*>(AVAILABLE_PWM_PINS[i], new GPIO(AVAILABLE_PWM_PINS[i], IOType::DIGITAL_INPUT_PULLUP)));
        i++;
    }
    i = 0;
    while (AVAILABLE_ANALOG_PINS[i] > 0) {
        gpioPins.insert(std::pair<int, GPIO*>(AVAILABLE_ANALOG_PINS[i], new GPIO(AVAILABLE_ANALOG_PINS[i], IOType::ANALOG_INPUT)));
        i++;
    }
    i = 0;
    while (AVAILABLE_GENERAL_PINS[i] > 0) {
        gpioPins.insert(std::pair<int, GPIO*>(AVAILABLE_GENERAL_PINS[i], new GPIO(AVAILABLE_GENERAL_PINS[i], IOType::DIGITAL_INPUT_PULLUP)));
        i++;
    }
}

bool checkValidRequestString(const char *header, const std::string &checkStr)
{
    return (checkStr.length() > (static_cast<std::string>(header).length() + 1));
}


bool isValidDigitalOutputPin(int pinNumber)
{
#if defined(ARDUINO_AVR_NANO)
    if ((pinNumber == A6) || (pinNumber == A7)) {
        return false;
    }
#endif
    for (auto &it : gpioPins) {
        if (pinNumber == it.first) {
            return true;
        }
    }
    return false;
}

bool isValidDigitalInputPin(int pinNumber)
{
#if defined(ARDUINO_AVR_NANO)
    if ((pinNumber == A6) || (pinNumber == A7)) {
        return false;
    }
#endif
    for (auto &it : gpioPins) {
        if (pinNumber == it.first) {
            return true;
        }
    }
    return false;
}

bool isValidAnalogOutputPin(int pinNumber)
{
    int i{0};
    while (AVAILABLE_PWM_PINS[i] > 0) {
        if (pinNumber == AVAILABLE_PWM_PINS[i]) { 
            return true;
        }
        i++;
    }
    return false;
}

bool isValidAnalogInputPin(int pinNumber)
{
    int i{0};
    while (AVAILABLE_ANALOG_PINS[i] > 0) {
        if (pinNumber == AVAILABLE_ANALOG_PINS[i]) {
            return true;
        }
        i++;
    }
    return false;
}

bool checkValidIOChangeRequest(IOType ioType, int pinNumber)
{
    if ((ioType == IOType::DIGITAL_INPUT) || (ioType == IOType::DIGITAL_INPUT_PULLUP)) {
        return isValidDigitalInputPin(pinNumber);
    } else if (ioType == IOType::DIGITAL_OUTPUT) {
        return isValidDigitalOutputPin(pinNumber);
    } else if (ioType == IOType::ANALOG_INPUT) {
        return isValidAnalogInputPin(pinNumber);
    } else if (ioType == IOType::ANALOG_OUTPUT) {
        return isValidAnalogOutputPin(pinNumber);
    } else {
        return false;
    }
}

std::string getIOTypeString(IOType ioType)
{
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
        return UNSPECIFIED_IO_TYPE_IDENTIFIER;
    }
}

int parsePin(const std::string &str)
{
    if (isValidAnalogPinIdentifier(str)) {
        return parseAnalogPin(str);
    } else if (isValidPinIdentifier(str)) {
        return atoi(str.c_str());
    } else {
        return INVALID_PIN;
    }
}

IOType parseIOType(const std::string &str)
{
    if (str == DIGITAL_INPUT_IDENTIFIER) {
        return IOType::DIGITAL_INPUT;
    } else if (str == DIGITAL_OUTPUT_IDENTIFIER) {
        return IOType::DIGITAL_OUTPUT;
    } else if (str == ANALOG_INPUT_IDENTIFIER) {
        return IOType::ANALOG_INPUT;
    } else if (str == ANALOG_OUTPUT_IDENTIFIER) {
        return IOType::ANALOG_OUTPUT;
    } else if (str == DIGITAL_INPUT_PULLUP_IDENTIFIER) {
        return IOType::DIGITAL_INPUT_PULLUP;
    } else {
        return IOType::UNSPECIFIED;
    }
}

bool isValidAnalogStateIdentifier(const std::string &str)
{
    if ((str == "0") || (str == "00") || (str == "000")) {
        return true;
    }
    int returnVal{atoi(str.c_str())};
    return (returnVal != 0); 
}

bool isValidDigitalStateIdentifier(const std::string &str)
{
    return ((str == "1") || (str == "true") || (str == "high") ||
            (str == "0") || (str == "false") || (str == "low"));
}

int parseToDigitalState(const std::string &str)
{
    if ((str == "1") || (str == "true") || (str == "high")) {
        return HIGH; 
    } else if ((str == "0") || (str == "false") || (str == "low")) {
        return LOW;
    } else {
        return OPERATION_FAILURE;
    }
}

int parseToAnalogState(const std::string &str)
{
    if (!isValidAnalogStateIdentifier(str)) {
        return OPERATION_FAILURE;
    } else {
        int temp{atoi(str.c_str())};
        if (temp > GPIO::ANALOG_MAX) {
            temp = GPIO::ANALOG_MAX;
        }
        if (temp < 0) {
            return OPERATION_FAILURE;
        }
        return temp;
    }
}

bool isValidPinIdentifier(const std::string &str)
{
    for (auto &it : gpioPins) {
        if (str == toDecString(it.first)) {
            return true;
        }
    }
    for (int i = 0; i < NUMBER_OF_ANALOG_PINS; i++) {
        if (toUppercase(str) == ("A" + toString(i))) {
            return true;
        }
    }
    return false;
}

bool isValidPwmPinIdentifier(const std::string &str)
{
    int i{0};
    while (AVAILABLE_PWM_PINS[i] > 0) {
        if (str == toString(AVAILABLE_PWM_PINS[i])) {
            return true;
        }
        i++;
    }
    return false;
}

bool isValidAnalogPinIdentifier(const std::string &str)
{
    int i{0};
    while (AVAILABLE_ANALOG_PINS[i] > 0) {
        if (str == toString(AVAILABLE_ANALOG_PINS[i])) {
            return true;
        }
        i++;
    }
    for (i = 0; i < NUMBER_OF_ANALOG_PINS; i++) {
        if (toUppercase(str) == ("A" + toString(i))) {
            return true;
        }
    }
    return false;
}

bool isValidPinTypeIdentifier(const std::string &str)
{
    std::string copyString{toLowercase(str)};
    return ((copyString == static_cast<std::string>(DIGITAL_INPUT_IDENTIFIER)) ||
            (copyString == static_cast<std::string>(DIGITAL_OUTPUT_IDENTIFIER)) ||
            (copyString == static_cast<std::string>(ANALOG_INPUT_IDENTIFIER)) ||
            (copyString == static_cast<std::string>(ANALOG_OUTPUT_IDENTIFIER)) ||
            (copyString == static_cast<std::string>(DIGITAL_INPUT_PULLUP_IDENTIFIER)));
}

void initializeSerialPorts()
{
    for (auto &it : hardwareSerialPorts) {
        if (it->isEnabled()) {
            (void)it->initialize();
        }
    }
}

GPIO *gpioPinByPinNumber(int pinNumber)
{
    auto result = gpioPins.find(pinNumber);
    if (result == gpioPins.end()) {
        return nullptr;
    } else {
        return result->second;
    }
}

IOType getEEPROMIOTypeFromIndex(unsigned char index)
{
    if (index == 0) {
        return IOType::DIGITAL_INPUT;
    } else if (index == 1) {
        return IOType::DIGITAL_OUTPUT;
    } else if (index == 2) {
        return IOType::ANALOG_INPUT;
    } else if (index == 3) {
        return IOType::ANALOG_OUTPUT;
    } else if (index == 4) {
        return IOType::DIGITAL_INPUT_PULLUP;
    } else {
        return IOType::UNSPECIFIED;
    }
}

unsigned char getEEPROMIOTypeFromEnumClass(IOType ioType)
{
    if (ioType == IOType::DIGITAL_INPUT) {
        return 0;
    } else if (ioType == IOType::DIGITAL_OUTPUT) {
        return 1;
    } else if (ioType == IOType::ANALOG_INPUT) {
        return 2;
    } else if (ioType == IOType::ANALOG_OUTPUT) {
        return 3;
    } else if (ioType == IOType::DIGITAL_INPUT_PULLUP) {
        return 4;
    } else {
        return 5;
    }
}

int parseAnalogPin(const std::string &pinAlias)
{
    for (int i = 0; i < NUMBER_OF_ANALOG_PINS; i++) {
        if (toUppercase(pinAlias) == ("A" + toString(i))) {
            return ANALOG_PIN_OFFSET + i;
        }
    }
    int i{0};
    while (AVAILABLE_ANALOG_PINS[i] > 0) {
        if (pinAlias == toString(AVAILABLE_ANALOG_PINS[i])) {
            return AVAILABLE_ANALOG_PINS[i];
        }
        i++;
    }  
    return 0;
}

std::string analogPinFromNumber(int pinNumber)
{
    int i{0};
    while (AVAILABLE_ANALOG_PINS[i] > 0) {
        if (pinNumber == AVAILABLE_ANALOG_PINS[i]) {
            return "A" + toString(i);
        }
        i++;
    }  
    return "";
}

int nextAvailableSerialSlotNumber()
{
    if (softwareSerialPorts.size() == MAXIMUM_SERIAL_PORTS) {
        return NEXT_SERIAL_PORT_UNAVAILABLE;
    } else {
        return softwareSerialPorts.size();
    }
}

bool pinInUseBySerialPort(int pinNumber)
{
    for (auto &it : hardwareSerialPorts) {
        if (it->isEnabled()) {
            if ((pinNumber == it->rxPin()) || (pinNumber == it->txPin())) {
                return true;
            }
        }
    }
    for (auto &it : softwareSerialPorts) {
        if (it->isEnabled()) {
            if ((pinNumber == it->rxPin()) || (pinNumber == it->txPin())) {
                return true;
            }
        }
    }
    return false;
}

void printString(const std::string &str, int coutIndex) 
{
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << str; 
}

void printStringWithNewline(const std::string &str, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << str << std::endl; 
}

void printResult(const std::string &header, const std::string &pinNumber, bool state, int resultCode, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; 
}

void printResult(const std::string &header, int pinNumber, bool state, int resultCode, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; 
}

void printResult(const std::string &header, const std::string &pinNumber, int state, int resultCode, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    *outputStream << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; 
}

void printResult(const std::string &header, int pinNumber, int state, int resultCode, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; 
}

void printResult(const std::string &header, const std::string &pinNumber, const std::string &state, int resultCode, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; 
}

void printResult(const std::string &header, int pinNumber, const std::string &state, int resultCode, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; 
}

void printSingleResult(const std::string &header, int resultCode, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << header << ':' << resultCode << '}'; 
}

void printTypeResult(const std::string &header, const std::string &type, int resultCode, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << header << ':' << type << ':' << resultCode << '}'; 
}

void printTypeResult(const std::string &header, int type, int resultCode, int coutIndex) 
{ 
    std::ohserialstream *outputStream{nullptr};
    if (isSoftwareCoutStream(coutIndex)) {
        outputStream = getSoftwareCout(coutIndex);
    } else {
        outputStream = getHardwareCout(coutIndex);
    }
    if (!outputStream) {
        outputStream = &DEFAULT_NATIVE_PORT;
    }
    *outputStream << header << ':' << type << ':' << resultCode << '}'; 
}

std::ohserialstream *getHardwareCout(int coutIndex)
{
    if (coutIndex < 0) {
        return bitBucket;
    }
    if (static_cast<unsigned int>(coutIndex) > hardwareSerialPorts.size()-1) {
        return bitBucket;
    } else {
        if (hardwareSerialPorts.at(coutIndex)->isEnabled()) {
            return hardwareSerialPorts.at(coutIndex)->outputStream();
        } else {
            return bitBucket;
        }
    }
}

std::ohserialstream *getSoftwareCout(int coutIndex)
{
    if (coutIndex < 0) {
        return bitBucket;
    }
    if (static_cast<unsigned int>(coutIndex) > softwareSerialPorts.size()-1) {
        return bitBucket;
    } else {
        if (softwareSerialPorts.at(coutIndex)->isEnabled()) {
            return softwareSerialPorts.at(coutIndex)->outputStream();
        } else {
            return bitBucket;
        }
    }
}

bool isSoftwareCoutStream(int coutIndex)
{
    if ((coutIndex == SerialIndex::SOFTWARE_PORT_0) ||
        (coutIndex == SerialIndex::SOFTWARE_PORT_1) ||
        (coutIndex == SerialIndex::SOFTWARE_PORT_2) ||
        (coutIndex == SerialIndex::SOFTWARE_PORT_3)) {
        
        return true;
    } else { 
        return false;
    }
}

#if defined(__HAVE_CAN_BUS__)
    void printCanResult(const std::string &header, const std::string &str, int resultCode, int coutIndex) 
    { 
        std::ohserialstream *outputStream{nullptr};
        if (isSoftwareCoutStream(coutIndex)) {
            outputStream = getSoftwareCout(coutIndex);
        } else {
            outputStream = getHardwareCout(coutIndex);
        }
        if (!outputStream) {
            outputStream = &DEFAULT_NATIVE_PORT;
        }
        *outputStream << header << ':' << str << ':' << resultCode << '}'; 
    }
    
    void printCanResult(const std::string &header, const CanMessage &msg, int resultCode, int coutIndex) 
    { 
        std::ohserialstream *outputStream{nullptr};
        if (isSoftwareCoutStream(coutIndex)) {
            outputStream = getSoftwareCout(coutIndex);
        } else {
            outputStream = getHardwareCout(coutIndex);
        }
        if (!outputStream) {
            outputStream = &DEFAULT_NATIVE_PORT;
        }
        *outputStream << header << ':' << msg.toString(, int coutIndex) << ':' << resultCode << '}'; 
    }
    
    void printBlankCanResult(const std::string &header, int resultCode, int coutIndex) 
    { 
        std::ohserialstream *outputStream{nullptr};
        if (isSoftwareCoutStream(coutIndex)) {
            outputStream = getSoftwareCout(coutIndex);
        } else {
            outputStream = getHardwareCout(coutIndex);
        }
        if (!outputStream) {
            outputStream = &DEFAULT_NATIVE_PORT;
        }
        *outputStream << header << ':' << resultCode << '}'; 
    } 

    void canInit()
    {
        using namespace ArduinoPCStrings;
        unsigned int startTime = millis();
        unsigned int endTime = millis();
        if (canController) {
            while (canController->begin(CAN_500KBPS) != CAN_OK) {
                endTime = millis();
                if ((endTime - startTime) >= CAN_CONNECTION_TIMEOUT) {
                    return;
                }
            }
            canBusInitialized = true;
        }
    }

    void canInitRequest(int coutIndex)
    {
        using namespace ArduinoPCStrings;
        if (canBusInitialized) {
            printSingleResult(CAN_INIT_HEADER, OPERATION_SUCCESS, coutIndex);
            return;
        } else {
            canInit();
        }
        if (canBusInitialized) {
            printSingleResult(CAN_INIT_HEADER, OPERATION_SUCCESS, coutIndex);
        } else {
            printSingleResult(CAN_INIT_HEADER, OPERATION_FAILURE, coutIndex);
        }
        
    }

    void canReadRequest(bool autoUp, int coutIndex)
    {
        using namespace ArduinoPCStrings;
        if (!canBusInitialized) {
            canInit();
        }
        unsigned char receivedPacketLength{0};
        unsigned char pack[8]{0, 0, 0, 0, 0, 0, 0, 0};
        uint32_t canID{0}; 
    
        if (CAN_MSGAVAIL == canController->checkReceive()) {
            canController->readMsgBuf(&receivedPacketLength, pack);
            canID = canController->getCanId();
            if ((positiveCanMasks.size() == 0) && (negativeCanMasks.size() == 0)) {
                CanMessage msg{canID, CAN_FRAME, receivedPacketLength, CanDataPacket{pack[0], pack[1], pack[2], pack[3], pack[4], pack[5], pack[6], pack[7]}};
                printCanResult(CAN_READ_HEADER, msg.toString(), OPERATION_SUCCESS, coutIndex);
            } else {
                if (positiveCanMasks.size() == 0) {
                    if (negativeCanMasks.find(canID) == negativeCanMasks.end()) {
                        CanMessage msg{canID, CAN_FRAME, receivedPacketLength, CanDataPacket{pack[0], pack[1], pack[2], pack[3], pack[4], pack[5], pack[6], pack[7]}};
                        printCanResult(CAN_READ_HEADER, msg.toString(), OPERATION_SUCCESS, coutIndex);
                    }
                }
                if (negativeCanMasks.size() == 0) {
                    if (positiveCanMasks.find(canID) != positiveCanMasks.end()) {
                        CanMessage msg{canID, CAN_FRAME, receivedPacketLength, CanDataPacket{pack[0], pack[1], pack[2], pack[3], pack[4], pack[5], pack[6], pack[7]}};
                        printCanResult(CAN_READ_HEADER, msg.toString(), OPERATION_SUCCESS, coutIndex);
                    }
                }
                if (positiveCanMasks.find(canID) != positiveCanMasks.end()) {
                    if (negativeCanMasks.find(canID) == negativeCanMasks.end()) {
                        CanMessage msg{canID, CAN_FRAME, receivedPacketLength, CanDataPacket{pack[0], pack[1], pack[2], pack[3], pack[4], pack[5], pack[6], pack[7]}};
                        printCanResult(CAN_READ_HEADER, msg.toString(), OPERATION_SUCCESS, coutIndex);
                    }
                }
            }
        } else if (!autoUp) {
            printBlankCanResult(CAN_READ_HEADER, OPERATION_SUCCESS, coutIndex);
        }
    }

    void canWriteRequest(const std::string &str, bool once, int coutIndex)
    {
        using namespace ArduinoPCStrings;
        if (!canBusInitialized) {
            canInit();
        }
        CanMessage msg{parseCanMessage(str)};
        if (msg.toString() == "") {
            printSingleResult(CAN_WRITE_HEADER, OPERATION_FAILURE, coutIndex);
            return;
        }
        if (!once) {
            addLastCanMessage(msg);
        }
        sendCanMessage(msg);
        printCanResult(CAN_WRITE_HEADER, msg.toString(), OPERATION_SUCCESS, coutIndex);
    }

    void addPositiveCanMaskRequest(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(ADD_POSITIVE_CAN_MASK_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
        if ((positiveCanMasks.insert(maybeID)).second) {
            printTypeResult(ADD_POSITIVE_CAN_MASK_HEADER, str, OPERATION_SUCCESS, coutIndex);
        } else {
            printTypeResult(ADD_POSITIVE_CAN_MASK_HEADER, str, OPERATION_KIND_OF_SUCCESS, coutIndex);
        }
    }

    void removePositiveCanMaskRequest(const std::string &str, int coutIndex)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(REMOVE_POSITIVE_CAN_MASK_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
        positiveCanMasks.erase(maybeID);
        printTypeResult(REMOVE_POSITIVE_CAN_MASK_HEADER, str, OPERATION_SUCCESS, coutIndex);
    }

    void addNegativeCanMaskRequest(const std::string &str, int coutIndex)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(ADD_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
        if ((negativeCanMasks.insert(maybeID)).second) {
            printTypeResult(ADD_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_SUCCESS, coutIndex);
        } else {
            printTypeResult(ADD_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_KIND_OF_SUCCESS, coutIndex);
        }
    }

    void removeNegativeCanMaskRequest(const std::string &str, int coutIndex)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(REMOVE_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
        negativeCanMasks.erase(maybeID);
        printTypeResult(REMOVE_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_SUCCESS, coutIndex);
    }

    void canLiveUpdateRequest(const std::string &str, int coutIndex)
    {
        using namespace ArduinoPCStrings;
        if (str == "") {
            printTypeResult(CAN_LIVE_UPDATE_HEADER, OPERATION_FAILURE, OPERATION_FAILURE, coutIndex);
            return;
        }
        std::pair<int, std::string> canState{parseToCanState(str)};
        if (canState.first == OPERATION_FAILURE) {
            printTypeResult(CAN_LIVE_UPDATE_HEADER, canState.second, OPERATION_FAILURE, coutIndex);
        } else {
            canLiveUpdate = canState.first;
            printTypeResult(CAN_LIVE_UPDATE_HEADER, canState.second, OPERATION_SUCCESS, coutIndex);
        }
    }

    void currentCachedCanMessageByIdRequest(const std::string &str, int coutIndex)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(CURRENT_CAN_MESSAGE_BY_ID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
        if (lastCanMessages.find(maybeID) == lastCanMessages.end()) {
            printBlankCanResult(CURRENT_CAN_MESSAGE_BY_ID_HEADER, OPERATION_SUCCESS, coutIndex);
        } else {
            printCanResult(CURRENT_CAN_MESSAGE_BY_ID_HEADER, lastCanMessages.find(maybeID)->second, OPERATION_SUCCESS, coutIndex);
        }
    }

    void clearCurrentMessageByIdRequest(const std::string &str, int coutIndex)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(CLEAR_CAN_MESSAGE_BY_ID_HEADER, str, OPERATION_FAILURE, coutIndex);
        }
        lastCanMessages.erase(maybeID);
        printTypeResult(CLEAR_CAN_MESSAGE_BY_ID_HEADER, str, OPERATION_SUCCESS, coutIndex);
    }

    void currentCachedCanMessagesRequest()
    {
        for (auto &it : lastCanMessages) {
            printCanResult(CURRENT_CAN_MESSAGES_HEADER, it.second, OPERATION_SUCCESS, coutIndex);
        }
    }
        
    void clearCanMessagesRequest(int coutIndex)
    {
        lastCanMessages.clear();
        printSingleResult(CLEAR_CAN_MESSAGES_HEADER, OPERATION_SUCCESS, coutIndex);
    }

    void currentPositiveCanMasksRequest(int coutIndex)
    {
        for (auto &it :positiveCanMasks) {
            printTypeResult(CURRENT_POSITIVE_CAN_MASKS_HEADER, it, OPERATION_SUCCESS, coutIndex);
        }
    }

    void currentNegativeCanMasksRequest(int coutIndex)
    {
        for (auto &it : negativeCanMasks) {
            printTypeResult(CURRENT_NEGATIVE_CAN_MASKS_HEADER, it, OPERATION_SUCCESS, coutIndex);
        }
    }

    void allCurrentCanMasksRequest(int coutIndex)
    {
        currentPositiveCanMasksRequest(coutIndex);
        currentNegativeCanMasksRequest(coutIndex);
    }

    void clearPositiveCanMasksRequest(int coutIndex)
    {
        positiveCanMasks.clear();
        printSingleResult(CLEAR_POSITIVE_CAN_MASKS_HEADER, OPERATION_SUCCESS, coutIndex);
    }

    void clearNegativeCanMasksRequest(int coutIndex)
    {
        negativeCanMasks.clear();
        printSingleResult(CLEAR_NEGATIVE_CAN_MASKS_HEADER, OPERATION_SUCCESS, coutIndex);
    }

    void clearAllCanMasksRequest(int coutIndex)
    {
        positiveCanMasks.clear();
        negativeCanMasks.clear();
        printSingleResult(CLEAR_ALL_CAN_MASKS_HEADER, OPERATION_SUCCESS, coutIndex);
    }

    void addLastCanMessage(const CanMessage &msg)
    {
        std::map<uint32_t, CanMessage>::iterator iter{lastCanMessages.find(msg.id())};
        if (iter != lastCanMessages.end()) {
            iter->second = msg;
        } else {
            lastCanMessages.insert(std::pair<uint32_t, CanMessage>(msg.id(), msg));
        }
    }
    CanMessage parseCanMessage(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        std::vector<std::string> rawMsg{parseToVector(str, ':')};
        if (rawMsg.size() != CAN_WRITE_REQUEST_SIZE) {
            return CanMessage{};
        }
        CanMessage returnMessage;
        int i{0};
        returnMessage.setFrame(CAN_FRAME);
        returnMessage.setLength(CAN_MESSAGE_LENGTH);
        for (auto &it : rawMsg) {
            if (i++ == 0) {
                returnMessage.setID(CanMessage::parseCanID(it));
            } else {
                returnMessage.setDataPacketNthByte(i-2, CanMessage::parseCanByte(it));
            }
        }
        return returnMessage;
    }

    std::pair<int, std::string> parseToCanState(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        int i{0};
        while (static_cast<std::string>(DIGITAL_STATE_HIGH_IDENTIFIERS[i]) != static_cast<std::string>(CHAR_ARRAY_TERMINATOR)) {
            if (toLowercase(str) == static_cast<std::string>(DIGITAL_STATE_HIGH_IDENTIFIERS[i++])) {
                return std::pair<int, std::string>(HIGH, DIGITAL_STATE_HIGH_STRING); 
            }
        }
        i = 0;
        while (static_cast<std::string>(DIGITAL_STATE_LOW_IDENTIFIERS[i]) != static_cast<std::string>(CHAR_ARRAY_TERMINATOR)) {
            if (toLowercase(str) == static_cast<std::string>(DIGITAL_STATE_LOW_IDENTIFIERS[i++])) {
                return std::pair<int, std::string>(LOW, DIGITAL_STATE_LOW_STRING);
            }
        }
        return std::pair<int, std::string>(OPERATION_FAILURE, str);
    }

    void sendCanMessage(const CanMessage &msg)
    {
        unsigned char send[8]{0, 0, 0, 0, 0, 0, 0, 0};
        msg.dataPacket().toBasicArray(send);
        canController->sendMsgBuf(msg.id(), msg.frame(), msg.length(), send);
    }

#endif
