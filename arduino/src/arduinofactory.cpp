#include "arduinofactory.h"

std::shared_ptr<Arduino> ArduinoFactory::makeArduino(std::shared_ptr<SerialPort> serialPort)
{
    using namespace GeneralUtilities;
    std::string arduinoType{""};
    for (unsigned int tryCount = 0; tryCount <  Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT(); tryCount++) {
        try {
            serialPort->writeString(static_cast<std::string>(Arduino::CAN_LIVE_UPDATE_HEADER) + ":0}");
            delayMilliseconds(Arduino::isBluetooth(serialPort->portName()) ? Arduino::BLUETOOTH_SERIAL_SEND_DELAY*3 : Arduino::BLUETOOTH_SERIAL_SEND_DELAY);
            std::pair<IOStatus, std::string> retrieveArduinoType{Arduino::getArduinoType(serialPort)};
            if (retrieveArduinoType.first == IOStatus::OPERATION_SUCCESS) {
                arduinoType = static_cast<std::string>(retrieveArduinoType.second);
                if (arduinoType == static_cast<std::string>(ArduinoUno::ARDUINO_TYPE_IDENTIFIER)) {
                    tryCount =  Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT();
                    arduinoType = ARDUINO_UNO_IDENTIFIER;
                } else if (arduinoType == static_cast<std::string>(ArduinoNano::ARDUINO_TYPE_IDENTIFIER)) {
                    tryCount =  Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT();
                    arduinoType = ARDUINO_NANO_IDENTIFIER;
                } else if (arduinoType == static_cast<std::string>(ArduinoMega::ARDUINO_TYPE_IDENTIFIER)) {
                    tryCount = Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT();
                    arduinoType = ARDUINO_MEGA_IDENTIFIER;
                } else {
                    if (tryCount == ( Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT()-1)) {
                        return nullptr;
                    }
                }
            } else {
                if (tryCount == ( Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT()-1)) {
                    return nullptr;
                }
            }
        } catch (std::exception &e) {
            if (tryCount == ( Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT()-1)) {
                return nullptr;
            }
        }
    }
    std::pair<IOStatus, std::string> firmwareVersion;
    for (unsigned int tryCount = 0; tryCount <  Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT(); tryCount++) {
        try {
            firmwareVersion = Arduino::getFirmwareVersion(serialPort);
            if (firmwareVersion.first == IOStatus::OPERATION_SUCCESS) {
                tryCount =  Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT();
            } else {
                if (tryCount == ( Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT()-1)) {
                    return nullptr;
                }
            }
        } catch (std::exception &e) {
            if (tryCount == ( Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT()-1)) {
                return nullptr;
            }
        }
    }

    std::pair<IOStatus, int> analogToDigitalThreshold;
    for (unsigned int tryCount = 0; tryCount <  Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT(); tryCount++) {
        try {
            analogToDigitalThreshold = Arduino::getAnalogToDigitalThreshold(serialPort);
            if (analogToDigitalThreshold.first == IOStatus::OPERATION_SUCCESS) {
                tryCount =  Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT();
            } else {
                if (tryCount == ( Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT()-1)) {
                    return nullptr;
                }
            }
        } catch (std::exception &e) {
            if (tryCount == ( Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT()-1)) {
                return nullptr;
            }
        }
    }
    std::pair<IOStatus, bool> can;
    for (unsigned int tryCount = 0; tryCount <  Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT(); tryCount++) {
        try {
            can = Arduino::getCanCapability(serialPort);
            if (can.first == IOStatus::OPERATION_SUCCESS) {
                tryCount = Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT();
            }
        } catch (std::exception &e) {
            (void)e;
        }
    }
    if (firmwareVersion.first == IOStatus::OPERATION_FAILURE) {
        return nullptr;
    }
    if (can.first == IOStatus::OPERATION_FAILURE) {
        return nullptr;
    }
    if (arduinoType == "") {
        return nullptr;
    }
    if (analogToDigitalThreshold.first == IOStatus::OPERATION_FAILURE) {
        return nullptr;
    }

    if (arduinoType == ARDUINO_UNO_IDENTIFIER) {
        return std::make_shared<ArduinoUno>(serialPort, firmwareVersion.second, can.second, globalLogger);
    } else if (arduinoType == ARDUINO_NANO_IDENTIFIER) {
        return std::make_shared<ArduinoNano>(serialPort, firmwareVersion.second, can.second, globalLogger);
    } else if (arduinoType == ARDUINO_MEGA_IDENTIFIER) {
        return std::make_shared<ArduinoMega>(serialPort, firmwareVersion.second, can.second, globalLogger);
    } else {
        return nullptr;
    }
}
