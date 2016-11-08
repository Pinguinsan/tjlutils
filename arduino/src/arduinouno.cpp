#include "arduinouno.h"

std::set<int> ArduinoUno::s_availableAnalogPins{14, 15, 16, 17, 18, 19};
std::set<int> ArduinoUno::s_availablePwmPins{3, 5, 6, 9, 10, 11};
std::set<int> ArduinoUno::s_availablePins{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

const char *ArduinoUno::ARDUINO_TYPE_IDENTIFIER{"arduino_uno"};
int ArduinoUno::s_numberOfDigitalPins{13};

ArduinoUno::ArduinoUno(std::shared_ptr<SerialPort> serialPort, std::shared_ptr<GlobalLogger> globalLogger) :
    m_serialPortIndex{Arduino::addSerialPort(serialPort)},
    m_firmwareVersion{""},
    m_canCapability{std::make_pair(false, false)},
    m_canPinAlias{""},
    m_availablePins{ArduinoUno::s_availablePins},
    m_availablePwmPins{ArduinoUno::s_availablePwmPins},
    m_availableAnalogPins{ArduinoUno::s_availableAnalogPins},
    m_numberOfDigitalPins{ArduinoUno::s_numberOfDigitalPins},
    m_analogToDigitalThreshold{-1}
{
    initializeIO(globalLogger);
}

ArduinoUno::ArduinoUno(std::shared_ptr<SerialPort> serialPort, bool canCapability, std::shared_ptr<GlobalLogger> globalLogger) :
    m_serialPortIndex{Arduino::addSerialPort(serialPort)},
    m_firmwareVersion{""},
    m_canCapability{std::make_pair(true, canCapability)},
    m_canPinAlias{""},
    m_availablePins{ArduinoUno::s_availablePins},
    m_availablePwmPins{ArduinoUno::s_availablePwmPins},
    m_availableAnalogPins{ArduinoUno::s_availableAnalogPins},
    m_numberOfDigitalPins{ArduinoUno::s_numberOfDigitalPins},
    m_analogToDigitalThreshold{-1}

{
    initializeIO(globalLogger);
}


ArduinoUno::ArduinoUno(std::shared_ptr<SerialPort> serialPort, const std::string &firmwareVersion, std::shared_ptr<GlobalLogger> globalLogger) :
    m_serialPortIndex{Arduino::addSerialPort(serialPort)},
    m_firmwareVersion{firmwareVersion},
    m_canCapability{std::make_pair(false, false)},
    m_canPinAlias{""},
    m_availablePins{ArduinoUno::s_availablePins},
    m_availablePwmPins{ArduinoUno::s_availablePwmPins},
    m_availableAnalogPins{ArduinoUno::s_availableAnalogPins},
    m_numberOfDigitalPins{ArduinoUno::s_numberOfDigitalPins},
    m_analogToDigitalThreshold{-1}
{
    initializeIO(globalLogger);
}

ArduinoUno::ArduinoUno(std::shared_ptr<SerialPort> serialPort, const std::string &firmwareVersion, bool canCapability, std::shared_ptr<GlobalLogger> globalLogger) :
    m_serialPortIndex{Arduino::addSerialPort(serialPort)},
    m_firmwareVersion{firmwareVersion},
    m_canCapability{std::make_pair(true, canCapability)},
    m_canPinAlias{""},
    m_availablePins{ArduinoUno::s_availablePins},
    m_availablePwmPins{ArduinoUno::s_availablePwmPins},
    m_availableAnalogPins{ArduinoUno::s_availableAnalogPins},
    m_numberOfDigitalPins{ArduinoUno::s_numberOfDigitalPins},
    m_analogToDigitalThreshold{-1}
{
    initializeIO(globalLogger);
}

bool ArduinoUno::setPinAlias(int pinNumber, const std::string &newAlias)
{
    if (this->m_gpioPinIterationAliasMap.find(pinNumber) == this->m_gpioPinIterationAliasMap.end()) {
        return false;
    } else {
        this->m_gpioPinIterationAliasMap.find(pinNumber)->second = newAlias;
        return true;
    }
}

bool ArduinoUno::setPinAlias(std::shared_ptr<GPIO> gpio, const std::string &newAlias)
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

bool ArduinoUno::setPinAlias(const GPIO &gpio, const std::string &newAlias)
{
    if (this->m_gpioPinIterationAliasMap.find(gpio.pinNumber()) == this->m_gpioPinIterationAliasMap.end()) {
        return false;
    } else {
        this->m_gpioPinIterationAliasMap.find(gpio.pinNumber())->second = newAlias;
        return true;
    }
}

bool ArduinoUno::isBluetooth() const
{
    return Arduino::isBluetooth(this->m_serialPortIndex);
}

void ArduinoUno::initializeIO(std::shared_ptr<GlobalLogger> globalLogger)
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
    for (auto &it : cfr.unoAliasesMap()) {
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
    for (auto &it : cfr.unoIOTypesMap()) {
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
    for (auto &it : cfr.unoInitialStatesMap()) {
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



void ArduinoUno::writeRawString(const std::string &str)
{
    return Arduino::writeRawString(str, this->m_serialPortIndex);
}

std::string ArduinoUno::readRawString()
{
    return Arduino::readRawString(this->m_serialPortIndex);
}

void ArduinoUno::flushRX()
{
    return Arduino::flushRX(this->m_serialPortIndex);
}

void ArduinoUno::flushTX()
{
    return Arduino::flushTX(this->m_serialPortIndex);
}

void ArduinoUno::flushRXTX()
{
    return Arduino::flushRXTX(this->m_serialPortIndex);
}

void ArduinoUno::flushTXRX()
{
    return Arduino::flushTXRX(this->m_serialPortIndex);
}

void ArduinoUno::setCanCapability(const std::pair<bool, bool> &capability)
{
    this->m_canCapability = capability;
    if ((this->m_canCapability.first) && (this->m_canCapability.second)) {
        eraseCanPin();
    }
}

void ArduinoUno::setFirmwareVersion(const std::string &firmwareVersion)
{
    this->m_firmwareVersion = firmwareVersion;
}

std::string ArduinoUno::firmwareVersion()
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

void ArduinoUno::setAnalogToDigitalThreshold(int state)
{
    this->m_analogToDigitalThreshold = state;
}

int ArduinoUno::analogToDigitalThreshold()
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


void ArduinoUno::eraseCanPin()
{
    this->m_availablePins.erase(this->m_availablePins.find(CAN_BUS_PIN));
    this->m_availablePwmPins.erase(this->m_availablePwmPins.find(CAN_BUS_PIN));
    this->m_gpioPins.erase(this->m_gpioPins.find(CAN_BUS_PIN));
    this->m_gpioPinsAlias.erase(this->m_gpioPinsAlias.find(std::to_string(CAN_BUS_PIN)));
    this->m_numberOfDigitalPins--;

}

std::pair<bool, bool> ArduinoUno::canCapability()
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

std::shared_ptr<SerialPort> ArduinoUno::serialPort() const
{
    return Arduino::serialPortAtIndex(this->m_serialPortIndex);
}

bool ArduinoUno::confirmValidAlias(const std::pair<int, std::string> &alias) const
{
    (void)alias;
    return true;
}

bool ArduinoUno::confirmValidIOType(const std::pair<int, std::string> &ioType) const
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

bool ArduinoUno::confirmValidStates(const std::pair<int, std::string> &state) const
{
    (void)state;
    return true;
}

void ArduinoUno::assignIOTypes(std::shared_ptr<GlobalLogger> globalLogger)
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

std::shared_ptr<GPIO> ArduinoUno::gpioPinByPinAlias(const std::string &pinAlias) const
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return (this->m_gpioPinsAlias.find(pinAlias)->second);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias);
    }
}

std::shared_ptr<GPIO> ArduinoUno::gpioPinByPinNumber(int pinNumber) const
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(pinNumber) != this->m_gpioPins.end()) {
        return (this->m_gpioPins.find(pinNumber)->second);
    } else {
        throw std::runtime_error(INVALID_PIN_NUMBER_STRING + pinNumber);
    }
}

std::string ArduinoUno::gpioAliasByPinNumber(int number) const
{
    if (this->m_gpioPinIterationAliasMap.find(number) != this->m_gpioPinIterationAliasMap.end()) {
        return this->m_gpioPinIterationAliasMap.find(number)->second;
    } else {
        throw std::runtime_error("");
    }
}

std::pair<IOStatus, bool> ArduinoUno::digitalRead(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return digitalRead(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_DIGITAL_READ_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> ArduinoUno::digitalWrite(const std::string &pinAlias, bool state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return digitalWrite(this->m_gpioPinsAlias.find(pinAlias)->first, state);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_DIGITAL_WRITE_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, double> ArduinoUno::analogRead(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return analogRead(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_ANALOG_READ_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, int> ArduinoUno::analogReadRaw(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return analogReadRaw(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_ANALOG_READ_RAW_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, double> ArduinoUno::analogWrite(const std::string &pinAlias, double state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return analogWrite(this->m_gpioPinsAlias.find(pinAlias)->first, state);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_ANALOG_WRITE_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, int> ArduinoUno::analogWriteRaw(const std::string &pinAlias, int state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return analogWriteRaw(this->m_gpioPinsAlias.find(pinAlias)->first, state);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_ANALOG_WRITE_RAW_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> ArduinoUno::softDigitalRead(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return softDigitalRead(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_SOFT_DIGITAL_READ_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, double> ArduinoUno::softAnalogRead(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return softAnalogRead(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_SOFT_ANALOG_READ_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, int> ArduinoUno::softAnalogReadRaw(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return softAnalogReadRaw(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_SOFT_ANALOG_READ_RAW_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, IOType> ArduinoUno::pinMode(const std::string &pinAlias, IOType ioType)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return pinMode(this->m_gpioPinsAlias.find(pinAlias)->first, ioType);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_PIN_MODE_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, IOType> ArduinoUno::currentPinMode(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPinsAlias.find(pinAlias) != this->m_gpioPinsAlias.end()) {
        return currentPinMode(this->m_gpioPinsAlias.find(pinAlias)->first);
    } else {
        throw std::runtime_error(INVALID_PIN_ALIAS_STRING + pinAlias + PASSED_TO_CURRENT_PIN_MODE_STRING_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> ArduinoUno::digitalRead(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::digitalRead(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_DIGITAL_READ_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> ArduinoUno::digitalWrite(const GPIO &gpio, bool state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::digitalWrite(gpio.pinNumber(), state, this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_DIGITAL_WRITE_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, double> ArduinoUno::analogRead(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::analogRead(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_ANALOG_READ_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, int> ArduinoUno::analogReadRaw(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::analogReadRaw(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_ANALOG_READ_RAW_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, double> ArduinoUno::analogWrite(const GPIO &gpio, double state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::analogWrite(gpio.pinNumber(), state, this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_ANALOG_WRITE_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, int> ArduinoUno::analogWriteRaw(const GPIO &gpio, int state)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::analogWriteRaw(gpio.pinNumber(), state, this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_ANALOG_WRITE_RAW_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> ArduinoUno::softDigitalRead(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::softDigitalRead(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_SOFT_DIGITAL_READ_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, double> ArduinoUno::softAnalogRead(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::softAnalogRead(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_SOFT_ANALOG_READ_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, int> ArduinoUno::softAnalogReadRaw(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::softAnalogReadRaw(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_SOFT_ANALOG_READ_RAW_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, IOType> ArduinoUno::pinMode(GPIO &gpio, IOType ioType)
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

std::pair<IOStatus, IOType> ArduinoUno::currentPinMode(const GPIO &gpio)
{
    using namespace ArduinoPCStrings;
    if (this->m_gpioPins.find(gpio.pinNumber()) != this->m_gpioPins.end()) {
        return Arduino::currentPinMode(gpio.pinNumber(), this->m_serialPortIndex);
    } else {
        throw std::runtime_error(INVALID_GPIO_PIN_STRING + std::to_string(gpio.pinNumber()) + PASSED_TO_CURRENT_PIN_MODE_GPIO_TAIL_STRING);
    }
}

std::pair<IOStatus, bool> ArduinoUno::digitalRead(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return digitalRead(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_DIGITAL_READ_STRING);
    }
}

std::pair<IOStatus, bool> ArduinoUno::digitalWrite(std::shared_ptr<GPIO> gpioPtr, bool state)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return digitalWrite(*gpioPtr, state);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_DIGITAL_WRITE_STRING);
    }
}

std::pair<IOStatus, double> ArduinoUno::analogRead(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return analogRead(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_ANALOG_READ_STRING);
    }
}

std::pair<IOStatus, int> ArduinoUno::analogReadRaw(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return analogReadRaw(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_ANALOG_READ_RAW_STRING);
    }
}

std::pair<IOStatus, double> ArduinoUno::analogWrite(std::shared_ptr<GPIO> gpioPtr, double state)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return analogWrite(*gpioPtr, state);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_ANALOG_WRITE_STRING);
    }
}

std::pair<IOStatus, int> ArduinoUno::analogWriteRaw(std::shared_ptr<GPIO> gpioPtr, int state)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return analogWriteRaw(*gpioPtr, state);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_ANALOG_WRITE_RAW_STRING);
    }
}

std::pair<IOStatus, bool> ArduinoUno::softDigitalRead(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return softDigitalRead(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_SOFT_DIGITAL_READ_STRING);
    }
}

std::pair<IOStatus, double> ArduinoUno::softAnalogRead(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return softAnalogRead(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_SOFT_ANALOG_READ_STRING);
    }
}

std::pair<IOStatus, int> ArduinoUno::softAnalogReadRaw(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return softAnalogReadRaw(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_SOFT_ANALOG_READ_RAW_STRING);
    }
}

std::pair<IOStatus, IOType> ArduinoUno::pinMode(std::shared_ptr<GPIO> gpioPtr, IOType ioType)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return pinMode(*gpioPtr, ioType);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_PIN_MODE_STRING);
    }
}

std::pair<IOStatus, IOType> ArduinoUno::currentPinMode(std::shared_ptr<GPIO> gpioPtr)
{
    using namespace ArduinoPCStrings;
    if (gpioPtr) {
        return currentPinMode(*gpioPtr);
    } else {
        throw std::runtime_error(NULL_GPIO_PTR_TO_CURRENT_PIN_MODE_STRING);
    }
}


void ArduinoUno::onPinModeImplicitlyChanged(int pinNumber, IOType newIOType)
{
    (void)pinNumber;
    (void)newIOType;
}

void ArduinoUno::onPinModeImplicitlyChanged(const std::string &pinAlias, IOType newIOType)
{
    (void)pinAlias;
    (void)newIOType;
}

void ArduinoUno::onPinModeExplicitlyChanged(int pinNumber, IOType newIOType)
{
    (void)pinNumber;
    (void)newIOType;
}

void ArduinoUno::onPinModeExplicitlyChanged(const std::string &pinAlias, IOType newIOType)
{
    (void)pinAlias;
    (void)newIOType;
}

bool ArduinoUno::isValidAnalogPinIdentifier(const std::string &state) const
{
    for (auto &it : ArduinoUno::s_availableAnalogPins) {
        if (state == analogPinFromNumber(it)) {
            return true;
        }
    }
    return false;
}

bool ArduinoUno::isValidDigitalOutputPin(int pinNumber) const
{
    for (auto &it : ArduinoUno::s_availableAnalogPins) {
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

bool ArduinoUno::isValidDigitalInputPin(int pinNumber) const
{
    for (auto &it : ArduinoUno::s_availableAnalogPins) {
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

bool ArduinoUno::isValidAnalogOutputPin(int pinNumber) const
{
    for (auto &it : ArduinoUno::s_availablePwmPins) {
        if (pinNumber == it) {
            return true;
        }
    }
    return false;
}

bool ArduinoUno::isValidAnalogInputPin(int pinNumber) const
{
    for (auto &it : ArduinoUno::s_availableAnalogPins) {
        if (pinNumber == it) {
            return true;
        }
    }
    return false;
}

std::set<int> ArduinoUno::AVAILABLE_ANALOG_PINS() const
{
    return ArduinoUno::s_availableAnalogPins;
}

std::set<int> ArduinoUno::AVAILABLE_PWM_PINS() const
{
    return ArduinoUno::s_availablePwmPins;
}

std::set<int> ArduinoUno::AVAILABLE_PINS() const
{
    return ArduinoUno::s_availablePins;
}

const char *ArduinoUno::ARDUINO_TYPE_IDENTIFIER_FUNC() const
{
    return ArduinoUno::ARDUINO_TYPE_IDENTIFIER;
}

int ArduinoUno::NUMBER_OF_DIGITAL_PINS() const
{
    return ArduinoUno::s_numberOfDigitalPins;
}

int ArduinoUno::parseAnalogPin(const std::string &pinAlias) const
{
    return ArduinoUno::staticParseAnalogPin(pinAlias);
}

std::string ArduinoUno::analogPinFromNumber(int pinNumber) const
{
    return ArduinoUno::staticAnalogPinFromNumber(pinNumber);
}


int ArduinoUno::staticParseAnalogPin(const std::string &pinAlias)
{
    using namespace ArduinoPCStrings;
    if ((pinAlias == UNO_A0_STRING) || (pinAlias == UNO_A0_EQUIVALENT_STRING)) {
        return 14;
    } else if ((pinAlias == UNO_A1_STRING) || (pinAlias == UNO_A1_EQUIVALENT_STRING)) {
        return 15;
    } else if ((pinAlias == UNO_A2_STRING) || (pinAlias == UNO_A2_EQUIVALENT_STRING)) {
        return 16;
    } else if ((pinAlias == UNO_A3_STRING) || (pinAlias == UNO_A3_EQUIVALENT_STRING)) {
        return 17;
    } else if ((pinAlias == UNO_A4_STRING) || (pinAlias == UNO_A4_EQUIVALENT_STRING)) {
        return 18;
    } else if ((pinAlias == UNO_A5_STRING) || (pinAlias == UNO_A5_EQUIVALENT_STRING)) {
        return 19;
    } else {
        throw std::logic_error(INVALID_ANALOG_PIN_BASE_STRING + pinAlias + UNO_INVALID_ANALOG_STRING_TAIL_STRING);
    }
}

std::string ArduinoUno::staticAnalogPinFromNumber(int pinNumber)
{
    using namespace ArduinoPCStrings;
    if (pinNumber == 14) {
        return UNO_A0_STRING;
    } else if (pinNumber == 15) {
        return UNO_A1_STRING;
    } else if (pinNumber == 16) {
        return UNO_A2_STRING;
    } else if (pinNumber == 17) {
        return UNO_A3_STRING;
    } else if (pinNumber == 18) {
        return UNO_A4_STRING;
    } else if (pinNumber == 19) {
        return UNO_A5_STRING;
    } else {
        throw std::runtime_error(INVALID_ANALOG_PIN_BASE_STRING + std::to_string(pinNumber) + UNO_INVALID_ANALOG_INT_TAIL_STRING);
    }
}
