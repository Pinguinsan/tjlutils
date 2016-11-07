#ifndef ARDUINOPC_ARDUINONANO_H
#define ARDUINOPC_ARDUINONANO_H

#include <list>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <chrono>
#include <tuple>
#include <mutex>
#include <utility>

#include <generalutilities.h>
#include <mathutilities.h>
#include <serialport.h>


#include <generalutilities.h>
#include <mathutilities.h>
#include <fileutilities.h>
#include <serialport.h>

#include "arduino/arduino.h"
#include "arduino/gpio.h"

#include "manualscreen/ioreport.h"

#include "configurationfilereader.h"
#include "mainwindow.h"
#include "globallogger.h"


class ArduinoNano : public Arduino
{

public:
    ArduinoNano(std::shared_ptr<SerialPort> serialPort, std::shared_ptr<GlobalLogger> globalLogger = nullptr);
    ArduinoNano(std::shared_ptr<SerialPort> serialPort, bool canCapability, std::shared_ptr<GlobalLogger> globalLogger = nullptr);
    ArduinoNano(std::shared_ptr<SerialPort> serialPort, const std::string &firmwareVersion, std::shared_ptr<GlobalLogger> globalLogger = nullptr);
    ArduinoNano(std::shared_ptr<SerialPort> serialPort, const std::string &firmwareVersion, bool canCapability, std::shared_ptr<GlobalLogger> globalLogger = nullptr);

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }

    std::pair<IOStatus, bool> digitalRead(const std::string &pinAlias) override;
    std::pair<IOStatus, bool> digitalWrite(const std::string &pinAlias, bool state) override;
    std::pair<IOStatus, double> analogRead(const std::string &pinAlias) override;
    std::pair<IOStatus, int> analogReadRaw(const std::string &pinAlias) override;
    std::pair<IOStatus, double> analogWrite(const std::string &pinAlias, double state) override;
    std::pair<IOStatus, int> analogWriteRaw(const std::string &pinAlias, int state) override;
    std::pair<IOStatus, bool> softDigitalRead(const std::string &pinAlias) override;
    std::pair<IOStatus, double> softAnalogRead(const std::string &pinAlias) override;
    std::pair<IOStatus, int> softAnalogReadRaw(const std::string &pinAlias) override;
    std::pair<IOStatus, IOType> pinMode(const std::string &pinAlias, IOType ioType) override;
    std::pair<IOStatus, IOType> currentPinMode(const std::string &pinAlias) override;

    std::pair<IOStatus, bool> digitalRead(const GPIO &gpio) override;
    std::pair<IOStatus, bool> digitalWrite(const GPIO &gpio, bool state) override;
    std::pair<IOStatus, double> analogRead(const GPIO &gpio) override;
    std::pair<IOStatus, int> analogReadRaw(const GPIO &gpio) override;
    std::pair<IOStatus, double> analogWrite(const GPIO &gpio, double state) override;
    std::pair<IOStatus, int> analogWriteRaw(const GPIO &gpio, int state) override;
    std::pair<IOStatus, bool> softDigitalRead(const GPIO &gpio) override;
    std::pair<IOStatus, double> softAnalogRead(const GPIO &gpio) override;
    std::pair<IOStatus, int> softAnalogReadRaw(const GPIO &gpio) override;
    std::pair<IOStatus, IOType> pinMode(GPIO &gpio, IOType ioType) override;
    std::pair<IOStatus, IOType> currentPinMode(const GPIO &gpio) override;
    void writeRawString(const std::string &str) override;
    std::string readRawString() override;
    void flushRX() override;
    void flushTX() override;
    void flushRXTX() override;
    void flushTXRX() override;

    std::pair<IOStatus, bool> digitalRead(std::shared_ptr<GPIO> gpioPtr) override;
    std::pair<IOStatus, bool> digitalWrite(std::shared_ptr<GPIO> gpioPtr, bool state) override;
    std::pair<IOStatus, double> analogRead(std::shared_ptr<GPIO> gpioPtr) override;
    std::pair<IOStatus, int> analogReadRaw(std::shared_ptr<GPIO> gpioPtr) override;
    std::pair<IOStatus, double> analogWrite(std::shared_ptr<GPIO> gpioPtr, double state) override;
    std::pair<IOStatus, int> analogWriteRaw(std::shared_ptr<GPIO> gpioPtr, int state) override;
    std::pair<IOStatus, bool> softDigitalRead(std::shared_ptr<GPIO> gpioPtr) override;
    std::pair<IOStatus, double> softAnalogRead(std::shared_ptr<GPIO> gpioPtr) override;
    std::pair<IOStatus, int> softAnalogReadRaw(std::shared_ptr<GPIO> gpioPtr) override;
    std::pair<IOStatus, IOType> pinMode(std::shared_ptr<GPIO> gpioPtr, IOType ioType) override;
    std::pair<IOStatus, IOType> currentPinMode(std::shared_ptr<GPIO> gpioPtr) override;

    std::string firmwareVersion() override;
    std::pair<bool, bool> canCapability() override;
    std::shared_ptr<SerialPort> serialPort() const override;
    bool isValidAnalogPinIdentifier(const std::string &state) const override;
    void eraseCanPin();
    bool isBluetooth() const override;

    std::shared_ptr<GPIO> gpioPinByPinAlias(const std::string &pinAlias) const override;
    std::shared_ptr<GPIO> gpioPinByPinNumber(int pinNumber) const override;
    std::string gpioAliasByPinNumber(int number) const override;
    void assignIOTypes(std::shared_ptr<GlobalLogger> globalLogger = nullptr) override;

    bool isValidDigitalOutputPin(int pinNumber) const override;
    bool isValidDigitalInputPin(int pinNumber) const override;
    bool isValidAnalogOutputPin(int pinNumber) const override;
    bool isValidAnalogInputPin(int pinNumber) const override;

    int parseAnalogPin(const std::string &pinAlias) const override;
    std::string analogPinFromNumber(int pinNumber) const override;
    static int staticParseAnalogPin(const std::string &pinAlias);
    static std::string staticAnalogPinFromNumber(int pinNumber);

    void setFirmwareVersion(const std::string &firmwareVersion) override;
    void setCanCapability(const std::pair<bool, bool> &capability) override;
    void setAnalogToDigitalThreshold(int state) override;
    int analogToDigitalThreshold() override;
    bool setPinAlias(int pinNumber, const std::string &newAlias) override;
    bool setPinAlias(const GPIO &gpio, const std::string &newAlias) override;
    bool setPinAlias(std::shared_ptr<GPIO> gpio, const std::string &newAlias) override;

    bool confirmValidAlias(const std::pair<int, std::string> &alias) const override;
    bool confirmValidIOType(const std::pair<int, std::string> &io) const override;
    bool confirmValidStates(const std::pair<int, std::string> &alias) const override;

    std::set<int> AVAILABLE_ANALOG_PINS() const override;
    std::set<int> AVAILABLE_PWM_PINS() const override;
    std::set<int> AVAILABLE_PINS() const override;
    int NUMBER_OF_DIGITAL_PINS() const override;
    const char *ARDUINO_TYPE_IDENTIFIER_FUNC() const override;
    static const char *ARDUINO_TYPE_IDENTIFIER;

private slots:
    void onPinModeImplicitlyChanged(int pinNumber, IOType newIOType);
    void onPinModeImplicitlyChanged(const std::string &pinAlias, IOType newIOType);
    void onPinModeExplicitlyChanged(int pinNumber, IOType newIOType);
    void onPinModeExplicitlyChanged(const std::string &pinAlias, IOType newIOType);


private:
    std::map<std::string, std::shared_ptr<GPIO>> m_gpioPinsAlias;
    std::map<int, std::string> m_gpioPinIterationAliasMap;
    std::map<int, std::shared_ptr<GPIO>> m_gpioPins;
    int m_serialPortIndex;
    std::string m_firmwareVersion;
    std::pair<bool, bool> m_canCapability;
    std::string m_canPinAlias;
    std::set<int> m_availablePins;
    std::set<int> m_availablePwmPins;
    std::set<int> m_availableAnalogPins;
    int m_numberOfDigitalPins;
    int m_analogToDigitalThreshold;

    static std::set<int> s_availableAnalogPins;
    static std::set<int> s_availablePwmPins;
    static std::set<int> s_availablePins;
    static int s_numberOfDigitalPins;

    void initializeIO(std::shared_ptr<GlobalLogger> globalLogger = nullptr);

};

#endif //ARDUINOPC_ARDUINONANO_H
