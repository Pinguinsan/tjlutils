#include <Arduino.h>
#include "include/arduinoinit.h"
#include "include/gpio.h"
#include "include/shortwatchdog.h"
#include "include/firmwareutilities.h"
#include "include/arduinopcstrings.h"

/* 
 * Standard C++ headers
 * Special thanks to maniacbug for the
 * nice port of the c++ stdlib 
 * https://github.com/maniacbug/StandardCplusplus
 */
#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#if defined(__HAVE_CAN_BUS__)
    #include "include/mcp_can.h"
    #include "include/candatapacket.h"
    #include "include/canmessage.h"
#endif

using namespace ArduinoPCStrings;
using namespace FirmwareUtilities;

namespace std { ohserialstream cout(Serial); }
static const int STDOUT_PRECISION{2};
static const int ID_WIDTH{3};
static const int MESSAGE_WIDTH{2};
static const int MAXIMUM_SERIAL_READ_SIZE{175};
static const int PIN_OFFSET{2};
static const long long int SERIAL_BAUD{115200L};
static const int SERIAL_TIMEOUT{1000};
static const int OPERATION_FAILURE{-1};
static const int INVALID_PIN{-1};
static const int STATE_FAILURE{-1};
static const int OPERATION_SUCCESS{1};
static const int OPERATION_KIND_OF_SUCCESS{2};
static const int PIN_PLACEHOLDER{1};
static const bool SOFT{true};
void printString(const std::string &str) { std::cout << str; }
void printStringWithNewline(const std::string &str) { std::cout << str << std::endl; }
void printResult(const std::string &header, const std::string &pinNumber, bool state, int resultCode) { std::cout << std::setprecision(STDOUT_PRECISION) << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; }
void printResult(const std::string &header, int pinNumber, bool state, int resultCode) { std::cout << std::setprecision(STDOUT_PRECISION) << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; }
void printResult(const std::string &header, const std::string &pinNumber, int state, int resultCode) { std::cout << std::setprecision(STDOUT_PRECISION) << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; }
void printResult(const std::string &header, int pinNumber, int state, int resultCode) { std::cout << std::setprecision(STDOUT_PRECISION) << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; }
void printResult(const std::string &header, const std::string &pinNumber, const std::string &state, int resultCode) { std::cout << std::setprecision(STDOUT_PRECISION) << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; }
void printResult(const std::string &header, int pinNumber, const std::string &state, int resultCode) { std::cout << std::setprecision(STDOUT_PRECISION) << header << ':' << pinNumber << ':' << state << ':' << resultCode << '}'; }
void printSingleResult(const std::string &header, int resultCode) { std::cout << header << ':' << resultCode << '}'; }
void printTypeResult(const std::string &header, const std::string &type, int resultCode) { std::cout << header << ':' << type << ':' << resultCode << '}'; }
void printTypeResult(const std::string &header, int type, int resultCode) { std::cout << header << ':' << type << ':' << resultCode << '}'; }
#if defined(__HAVE_CAN_BUS__)
    void printCanResult(const std::string &header, const std::string &str, int resultCode) { std::cout << header << ':' << str << ':' << resultCode << '}'; }
    void printCanResult(const std::string &header, const CanMessage &msg, int resultCode) { std::cout << header << ':' << msg.toString() << ':' << resultCode << '}'; }

    void printBlankCanResult(const std::string &header, int resultCode) { std::cout << header << ':' << resultCode << '}'; } 
#endif

void printWatchdogShortEventResult(const std::string &header, const ShortWatchdog &shortWatchdog)
{
    std::cout << header;
    for (auto &it : shortWatchdog.shorts()) {
        std::cout << ':' << it.first->pinNumber() << ':' << it.second;
    }
    std::cout << '}';
}

void printWatchdogResult(const std::string &header, const ShortWatchdog &shortWatchdog, int resultCode)
{
    std::cout << header;
    for (auto &it : shortWatchdog.shorts()) {
        std::cout << ':' << it.first->pinNumber() << ':' << it.second;
    }
    if (shortWatchdog.failSafes().size() != 0) {
        std::cout << FAILSAFE_INTERIM_HEADER;
        int i{0};
        for (auto &it : shortWatchdog.failSafes()) {
            if (i++ != 0) {
                std::cout << ':';
            }
            std::cout << it.gpio()->pinNumber() << ':' << it.state();
        }
    }
    std::cout << ':' << resultCode << '}';
}

void handleSerialString(const std::string &str);
void handleSerialString(const char *str);
void digitalReadRequest(const std::string &str, bool soft = false);
void digitalWriteRequest(const std::string &str);
void analogReadRequest(const std::string &str);
void analogWriteRequest(const std::string &str);
void softAnalogReadRequest(const std::string &str);

void pinTypeRequest(const std::string &str);
void pinTypeChangeRequest(const std::string &str);
void addShortWatchdogRequest(const std::string &str);
void removeShortWatchdogRequest(const std::string &str);

void changeAToDThresholdRequest(const std::string &str);
void currentAToDThresholdRequest();

void arduinoTypeRequest();
void firmwareVersionRequest();
void canBusEnabledRequest();
void heartbeatRequest();
void ioReportRequest();

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
std::pair<int, ShortWatchdog> parseShortWatchdog(const std::string &str);
std::pair<int, std::string> parseToAnalogState(const std::string &str);
std::pair<IOType, std::string> parseIOType(const std::string &str);
std::pair<int, std::string> parseToDigitalState(const std::string &str);
std::pair<int, std::string> parsePin(const std::string &str);
void populateGpioMap();
static std::map<int, GPIO*> gpioPins;
static std::set<ShortWatchdog> shortWatchdogs;
std::string getPinType(IOType type);
std::string analogPinFromNumber(int number);
GPIO *gpioPinByPinNumber(int pinNumber);
void serialInit(long long int baud, int timeout);

#if defined(ARDUINO_AVR_UNO)
    static const int AVAILABLE_ANALOG_PINS[]{A0, A1, A2, A3, A4, A5, -1};
    #if defined(__HAVE_CAN_BUS__)
        static const unsigned int NUMBER_OF_PINS{17};
        static const int AVAILABLE_PWM_PINS[]{3, 5, 6, 10, 11, -1};
        static const int AVAILABLE_PINS[]{2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5, -1};
    #else
        static const unsigned int NUMBER_OF_PINS{18};
        static const int AVAILABLE_PWM_PINS[]{3, 5, 6, 9, 10, 11, -1};
        static const int AVAILABLE_PINS[]{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5, -1};
    #endif
#elif defined(ARDUINO_AVR_NANO)
    static const int AVAILABLE_ANALOG_PINS[]{A0, A1, A2, A3, A4, A5, A6, A7, -1};                                                
    
    #if defined(__HAVE_CAN_BUS__)
        static const unsigned int NUMBER_OF_PINS{18};
        static const int AVAILABLE_PWM_PINS[]{2, 3, 4, 5, 6, 7, 8, 10, 11, 12, -1};
        static const int AVAILABLE_PINS[]{2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5, A6, A7, -1};
    #else
        static const unsigned int NUMBER_OF_PINS{19};
        static const int AVAILABLE_PWM_PINS[]{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 44, 45, 46, -1};
        static const int AVAILABLE_PINS[]{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5, A6, A7, -1};
    #endif
#elif defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560)
    static const int AVAILABLE_ANALOG_PINS[]{A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, -1};                                                
    
    #if defined(__HAVE_CAN_BUS__)
        static const unsigned int NUMBER_OF_PINS{67};
        static const int AVAILABLE_PWM_PINS[]{2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 44, 45, 46, -1};
        static const int AVAILABLE_PINS[]{2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
                                        38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
                                        A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, -1};
    #else
        static const unsigned int NUMBER_OF_PINS{68};
        static const int AVAILABLE_PWM_PINS[]{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 44, 45, 46, -1};
        static const int AVAILABLE_PINS[]{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
                                        38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
                                        A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, -1};
    #endif
#endif

#if defined(__HAVE_CAN_BUS__)
    #ifndef INT32U
        #define INT32U unsigned long
    #endif
    #ifndef INT8U
        #define INT8U byte
    #endif
    void canInitRequest();
    void canReadRequest(bool autoUp = false);
    void canWriteRequest(const std::string &canMessage, bool once = false);
    void addNegativeCanMaskRequest(const std::string &canMask);
    void removePositiveCanMaskRequest(const std::string &canMask);
    void canLiveUpdateRequest(const std::string &str);
    void clearCurrentMessageByIdRequest(const std::string &str);
    void currentCachedCanMessageByIdRequest(const std::string &str);
    void clearCanMessagesRequest();
    void currentPositiveCanMasksRequest();
    void currentNegativeCanMasksRequest();
    void allCurrentCanMasksRequest();
    void clearPositiveCanMasksRequest();
    void clearNegativeCanMasksRequest();
    void clearCanMasksRequest();
    void currentCachedCanMessagesRequest();
    void clearAllCanMasksRequest();
    std::pair<int, std::string> parseToCanState(const std::string &str);
    void sendCanMessage(const CanMessage &msg);
    static const int SPI_CS_PIN{9};
    MCP_CAN *canController;
    static const int CAN_CONNECTION_TIMEOUT{1000};
    static const int CAN_WRITE_REQUEST_SIZE{9};
    static bool canBusInitialized{false};
    static bool canLiveUpdate{false};
    CanMessage parseCanMessage(const std::string &str);
    static std::map<uint32_t, CanMessage> lastCanMessages;
    static std::set<uint32_t> positiveCanMasks;
    static std::set<uint32_t> negativeCanMasks;
    void addLastCanMessage(const CanMessage &msg);
    static const uint8_t CAN_MESSAGE_LENGTH{8};
    static const uint8_t CAN_FRAME{0};
    static const unsigned int CAN_COMMUNICATION_DOWN_TIME{1000};
    static int canCommunicationStartTime{0};
    static int canCommunicationEndTime{0};
#endif

int main()
{
    arduinoInit();
    serialInit(SERIAL_BAUD, SERIAL_TIMEOUT);
    populateGpioMap();

    while (true) {
        if (Serial.available()) {
            String serialRead{Serial.readStringUntil('}')};
            if (serialRead.length() > MAXIMUM_SERIAL_READ_SIZE) {
                printString(INVALID_LENGTH_EXCEEDED_HEADER);
                continue;
            }
            handleSerialString(serialRead.c_str());
        }
        if (shortWatchdogs.size() != 0) {
            for (auto &it : shortWatchdogs) {
                it.checkForShorts();
                if (it.shortEvent()) {
                    it.executeFailSafe();
                    printWatchdogShortEventResult(SHORT_EVENT_DETECTED_HEADER, it);
                    it.reset();
                }
            }
        }
        #if defined(__HAVE_CAN_BUS__)
            if (canLiveUpdate) {
                canReadRequest(canLiveUpdate);
            }
            bool canSend{false};
            for (auto &it : lastCanMessages) {
                if (it.second.toString() != "") {
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
    #if defined(__HAVE_CAN_BUS__)
        delete canController;
    #endif
    return 0;
}

void handleSerialString(const char *str)
{
    return handleSerialString(static_cast<std::string>(str));
}

void handleSerialString(const std::string &str)
{
    if (str == EMPTY_STRING) {
        return;
    } else if (isWhitespace(str)) {
        return;
    }
    if (startsWith(str, DIGITAL_READ_HEADER)) {
        if (checkValidRequestString(DIGITAL_READ_HEADER, str)) {
            digitalReadRequest(str.substr(static_cast<std::string>(DIGITAL_READ_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, DIGITAL_WRITE_HEADER)) {
        if (checkValidRequestString(DIGITAL_WRITE_HEADER, str)) {
            digitalWriteRequest(str.substr(static_cast<std::string>(DIGITAL_WRITE_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, ANALOG_READ_HEADER)) {
        if (checkValidRequestString(ANALOG_READ_HEADER, str)) {
            analogReadRequest(str.substr(static_cast<std::string>(ANALOG_READ_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, ANALOG_WRITE_HEADER)) {
        if (checkValidRequestString(ANALOG_WRITE_HEADER, str)) {
            analogWriteRequest(str.substr(static_cast<std::string>(ANALOG_WRITE_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, PIN_TYPE_HEADER)) {
        if (checkValidRequestString(PIN_TYPE_HEADER, str)) {
            pinTypeRequest(str.substr(static_cast<std::string>(PIN_TYPE_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, PIN_TYPE_CHANGE_HEADER)) {
        if (checkValidRequestString(PIN_TYPE_CHANGE_HEADER, str)) {
            pinTypeChangeRequest(str.substr(static_cast<std::string>(PIN_TYPE_CHANGE_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, SOFT_DIGITAL_READ_HEADER)) {
        if (checkValidRequestString(SOFT_DIGITAL_READ_HEADER, str)) {
            digitalReadRequest(str.substr(static_cast<std::string>(SOFT_DIGITAL_READ_HEADER).length()+1), SOFT);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, SOFT_ANALOG_READ_HEADER)) {
        if (checkValidRequestString(SOFT_ANALOG_READ_HEADER, str)) {
            softAnalogReadRequest(str.substr(static_cast<std::string>(SOFT_ANALOG_READ_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, ADD_SHORT_WATCHDOG_HEADER)) {
        if (checkValidRequestString(ADD_SHORT_WATCHDOG_HEADER, str)) {
            addShortWatchdogRequest(str.substr(static_cast<std::string>(ADD_SHORT_WATCHDOG_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, REMOVE_SHORT_WATCHDOG_HEADER)) {
        if (checkValidRequestString(REMOVE_SHORT_WATCHDOG_HEADER, str)) {
            removeShortWatchdogRequest(str.substr(static_cast<std::string>(REMOVE_SHORT_WATCHDOG_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, CHANGE_A_TO_D_THRESHOLD_HEADER)) {
        if (checkValidRequestString(CHANGE_A_TO_D_THRESHOLD_HEADER, str)) {
            changeAToDThresholdRequest(str.substr(static_cast<std::string>(CHANGE_A_TO_D_THRESHOLD_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, CURRENT_A_TO_D_THRESHOLD_HEADER)) {
        currentAToDThresholdRequest();
    } else if (startsWith(str, IO_REPORT_HEADER)) {
        ioReportRequest();
    } else if (startsWith(str, ARDUINO_TYPE_HEADER)) {
        arduinoTypeRequest();
    } else if (startsWith(str, CAN_BUS_ENABLED_HEADER)) {
        canBusEnabledRequest();
    } else if (startsWith(str, FIRMWARE_VERSION_HEADER)) {
        firmwareVersionRequest();
    } else if (startsWith(str, HEARTBEAT_HEADER)) {
        heartbeatRequest();
#if defined(__HAVE_CAN_BUS__)
    } else if (startsWith(str, CAN_INIT_HEADER)) {
        canInitRequest();
    } else if (startsWith(str, CAN_READ_HEADER)) {
        canReadRequest();
    } else if (startsWith(str, CAN_WRITE_ONCE_HEADER)) {
        if (checkValidRequestString(CAN_WRITE_ONCE_HEADER, str)) {
            canWriteRequest(str.substr(static_cast<std::string>(CAN_WRITE_HEADER).length()+1), true);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, CAN_WRITE_HEADER)) {
        if (checkValidRequestString(CAN_WRITE_HEADER, str)) {
            canWriteRequest(str.substr(static_cast<std::string>(CAN_WRITE_HEADER).length()+1), false);
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }     
    } else if (startsWith(str, CAN_LIVE_UPDATE_HEADER)) {
        if (checkValidRequestString(CAN_LIVE_UPDATE_HEADER, str)) {
            canLiveUpdateRequest(str.substr(static_cast<std::string>(CAN_LIVE_UPDATE_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, ADD_POSITIVE_CAN_MASK_HEADER)) {
        if (checkValidRequestString(ADD_POSITIVE_CAN_MASK_HEADER, str)) {
            addPositiveCanMaskRequest(str.substr(static_cast<std::string>(ADD_POSITIVE_CAN_MASK_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, REMOVE_POSITIVE_CAN_MASK_HEADER)) {
        if (checkValidRequestString(REMOVE_POSITIVE_CAN_MASK_HEADER, str)) {
            removePositiveCanMaskRequest(str.substr(static_cast<std::string>(REMOVE_POSITIVE_CAN_MASK_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, ADD_NEGATIVE_CAN_MASK_HEADER)) {
        if (checkValidRequestString(ADD_NEGATIVE_CAN_MASK_HEADER, str)) {
            addNegativeCanMaskRequest(str.substr(static_cast<std::string>(ADD_NEGATIVE_CAN_MASK_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    } else if (startsWith(str, REMOVE_NEGATIVE_CAN_MASK_HEADER)) {
        if (checkValidRequestString(REMOVE_NEGATIVE_CAN_MASK_HEADER, str)) {
            removeNegativeCanMaskRequest(str.substr(static_cast<std::string>(REMOVE_NEGATIVE_CAN_MASK_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }
    }  else if (startsWith(str, CLEAR_CAN_MESSAGE_BY_ID_HEADER)) {
        if (checkValidRequestString(CLEAR_CAN_MESSAGE_BY_ID_HEADER, str)) {
            clearCurrentMessageByIdRequest(str.substr(static_cast<std::string>(CLEAR_CAN_MESSAGE_BY_ID_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }   
    } else if (startsWith(str, CURRENT_CAN_MESSAGE_BY_ID_HEADER)) {
        if (checkValidRequestString(CURRENT_CAN_MESSAGE_BY_ID_HEADER, str)) {
            currentCachedCanMessageByIdRequest(str.substr(static_cast<std::string>(CURRENT_CAN_MESSAGE_BY_ID_HEADER).length()+1));
        } else {
            printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
        }   
    } else if (startsWith(str, CURRENT_CAN_MESSAGES_HEADER)) {
        currentCachedCanMessagesRequest();
    } else if (startsWith(str, CLEAR_CAN_MESSAGES_HEADER)) {
        clearCanMessagesRequest();
    }   else if (startsWith(str, CURRENT_POSITIVE_CAN_MASKS_HEADER)) {
        currentPositiveCanMasksRequest();
    } else if (startsWith(str, CURRENT_NEGATIVE_CAN_MASKS_HEADER)) {
        currentNegativeCanMasksRequest();
    } else if (startsWith(str, ALL_CURRENT_CAN_MASKS_HEADER)) {
        allCurrentCanMasksRequest();
    }  else if (startsWith(str, CLEAR_POSITIVE_CAN_MASKS_HEADER)) {
        clearPositiveCanMasksRequest();
    } else if (startsWith(str, CLEAR_NEGATIVE_CAN_MASKS_HEADER)) {
        clearNegativeCanMasksRequest();
    } else if (startsWith(str, CLEAR_ALL_CAN_MASKS_HEADER)) {
        clearAllCanMasksRequest();
#endif
    } else {
        printTypeResult(INVALID_HEADER, str, OPERATION_FAILURE);
    }
}


void changeAToDThresholdRequest(const std::string &str)
{
    if (str.length() == 0) {
        printTypeResult(CHANGE_A_TO_D_THRESHOLD_HEADER, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    std::pair<int, std::string> maybeState{parseToAnalogState(str)};
    if (maybeState.first == STATE_FAILURE) {
        printTypeResult(CHANGE_A_TO_D_THRESHOLD_HEADER, maybeState.second, OPERATION_FAILURE);
        return;
    } 
    GPIO::setAnalogToDigitalThreshold(maybeState.first);
    printTypeResult(CHANGE_A_TO_D_THRESHOLD_HEADER, GPIO::analogToDigitalThreshold(), OPERATION_SUCCESS);
}

void currentAToDThresholdRequest()
{
    printTypeResult(CURRENT_A_TO_D_THRESHOLD_HEADER, GPIO::analogToDigitalThreshold(), OPERATION_SUCCESS);
}

void populateGpioMap()
{
    int i{0};
    while (AVAILABLE_PINS[i++] > 0) {
        if (isValidAnalogInputPin(AVAILABLE_PINS[i-1])) {
            gpioPins.insert(std::pair<int, GPIO*>(AVAILABLE_PINS[i-1], new GPIO(AVAILABLE_PINS[i-1], IOType::ANALOG_INPUT)));
        } else {
            gpioPins.insert(std::pair<int, GPIO*>(AVAILABLE_PINS[i-1], new GPIO(AVAILABLE_PINS[i-1], IOType::DIGITAL_INPUT_PULLUP)));
        }
    }
}

void ioReportRequest()
{
    std::cout << IO_REPORT_HEADER << '}';
    int i{0};
    while (AVAILABLE_PINS[i] > 0) {
        GPIO *gpioPin{gpioPinByPinNumber(AVAILABLE_PINS[i++])};
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
        std::cout << '{' << gpioPin->pinNumber() << ':' << getPinType(gpioPin->ioType()) << ':' << state << "};";
    }

    std::cout << IO_REPORT_END_HEADER << '}';
}

void digitalReadRequest(const std::string &str, bool soft)
{
    if (str.length() == 0) {
        printResult(PIN_TYPE_CHANGE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    std::pair<int, std::string> pinNumber = parsePin(str);
    if (pinNumber.first == INVALID_PIN) {
        printResult((soft ? SOFT_DIGITAL_READ_HEADER : DIGITAL_READ_HEADER), pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    if (!isValidDigitalInputPin(pinNumber.first)) {
        printResult((soft ? SOFT_DIGITAL_READ_HEADER : DIGITAL_READ_HEADER), pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    bool state{(soft ? gpioPinByPinNumber(pinNumber.first)->g_softDigitalRead() : gpioPinByPinNumber(pinNumber.first)->g_digitalRead())};
    printResult((soft ? SOFT_DIGITAL_READ_HEADER : DIGITAL_READ_HEADER), pinNumber.second, state, OPERATION_SUCCESS);
}

void digitalWriteRequest(const std::string &str)
{
    if (str.length() == 0) {
        printResult(PIN_TYPE_CHANGE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    size_t foundPosition{str.find(":")};
    std::string maybePin{str.substr(0, foundPosition).c_str()};
    std::pair<int, std::string> pinNumber{parsePin(maybePin)};
    if (pinNumber.first == INVALID_PIN) {
        printResult(DIGITAL_WRITE_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    if (!isValidDigitalOutputPin(pinNumber.first)) {
        printResult(DIGITAL_WRITE_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    std::string maybeState{str.substr(foundPosition+1).c_str()};
    std::pair<int, std::string> state{parseToDigitalState(maybeState)};
    if (state.first == STATE_FAILURE) {
        printResult(DIGITAL_WRITE_HEADER, pinNumber.second, state.second, OPERATION_FAILURE);
        return;
    } 
    gpioPinByPinNumber(pinNumber.first)->g_digitalWrite(state.first);
    printResult(DIGITAL_WRITE_HEADER, pinNumber.second, state.second, OPERATION_SUCCESS);
}

void analogReadRequest(const std::string &str)
{
    if (str.length() == 0) {
        printResult(PIN_TYPE_CHANGE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    std::pair<int, std::string> pinNumber{parsePin(str)};
    if ((pinNumber.first == INVALID_PIN) || (!isValidAnalogInputPin(pinNumber.first))) {
        printResult(ANALOG_READ_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    if (!isValidAnalogInputPin(pinNumber.first)) {
        printResult(ANALOG_READ_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    int state{gpioPinByPinNumber(pinNumber.first)->g_analogRead()};
    printResult(ANALOG_READ_HEADER, pinNumber.second, state, OPERATION_SUCCESS);
}

void analogWriteRequest(const std::string &str)
{
    if (str.length() == 0) {
        printResult(PIN_TYPE_CHANGE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    size_t foundPosition{str.find(":")};
    std::string maybePin{str.substr(0, foundPosition).c_str()};
    std::pair<int, std::string> pinNumber{parsePin(maybePin)};

    if (pinNumber.first == INVALID_PIN) {
        printResult(ANALOG_WRITE_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    if (!isValidAnalogOutputPin(pinNumber.first)) {
        printResult(ANALOG_WRITE_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    std::string maybeState{str.substr(foundPosition+1).c_str()};
    std::pair<int, std::string> state{parseToAnalogState(maybeState)};
    if (state.first == STATE_FAILURE) {
        printResult(ANALOG_WRITE_HEADER, pinNumber.second, state.second, OPERATION_FAILURE);
    } else {
        gpioPinByPinNumber(pinNumber.first)->g_analogWrite(state.first);
        printResult(ANALOG_WRITE_HEADER, pinNumber.second, state.second, OPERATION_SUCCESS);
    }
}

void pinTypeRequest(const std::string &str)
{
    if (str.length() == 0) {
        printResult(PIN_TYPE_CHANGE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    std::pair<int, std::string> pinNumber = parsePin(str);
    if (pinNumber.first == INVALID_PIN) {
        printResult(PIN_TYPE_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    printResult(PIN_TYPE_HEADER, pinNumber.second, getPinType(gpioPinByPinNumber(pinNumber.first)->ioType()), OPERATION_SUCCESS);
}

void pinTypeChangeRequest(const std::string &str)
{   
    if (str.length() == 0) {
        printResult(PIN_TYPE_CHANGE_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    size_t foundPosition{str.find(":")};
    std::string maybePin{str.substr(0, foundPosition).c_str()};
    std::pair<int, std::string> pinNumber = parsePin(maybePin);
    if (pinNumber.first == INVALID_PIN) {
        printResult(PIN_TYPE_CHANGE_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    std::string maybeType{str.substr(foundPosition+1).c_str()};
    std::pair<IOType, std::string> type{parseIOType(maybeType)};
    if (type.first == IOType::UNSPECIFIED) {
        printResult(PIN_TYPE_CHANGE_HEADER, pinNumber.second, type.second, OPERATION_FAILURE);
        return;
    }
    if (!checkValidIOChangeRequest(type.first, pinNumber.first)) {
        printResult(PIN_TYPE_CHANGE_HEADER, pinNumber.second, type.second, OPERATION_FAILURE);
        return;
    }
    gpioPinByPinNumber(pinNumber.first)->setIOType(type.first);
    printResult(PIN_TYPE_CHANGE_HEADER, pinNumber.second, type.second, OPERATION_SUCCESS);
}

void softAnalogReadRequest(const std::string &str)
{
    if (str.length() == 0) {
        printResult(SOFT_ANALOG_READ_HEADER, INVALID_PIN, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    std::pair<int, std::string> pinNumber{parsePin(str)};
    if (pinNumber.first == INVALID_PIN) {
        printResult(SOFT_ANALOG_READ_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    if (!isValidAnalogOutputPin(pinNumber.first)) {
        printResult(SOFT_ANALOG_READ_HEADER, pinNumber.second, STATE_FAILURE, OPERATION_FAILURE);
        return;
    }
    int state{gpioPinByPinNumber(pinNumber.first)->g_softAnalogRead()};
    printResult(SOFT_ANALOG_READ_HEADER, pinNumber.second, state, OPERATION_SUCCESS);
}

void addShortWatchdogRequest(const std::string &str)
{
    std::pair<int, ShortWatchdog> shortWatchdog{parseShortWatchdog(str)};
    if (shortWatchdog.first == OPERATION_FAILURE) {
        printTypeResult(ADD_SHORT_WATCHDOG_HEADER, str, OPERATION_FAILURE);
        return;
    }
    if ((shortWatchdogs.insert(shortWatchdog.second)).second) {
        printWatchdogResult(ADD_SHORT_WATCHDOG_HEADER, shortWatchdog.second, OPERATION_SUCCESS);
    } else {
        printWatchdogResult(ADD_SHORT_WATCHDOG_HEADER, shortWatchdog.second, OPERATION_KIND_OF_SUCCESS);
    }
}

void removeShortWatchdogRequest(const std::string &str)
{
    std::pair<int, ShortWatchdog> shortWatchdog{parseShortWatchdog(str)};
    if (shortWatchdog.first == OPERATION_FAILURE) {
        printTypeResult(ADD_SHORT_WATCHDOG_HEADER, str, OPERATION_FAILURE);
        return;
    }
    shortWatchdogs.erase(shortWatchdogs.find(shortWatchdog.second));
    printWatchdogResult(ADD_SHORT_WATCHDOG_HEADER, shortWatchdog.second, OPERATION_SUCCESS);
}

std::pair<int, ShortWatchdog> parseShortWatchdog(const std::string &str)
{
    using namespace FirmwareUtilities;
    if (str.length() == 0) {
        return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
    }
    if (str.find(FAILSAFE_INTERIM_HEADER) != std::string::npos) {
        std::string firstHalf{str.substr(0, str.find(FAILSAFE_INTERIM_HEADER))};
        std::string secondHalf{str.substr(str.find(FAILSAFE_INTERIM_HEADER)+static_cast<std::string>(FAILSAFE_INTERIM_HEADER).length())};

        std::vector<std::string> states{parseToVector(firstHalf, ':')};
        std::vector<std::pair<std::string, std::string>> pairs;
        std::vector<std::pair<GPIO*, bool>> shorts;
        if (!isEven(states.size())) {
            return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
        }
        for (unsigned int i = 0; i < states.size(); i += 2) {
            pairs.push_back(std::pair<std::string, std::string>(states.at(i), states.at(i+1)));
        } 
        for (auto &it : pairs) {
            std::pair<int, std::string> maybePin{parsePin(it.first)};
            if (maybePin.first == INVALID_PIN) {
                return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
            }
            if (!isValidDigitalInputPin(maybePin.first)) {
                return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
            }
            std::pair<int, std::string> maybeState{parseToDigitalState(it.second)};
            if (maybeState.first == STATE_FAILURE) {
                return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
            }
            shorts.push_back(std::pair<GPIO*, int>(gpioPinByPinNumber(maybePin.first), maybeState.first));
        }

        std::set<FailSafe> fails;
        pairs.clear();
        states = parseToVector(secondHalf, ':');
        if (!isEven(states.size())) {
            return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
        }
        for (unsigned int i = 0; i < states.size(); i += 2) {
            pairs.push_back(std::pair<std::string, std::string>(states.at(i), states.at(i+1)));
        } 
        for (auto &it : pairs) {
            std::pair<int, std::string> maybePin{parsePin(it.first)};
            if (maybePin.first == INVALID_PIN) {
                return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
            }
            if (!isValidDigitalOutputPin(maybePin.first)) {
                return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
            }
            std::pair<int, std::string> maybeState{parseToDigitalState(it.second)};
            if (maybeState.first == STATE_FAILURE) {
                return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
            }
            fails.insert(FailSafe{gpioPinByPinNumber(maybePin.first), maybeState.first});
        }
        return std::pair<int, ShortWatchdog>(OPERATION_SUCCESS, ShortWatchdog{shorts, fails});
    } else {
        std::vector<std::string> states{parseToVector(str, ':')};
        if (!isEven(states.size())) {
            return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
        }
        std::vector<std::pair<GPIO*, bool>> shorts;
        std::vector<std::pair<std::string, std::string>> pairs;
        for (unsigned int i = 0; i < states.size(); i += 2) {
            pairs.push_back(std::pair<std::string, std::string>(states.at(i), states.at(i+1)));
        } 
        for (auto &it : pairs) {
            std::pair<int, std::string> maybePin{parsePin(it.first)};
            if (maybePin.first == INVALID_PIN) {
                return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
            }
            if (!isValidDigitalInputPin(maybePin.first)) {
                return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
            }
            std::pair<int, std::string> maybeState{parseToDigitalState(it.second)};
            if (maybeState.first == STATE_FAILURE) {
                return std::pair<int, ShortWatchdog>(OPERATION_FAILURE, ShortWatchdog{std::vector<std::pair<GPIO*, bool>>{}});
            }
            shorts.push_back(std::pair<GPIO*, int>(gpioPinByPinNumber(maybePin.first), maybeState.first));
        }
        return std::pair<int, ShortWatchdog>(OPERATION_SUCCESS, ShortWatchdog{shorts});
    }
}

bool checkValidRequestString(const char *header, const std::string &checkStr)
{
    return (checkStr.length() > (static_cast<std::string>(header).length() + 1));
}

void heartbeatRequest()
{
    printString(static_cast<std::string>(HEARTBEAT_HEADER) + "}");
}

void arduinoTypeRequest()
{
    printTypeResult(ARDUINO_TYPE_HEADER, ARDUINO_TYPE, OPERATION_SUCCESS);
}

void firmwareVersionRequest()
{
    printTypeResult(FIRMWARE_VERSION_HEADER, FIRMWARE_VERSION, OPERATION_SUCCESS);
}

void canBusEnabledRequest()
{
    #if defined(__HAVE_CAN_BUS__)
        printTypeResult(CAN_BUS_ENABLED_HEADER, CAN_BUS_ENABLED_STRING, OPERATION_SUCCESS);
    #else
        printTypeResult(CAN_BUS_ENABLED_HEADER, CAN_BUS_DISABLED_STRING, OPERATION_SUCCESS);
    #endif
} 

#if defined(ARDUINO_AVR_NANO)
   bool isValidDigitalOutputPin(int pinNumber)
    {
        if ((pinNumber == A6) || (pinNumber == A7)) {
            return false;
        }
        int i{0};
        while (AVAILABLE_ANALOG_PINS[i] > 0) {
            if (pinNumber == AVAILABLE_ANALOG_PINS[i++]) {
                return true;
            }
        }
        i = 0;
        while (AVAILABLE_PINS[i] > 0) {
            if (pinNumber == AVAILABLE_PINS[i++]) {
                return true;
            }
        }
        return false;
    }

    bool isValidDigitalInputPin(int pinNumber)
    {
        if ((pinNumber == A6) || (pinNumber == A7)) {
            return false;
        }
        int i{0};
        while (AVAILABLE_ANALOG_PINS[i] > 0) {
            if (pinNumber == AVAILABLE_ANALOG_PINS[i++]) {
                return true;
            }
        }
        i = 0;
        while (AVAILABLE_PINS[i] > 0) {
            if (pinNumber == AVAILABLE_PINS[i++]) {
                return true;
            }
        }
        return false;
    }
#else
    bool isValidDigitalOutputPin(int pinNumber)
    {
        int i{0};
        while (AVAILABLE_ANALOG_PINS[i] > 0) {
            if (pinNumber == AVAILABLE_ANALOG_PINS[i++]) {
                return true;
            }
        }
        i = 0;
        while (AVAILABLE_PINS[i] > 0) {
            if (pinNumber == AVAILABLE_PINS[i++]) {
                return true;
            }
        }
        return false;
    }

    bool isValidDigitalInputPin(int pinNumber)
    {
        int i{0};
        while (AVAILABLE_ANALOG_PINS[i] > 0) {
            if (pinNumber == AVAILABLE_ANALOG_PINS[i++]) {
                return true;
            }
        }
        i = 0;
        while (AVAILABLE_PINS[i] > 0) {
            if (pinNumber == AVAILABLE_PINS[i++]) {
                return true;
            }
        }
        return false;
    }
#endif

bool isValidAnalogOutputPin(int pinNumber)
{
    int i{0};
    while (AVAILABLE_PWM_PINS[i] > 0) {
        if (pinNumber == AVAILABLE_PWM_PINS[i++]) { 
            return true;
        }
    }
    return false;
}

bool isValidAnalogInputPin(int pinNumber)
{
    int i{0};
    while (AVAILABLE_ANALOG_PINS[i] > 0) {
        if (pinNumber == AVAILABLE_ANALOG_PINS[i++]) {
            return true;
        }
    }
    return false;
}

bool checkValidIOChangeRequest(IOType ioType, int pinNumber)
{
    if ((ioType == IOType::DIGITAL_OUTPUT) && (!isValidDigitalOutputPin(pinNumber))) {
        return false;
    } else if ((ioType == IOType::DIGITAL_INPUT) && (!isValidDigitalInputPin(pinNumber))) {
        return false;
    } else if ((ioType == IOType::ANALOG_INPUT) && (!isValidAnalogInputPin(pinNumber))) {
        return false;
    } else if ((ioType == IOType::ANALOG_OUTPUT) && (!isValidAnalogOutputPin(pinNumber))) {
        return false;
    } else if ((ioType == IOType::DIGITAL_INPUT_PULLUP) && (!isValidDigitalInputPin(pinNumber))) {
        return false;
    } else if (ioType == IOType::UNSPECIFIED) {
        return false;
    } else {
        return true;
    }
    return true;
}

std::string getPinType(IOType ioType)
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

std::pair<int, std::string> parsePin(const std::string &str)
{
    if (isValidAnalogPinIdentifier(str)) {
        return std::pair<int, std::string>(parseAnalogPin(str), str);
    } else if (isValidPinIdentifier(str)) {
        return std::pair<int, std::string>(atoi(str.c_str()), str);
    } else {
        return std::pair<int, std::string>(INVALID_PIN, str);
    }
}

std::pair<IOType, std::string> parseIOType(const std::string &str)
{
    if (str == DIGITAL_INPUT_IDENTIFIER) {
        return std::pair<IOType, std::string>(IOType::DIGITAL_INPUT, str);
    } else if (str == DIGITAL_OUTPUT_IDENTIFIER) {
        return std::pair<IOType, std::string>(IOType::DIGITAL_OUTPUT, str);
    } else if (str == ANALOG_INPUT_IDENTIFIER) {
        return std::pair<IOType, std::string>(IOType::ANALOG_INPUT, str);
    } else if (str == ANALOG_OUTPUT_IDENTIFIER) {
        return std::pair<IOType, std::string>(IOType::ANALOG_OUTPUT, str);
    } else if (str == DIGITAL_INPUT_PULLUP_IDENTIFIER) {
        return std::pair<IOType, std::string>(IOType::DIGITAL_INPUT_PULLUP, str);
    } else {
        return std::pair<IOType, std::string>(IOType::UNSPECIFIED, str);
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
    int i{0};
    while (static_cast<std::string>(DIGITAL_STATE_HIGH_IDENTIFIERS[i]) != static_cast<std::string>(CHAR_ARRAY_TERMINATOR)) {
        if (toLowercase(str) == static_cast<std::string>(DIGITAL_STATE_HIGH_IDENTIFIERS[i++])) {
            return true;
        }
    }
    i = 0;
    while (static_cast<std::string>(DIGITAL_STATE_LOW_IDENTIFIERS[i]) != static_cast<std::string>(CHAR_ARRAY_TERMINATOR)) {
        if (toLowercase(str) == static_cast<std::string>(DIGITAL_STATE_LOW_IDENTIFIERS[i++])) {
            return true;
        }
    }
    return false;
}

std::pair<int, std::string> parseToDigitalState(const std::string &str)
{
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

std::pair<int, std::string> parseToAnalogState(const std::string &str)
{
    if (!isValidAnalogStateIdentifier(str)) {
        return std::pair<int, std::string>(INVALID_PIN, str);
    } else {
        int temp{atoi(str.c_str())};
        if (temp > GPIO::ANALOG_MAX) {
            temp = GPIO::ANALOG_MAX;
        }
        if (temp < 0) {
            return std::pair<int, std::string>(INVALID_PIN, str);
        }
        return std::pair<int, std::string>(temp, toDecString(temp));
    }
}

bool isValidPinIdentifier(const std::string &str)
{
    int i{0};
    while (static_cast<std::string>(VALID_PIN_IDENTIFIERS[i]) != static_cast<std::string>(CHAR_ARRAY_TERMINATOR)) {
        if (toLowercase(str) == static_cast<std::string>(VALID_PIN_IDENTIFIERS[i++])) {
            return true;
        }
    }
    return false;
}

bool isValidPwmPinIdentifier(const std::string &str)
{
    int i{0};
    while (static_cast<std::string>(VALID_PWM_PIN_IDENTIFIERS[i]) != static_cast<std::string>(CHAR_ARRAY_TERMINATOR)) {
        if (toLowercase(str) == static_cast<std::string>(VALID_PWM_PIN_IDENTIFIERS[i++])) {
            return true;
        }
    }
    return false;
}

bool isValidAnalogPinIdentifier(const std::string &str)
{
    int i{0};
    while (static_cast<std::string>(VALID_ANALOG_PIN_IDENTIFIERS[i]) != static_cast<std::string>(CHAR_ARRAY_TERMINATOR)) {
        if (toLowercase(str) == static_cast<std::string>(VALID_ANALOG_PIN_IDENTIFIERS[i++])) {
            return true;
        }
    }
    return false;
}

bool isValidPinTypeIdentifier(const std::string &str)
{
    int i{0};
    while (static_cast<std::string>(VALID_PIN_TYPE_IDENTIFIERS[i]) != static_cast<std::string>(CHAR_ARRAY_TERMINATOR)) {
        if (toLowercase(str) == static_cast<std::string>(VALID_PIN_TYPE_IDENTIFIERS[i++])) {
            return true;
        }
    }
    return false;
}

void serialInit(long long int baud, int timeout)
{
    Serial.begin(baud);
    Serial.setTimeout(timeout);
}

GPIO *gpioPinByPinNumber(int pinNumber)
{
    if (gpioPins.find(pinNumber) == gpioPins.end()) {
        return gpioPins.find(2)->second;
    } else {
        return gpioPins.find(pinNumber)->second;
    }
}


#if defined(ARDUINO_AVR_UNO)

    int parseAnalogPin(const std::string &pinAlias)
    {
        if ((pinAlias == A0_STRING) || (pinAlias == A0_EQUIVALENT_STRING)) {
            return 14;
        } else if ((pinAlias == A1_STRING) || (pinAlias == A1_EQUIVALENT_STRING)) {
            return 15;
        } else if ((pinAlias == A2_STRING) || (pinAlias == A2_EQUIVALENT_STRING)) {
            return 16;
        } else if ((pinAlias == A3_STRING) || (pinAlias == A3_EQUIVALENT_STRING)) {
            return 17;
        } else if ((pinAlias == A4_STRING) || (pinAlias == A4_EQUIVALENT_STRING)) {
            return 18;
        } else if ((pinAlias == A5_STRING) || (pinAlias == A5_EQUIVALENT_STRING)) {
            return 19;
        } else {
            return 0;
        }
    }

    std::string analogPinFromNumber(int pinNumber)
    {
        if (pinNumber == 14) {
            return A0_STRING;
        } else if (pinNumber == 15) {
            return A1_STRING;
        } else if (pinNumber == 16) {
            return A2_STRING;
        } else if (pinNumber == 17) {
            return A3_STRING;
        } else if (pinNumber == 18) {
            return A4_STRING;
        } else if (pinNumber == 19) {
            return A5_STRING;
        } else {
            return EMPTY_STRING;
        }
    }

#elif defined(ARDUINO_AVR_NANO)

    int parseAnalogPin(const std::string &pinAlias)
    {
        if ((pinAlias == A0_STRING) || (pinAlias == A0_EQUIVALENT_STRING)) {
            return 14;
        } else if ((pinAlias == A1_STRING) || (pinAlias == A1_EQUIVALENT_STRING)) {
            return 15;
        } else if ((pinAlias == A2_STRING) || (pinAlias == A2_EQUIVALENT_STRING)) {
            return 16;
        } else if ((pinAlias == A3_STRING) || (pinAlias == A3_EQUIVALENT_STRING)) {
            return 17;
        } else if ((pinAlias == A4_STRING) || (pinAlias == A4_EQUIVALENT_STRING)) {
            return 18;
        } else if ((pinAlias == A5_STRING) || (pinAlias == A5_EQUIVALENT_STRING)) {
            return 19;
        } else if ((pinAlias == A6_STRING) || (pinAlias == A6_EQUIVALENT_STRING)) {
            return 20;
        } else if ((pinAlias == A7_STRING) || (pinAlias == A7_EQUIVALENT_STRING)) {
            return 21;
        } else {
            return 0;
        }
    }

    std::string analogPinFromNumber(int pinNumber)
    {
        if (pinNumber == 14) {
            return A0_STRING;
        } else if (pinNumber == 15) {
            return A1_STRING;
        } else if (pinNumber == 16) {
            return A2_STRING;
        } else if (pinNumber == 17) {
            return A3_STRING;
        } else if (pinNumber == 18) {
            return A4_STRING;
        } else if (pinNumber == 19) {
            return A5_STRING;
        } else if (pinNumber == 20) {
            return A6_STRING;
        }  else if (pinNumber == 21) {
            return A7_STRING;
        } else {
            return EMPTY_STRING;
        }
    }

#elif defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560)

    int parseAnalogPin(const std::string &pinAlias)
    {
        if ((pinAlias == A0_STRING) || (pinAlias == A0_EQUIVALENT_STRING)) {
            return 54;
        } else if ((pinAlias == A1_STRING) || (pinAlias == A1_EQUIVALENT_STRING)) {
            return 55;
        } else if ((pinAlias == A2_STRING) || (pinAlias == A2_EQUIVALENT_STRING)) {
            return 56;
        } else if ((pinAlias == A3_STRING) || (pinAlias == A3_EQUIVALENT_STRING)) {
            return 57;
        } else if ((pinAlias == A4_STRING) || (pinAlias == A4_EQUIVALENT_STRING)) {
            return 58;
        } else if ((pinAlias == A5_STRING) || (pinAlias == A5_EQUIVALENT_STRING)) {
            return 59;
        } else if ((pinAlias == A6_STRING) || (pinAlias == A6_EQUIVALENT_STRING)) {
            return 60;
        } else if ((pinAlias == A7_STRING) || (pinAlias == A7_EQUIVALENT_STRING)) {
            return 61;
        } else if ((pinAlias == A8_STRING) || (pinAlias == A8_EQUIVALENT_STRING)) {
            return 62;
        } else if ((pinAlias == A9_STRING) || (pinAlias == A9_EQUIVALENT_STRING)) {
            return 63;
        } else if ((pinAlias == A10_STRING) || (pinAlias == A10_EQUIVALENT_STRING)) {
            return 64;
        } else if ((pinAlias == A11_STRING) || (pinAlias == A11_EQUIVALENT_STRING)) {
            return 65;
        } else if ((pinAlias == A12_STRING) || (pinAlias == A12_EQUIVALENT_STRING)) {
            return 66;
        } else if ((pinAlias == A13_STRING) || (pinAlias == A13_EQUIVALENT_STRING)) {
            return 67;
        } else if ((pinAlias == A14_STRING) || (pinAlias == A14_EQUIVALENT_STRING)) {
            return 68;
        } else if ((pinAlias == A15_STRING) || (pinAlias == A15_EQUIVALENT_STRING)) {
            return 69;
        } else {
            return 0;
        }
    }

    std::string analogPinFromNumber(int pinNumber)
    {
        if (pinNumber == 54) {
            return A0_STRING;
        } else if (pinNumber == 55) {
            return A1_STRING;
        } else if (pinNumber == 56) {
            return A2_STRING;
        } else if (pinNumber == 57) {
            return A3_STRING;
        } else if (pinNumber == 58) {
            return A4_STRING;
        } else if (pinNumber == 59) {
            return A5_STRING;
        } else if (pinNumber == 60) {
            return A6_STRING;
        } else if (pinNumber == 61) {
            return A7_STRING;
        } else if (pinNumber == 62) {
            return A8_STRING;
        } else if (pinNumber == 63) {
            return A9_STRING;
        } else if (pinNumber == 64) {
            return A10_STRING;
        } else if (pinNumber == 65) {
            return A11_STRING;
        } else if (pinNumber == 66) {
            return A12_STRING;
        } else if (pinNumber == 67) {
            return A13_STRING;
        } else if (pinNumber == 68) {
            return A14_STRING;
        } else if (pinNumber == 69) {
            return A15_STRING;
        } else {
            return EMPTY_STRING;
        }
    }

#endif

#if defined(__HAVE_CAN_BUS__)
    void canInit()
    {
        using namespace ArduinoPCStrings;
        canController = new MCP_CAN(SPI_CS_PIN);
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

    void canInitRequest()
    {
        using namespace ArduinoPCStrings;
        if (canBusInitialized) {
            printSingleResult(CAN_INIT_HEADER, OPERATION_SUCCESS);
            return;
        } else {
            canInit();
        }
        if (canBusInitialized) {
            printSingleResult(CAN_INIT_HEADER, OPERATION_SUCCESS);
        } else {
            printSingleResult(CAN_INIT_HEADER, OPERATION_FAILURE);
        }
        
    }

    void canReadRequest(bool autoUp)
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
                printCanResult(CAN_READ_HEADER, msg.toString(), OPERATION_SUCCESS);
            } else {
                if (positiveCanMasks.size() == 0) {
                    if (negativeCanMasks.find(canID) == negativeCanMasks.end()) {
                        CanMessage msg{canID, CAN_FRAME, receivedPacketLength, CanDataPacket{pack[0], pack[1], pack[2], pack[3], pack[4], pack[5], pack[6], pack[7]}};
                        printCanResult(CAN_READ_HEADER, msg.toString(), OPERATION_SUCCESS);
                    }
                }
                if (negativeCanMasks.size() == 0) {
                    if (positiveCanMasks.find(canID) != positiveCanMasks.end()) {
                        CanMessage msg{canID, CAN_FRAME, receivedPacketLength, CanDataPacket{pack[0], pack[1], pack[2], pack[3], pack[4], pack[5], pack[6], pack[7]}};
                        printCanResult(CAN_READ_HEADER, msg.toString(), OPERATION_SUCCESS);
                    }
                }
                if (positiveCanMasks.find(canID) != positiveCanMasks.end()) {
                    if (negativeCanMasks.find(canID) == negativeCanMasks.end()) {
                        CanMessage msg{canID, CAN_FRAME, receivedPacketLength, CanDataPacket{pack[0], pack[1], pack[2], pack[3], pack[4], pack[5], pack[6], pack[7]}};
                        printCanResult(CAN_READ_HEADER, msg.toString(), OPERATION_SUCCESS);
                    }
                }
            }
        } else if (!autoUp) {
            printBlankCanResult(CAN_READ_HEADER, OPERATION_SUCCESS);
        }
    }

    void canWriteRequest(const std::string &str, bool once)
    {
        using namespace ArduinoPCStrings;
        if (!canBusInitialized) {
            canInit();
        }
        CanMessage msg{parseCanMessage(str)};
        if (msg.toString() == "") {
            printSingleResult(CAN_WRITE_HEADER, OPERATION_FAILURE);
            return;
        }
        if (!once) {
            addLastCanMessage(msg);
        }
        sendCanMessage(msg);
        printCanResult(CAN_WRITE_HEADER, msg.toString(), OPERATION_SUCCESS);
    }

    void addPositiveCanMaskRequest(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(ADD_POSITIVE_CAN_MASK_HEADER, str, OPERATION_FAILURE);
        }
        if ((positiveCanMasks.insert(maybeID)).second) {
            printTypeResult(ADD_POSITIVE_CAN_MASK_HEADER, str, OPERATION_SUCCESS);
        } else {
            printTypeResult(ADD_POSITIVE_CAN_MASK_HEADER, str, OPERATION_KIND_OF_SUCCESS);
        }
    }

    void removePositiveCanMaskRequest(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(REMOVE_POSITIVE_CAN_MASK_HEADER, str, OPERATION_FAILURE);
        }
        positiveCanMasks.erase(maybeID);
        printTypeResult(REMOVE_POSITIVE_CAN_MASK_HEADER, str, OPERATION_SUCCESS);
    }

    void addNegativeCanMaskRequest(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(ADD_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_FAILURE);
        }
        if ((negativeCanMasks.insert(maybeID)).second) {
            printTypeResult(ADD_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_SUCCESS);
        } else {
            printTypeResult(ADD_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_KIND_OF_SUCCESS);
        }
    }

    void removeNegativeCanMaskRequest(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(REMOVE_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_FAILURE);
        }
        negativeCanMasks.erase(maybeID);
        printTypeResult(REMOVE_NEGATIVE_CAN_MASK_HEADER, str, OPERATION_SUCCESS);
    }

    void canLiveUpdateRequest(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        if (str == EMPTY_STRING) {
            printTypeResult(CAN_LIVE_UPDATE_HEADER, OPERATION_FAILURE_STRING, OPERATION_FAILURE);
            return;
        }
        std::pair<int, std::string> canState{parseToCanState(str)};
        if (canState.first == OPERATION_FAILURE) {
            printTypeResult(CAN_LIVE_UPDATE_HEADER, canState.second, OPERATION_FAILURE);
        } else {
            canLiveUpdate = canState.first;
            printTypeResult(CAN_LIVE_UPDATE_HEADER, canState.second, OPERATION_SUCCESS);
        }
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

    void currentCachedCanMessageByIdRequest(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(CURRENT_CAN_MESSAGE_BY_ID_HEADER, str, OPERATION_FAILURE);
        }
        if (lastCanMessages.find(maybeID) == lastCanMessages.end()) {
            printBlankCanResult(CURRENT_CAN_MESSAGE_BY_ID_HEADER, OPERATION_SUCCESS);
        } else {
            printCanResult(CURRENT_CAN_MESSAGE_BY_ID_HEADER, lastCanMessages.find(maybeID)->second, OPERATION_SUCCESS);
        }
    }

    void clearCurrentMessageByIdRequest(const std::string &str)
    {
        using namespace ArduinoPCStrings;
        using namespace FirmwareUtilities;
        uint32_t maybeID{CanMessage::parseCanID(str)};
        if (maybeID == 0) {
            printTypeResult(CLEAR_CAN_MESSAGE_BY_ID_HEADER, str, OPERATION_FAILURE);
        }
        lastCanMessages.erase(maybeID);
        printTypeResult(CLEAR_CAN_MESSAGE_BY_ID_HEADER, str, OPERATION_SUCCESS);
    }

    void currentCachedCanMessagesRequest()
    {
        for (auto &it : lastCanMessages) {
            printCanResult(CURRENT_CAN_MESSAGES_HEADER, it.second, OPERATION_SUCCESS);
        }
    }
        
    void clearCanMessagesRequest()
    {
        lastCanMessages.clear();
        printSingleResult(CLEAR_CAN_MESSAGES_HEADER, OPERATION_SUCCESS);
    }

    void currentPositiveCanMasksRequest()
    {
        for (auto &it :positiveCanMasks) {
            printTypeResult(CURRENT_POSITIVE_CAN_MASKS_HEADER, it, OPERATION_SUCCESS);
        }
    }

    void currentNegativeCanMasksRequest()
    {
        for (auto &it : negativeCanMasks) {
            printTypeResult(CURRENT_NEGATIVE_CAN_MASKS_HEADER, it, OPERATION_SUCCESS);
        }
    }

    void allCurrentCanMasksRequest()
    {
        currentPositiveCanMasksRequest();
        currentNegativeCanMasksRequest();
    }

    void clearPositiveCanMasksRequest()
    {
        positiveCanMasks.clear();
        printSingleResult(CLEAR_POSITIVE_CAN_MASKS_HEADER, OPERATION_SUCCESS);
    }

    void clearNegativeCanMasksRequest()
    {
        negativeCanMasks.clear();
        printSingleResult(CLEAR_NEGATIVE_CAN_MASKS_HEADER, OPERATION_SUCCESS);
    }

    void clearAllCanMasksRequest()
    {
        positiveCanMasks.clear();
        negativeCanMasks.clear();
        printSingleResult(CLEAR_ALL_CAN_MASKS_HEADER, OPERATION_SUCCESS);
    }

#endif
