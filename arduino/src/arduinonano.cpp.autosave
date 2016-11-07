#include "arduino/arduinonano.h"

std::set<int> ArduinoNano::s_availableAnalogPins{14, 15, 16, 17, 18, 19, 20, 21};
std::set<int> ArduinoNano::s_availablePwmPins{3, 5, 6, 9, 10, 11};
std::set<int> ArduinoNano::s_availablePins{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};
const char *ArduinoNano::ARDUINO_TYPE_IDENTIFIER{"arduino_nano"};
int ArduinoNano::s_numberOfDigitalPins{13};

ArduinoNano::ArduinoNano(std::shared_ptr<SerialPort> serialPort, std::shared_ptr<GlobalLogger> globalLogger) :
    m_serialPortIndex{Arduino::addSerialPort(serialPort)},
    m_firmwareVersion{""},
    m_canCapability{std::make_pair(false, false)},
    m_canPinAlias{""},
    m_availablePins{ArduinoNano::s_availablePins},
    m_availablePwmPins{ArduinoNano::s_availablePwmPins},
    m_availableAnalogPins{ArduinoNano::s_availableAnalogPins},
    m_numberOfDigitalPins{ArduinoNano::s_numberOfDigitalPins},
    m_analogToDigitalThreshold{-1}
{
    initializeIO(globalLogger);
}

ArduinoNano::ArduinoNano(std::shared_ptr<SerialPort> serialPort, bool canCapability, std::shared_ptr<GlobalLogger> globalLogger) :
    m_serialPortIndex{Arduino::addSerialPort(serialPort)},
    m_firmwareVersion{""},
    m_canCapability{std::make_pair(true, canCapability)},
    m_canPinAlias{""},
    m_availablePins{ArduinoNano::s_availablePins},
    m_availablePwmPins{ArduinoNano::s_availablePwmPins},
    m_availableAnalogPins{ArduinoNano::s_availableAnalogPins},
    m_numberOfDigitalPins{ArduinoNano::s_numberOfDigitalPins},
    m_analogToDigitalThreshold{-1}

{
    initializeIO(globalLogger);
}


ArduinoNano::ArduinoNano(std::shared_ptr<SerialPort> serialPort, const std::string &firmwareVersion, std::shared_ptr<GlobalLogger> globalLogger) :
    m_serialPortIndex{Arduino::addSerialPort(serialPort)},
    m_firmwareVersion{firmwareVersion},
    m_canCapability{std::make_pair(false, false)},
    m_canPinAlias{""},
    m_availablePins{ArduinoNano::s_availablePins},
    m_availablePwmPins{ArduinoNano::s_availablePwmPins},
    m_availableAnalogPins{ArduinoNano::s_availableAnalogPins},
    m_numberOfDigitalPins{ArduinoNano::s_numberOfDigitalPins},
    m_analogToDigitalThreshold{-1}
{
    initializeIO(globalLogger);
}

ArduinoNano::ArduinoNano(std::shared_ptr<SerialPort> serialPort, const std::string &firmwareVersion, bool canCapability, std::shared_ptr<GlobalLogger> globalLogger) :
    m_serialPortIndex{Arduino::addSerialPort(serialPort)},
    m_firmwareVersion{firmwareVersion},
    m_canCapability{std::make_pair(true, canCapability)},
    m_canPinAlias{""},
    m_availablePins{ArduinoNano::s_availablePins},
    m_availablePwmPins{ArduinoNano::s_availablePwmPins},
    m_availableAnalogPins{ArduinoNano::s_availableAnalogPins},
    m_numberOfDigitalPins{ArduinoNano::s_numberOfDigitalPins},
    m_analogToDigitalThreshold{-1}
{
    initializeIO(globalLogger);
}



int ArduinoNano::staticParseAnalogPin(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if ((pinAlias == NANO_A0_STRING) || (pinAlias == NANO_A0_EQUIVALENT_STRING)) {
        return 14;
    } else if ((pinAlias == NANO_A1_STRING) || (pinAlias == NANO_A1_EQUIVALENT_STRING)) {
        return 15;
    } else if ((pinAlias == NANO_A2_STRING) || (pinAlias == NANO_A2_EQUIVALENT_STRING)) {
        return 16;
    } else if ((pinAlias == NANO_A3_STRING) || (pinAlias == NANO_A3_EQUIVALENT_STRING)) {
        return 17;
    } else if ((pinAlias == NANO_A4_STRING) || (pinAlias == NANO_A4_EQUIVALENT_STRING)) {
        return 18;
    } else if ((pinAlias == NANO_A5_STRING) || (pinAlias == NANO_A5_EQUIVALENT_STRING)) {
        return 19;
    } else if ((pinAlias == NANO_A6_STRING) || (pinAlias == NANO_A6_EQUIVALENT_STRING)) {
        return 20;
    } else if ((pinAlias == NANO_A7_STRING) || (pinAlias == NANO_A7_EQUIVALENT_STRING)) {
        return 21;
    } else {
        throw std::logic_error(INVALID_ANALOG_PIN_BASE_STRING + pinAlias + NANO_INVALID_ANALOG_STRING_TAIL_STRING);
    }
}

std::string ArduinoNano::staticAnalogPinFromNumber(int pinNumber)
{
    using namespace ArduinoPCStrings;
    if (pinNumber == 14) {
        return NANO_A0_STRING;
    } else if (pinNumber == 15) {
        return NANO_A1_STRING;
    } else if (pinNumber == 16) {
        return NANO_A2_STRING;
    } else if (pinNumber == 17) {
        return NANO_A3_STRING;
    } else if (pinNumber == 18) {
        return NANO_A4_STRING;
    } else if (pinNumber == 19) {
        return NANO_A5_STRING;
    } else if (pinNumber == 20) {
        return NANO_A6_STRING;
    } else if (pinNumber == 21) {
        return NANO_A7_STRING;
    } else {
        throw std::runtime_error(INVALID_ANALOG_PIN_BASE_STRING + std::to_string(pinNumber) + NANO_INVALID_ANALOG_INT_TAIL_STRING);
    }
}

