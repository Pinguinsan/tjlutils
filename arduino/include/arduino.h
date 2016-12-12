#ifndef TJLUTILS_ARDUINO_H
#define TJLUTILS_ARDUINO_H

#include <string>
#include <utility>
#include <memory>
#include <future>
#include <mutex>

#include "serialport.h"
#include "generalutilities.h"
#include "eventtimer.h"
#include "arduinohelper.h"

class ArduinoUno;
class ArduinoNano;
class ArduinoMega;
class GPIO;
class IOReport;
class SerialReport;

class Arduino
{
public:
    Arduino(ArduinoType arduinoType, const std::string &serialPortName);
    std::pair<IOStatus, bool> digitalRead(int pinNumber);
    std::pair<IOStatus, bool> digitalWrite(int pinNumber, bool state);
    std::pair<IOStatus, double> analogRead(int pinNumber);
    std::pair<IOStatus, int> analogReadRaw(int pinNumber);
    std::pair<IOStatus, double> analogWrite(int pinNumber, double state);
    std::pair<IOStatus, int> analogWriteRaw(int pinNumber, int state);
    std::pair<IOStatus, bool> softDigitalRead(int pinNumber);
    std::pair<IOStatus, double> softAnalogRead(int pinNumber);
    std::pair<IOStatus, int> softAnalogReadRaw(int pinNumber);
    std::pair<IOStatus, IOType> pinMode(int pinNumber, IOType ioType);
    std::pair<IOStatus, IOType> currentPinMode(int pinNumber);
    std::pair<IOStatus, bool> canCapability();
    std::pair<IOStatus, std::string> firmwareVersion();
    std::pair<IOStatus, std::string> arduinoTypeString();
    std::pair<IOStatus, int> analogToDigitalThreshold();
    std::pair<IOStatus, int> setAnalogToDigitalThreshold(int threshold);
    SerialReport serialReportRequest(const std::string &delimiter);
    IOReport ioReportRequest();

    std::string serialPortName() const;

    std::set<int> AVAILABLE_ANALOG_PINS() const;
    std::set<int> AVAILABLE_PWM_PINS() const;
    std::set<int> AVAILABLE_PINS() const;
    int NUMBER_OF_DIGITAL_PINS() const;
    ArduinoType arduinoType() const;
    
    std::string identifier() const;
    std::string longName() const;

    void assignPinsAndIdentifiers();
    
private:
    std::map<int, std::shared_ptr<GPIO>> m_gpioPins;
    std::unique_ptr<SerialPort> m_serialPort;
    std::mutex m_ioMutex;
    ArduinoType m_arduinoType;
    std::string m_identifier;
    std::string m_longName;
    std::set<int> m_availablePins;
    std::set<int> m_availablePwmPins;
    std::set<int> m_availableAnalogPins;
    int m_numberOfDigitalPins;

    bool isValidAnalogPinIdentifier(const std::string &state) const;
    bool isValidDigitalStateIdentifier(const std::string &state) const;
    bool isValidAnalogStateIdentifier(const std::string &state) const;
    bool isValidAnalogRawStateIdentifier(const std::string &state) const;

    bool isValidDigitalOutputPin(int pinNumber) const;
    bool isValidDigitalInputPin(int pinNumber) const;
    bool isValidAnalogOutputPin(int pinNumber) const;
    bool isValidAnalogInputPin(int pinNumber) const;

    std::vector<std::string> genericIOTask(const std::string &stringToSend, const std::string &header, double delay);
    std::vector<std::string> genericIOReportTask(const std::string &stringToSend, const std::string &header, const std::string &endHeader, double delay); 
};

class ArduinoUno
{
public:
    ArduinoUno() = delete;
    virtual void doStuff() = 0;
    static std::set<int> s_availableAnalogPins;
    static std::set<int> s_availablePwmPins;
    static std::set<int> s_availablePins;
    static const char *IDENTIFIER;
    static const char *LONG_NAME;
    static int s_numberOfDigitalPins;
};

class ArduinoNano
{
public:
    ArduinoNano() = delete;
    virtual void doStuff() = 0;
    static std::set<int> s_availableAnalogPins;
    static std::set<int> s_availablePwmPins;
    static std::set<int> s_availablePins;
    static const char *IDENTIFIER;
    static const char *LONG_NAME;
    static int s_numberOfDigitalPins;
};

class ArduinoMega
{
public:
    ArduinoMega() = delete;
    virtual void doStuff() = 0;
    static std::set<int> s_availableAnalogPins;
    static std::set<int> s_availablePwmPins;
    static std::set<int> s_availablePins;
    static const char *IDENTIFIER;
    static const char *LONG_NAME;
    static int s_numberOfDigitalPins;
};

class GPIO
{
public:
    GPIO(int pinNumber, IOType ioType) :
        m_pinNumber{pinNumber},
        m_ioType{ioType} { }
    int pinNumber() const { return this->m_pinNumber; }
    IOType ioType() const { return this->m_ioType; }
    void setIOType(IOType ioType) { this->m_ioType = ioType; }
    friend bool operator==(const GPIO &lhs, const GPIO &rhs) { return (lhs.pinNumber() == rhs.pinNumber()); }

private:
    int m_pinNumber;
    IOType m_ioType;
};

class IOReport
{
public:
    void addDigitalInputResult(const std::pair<int, bool> &result) { this->m_digitalInputResults.emplace_back(result); }
    void addDigitalOutputResult(const std::pair<int, bool> &result) { this->m_digitalOutputResults.emplace_back(result); }
    void addAnalogInputResult(const std::pair<int, int> &result) { this->m_analogInputResults.emplace_back(result); }
    void addAnalogOutputResult(const std::pair<int, int> &result) { this->m_analogOutputResults.emplace_back(result); }

    std::vector<std::pair<int, bool>> digitalInputResults() const { return this->m_digitalInputResults; }
    std::vector<std::pair<int, bool>> digitalOutputResults() const { return this->m_digitalOutputResults; }
    std::vector<std::pair<int, int>> analogInputResults() const { return this->m_analogInputResults; }
    std::vector<std::pair<int, int>> analogOutputResults() const { return this->m_analogOutputResults; }

private:
    std::vector<std::pair<int, bool>> m_digitalInputResults;
    std::vector<std::pair<int, bool>> m_digitalOutputResults;
    std::vector<std::pair<int, int>> m_analogInputResults;
    std::vector<std::pair<int, int>> m_analogOutputResults;
};

class SerialReport
{
public:
    void addSerialResult(const std::string &result) { this->m_serialResults.emplace_back(result); }
    std::vector<std::string> serialResults() const { return this->m_serialResults; }

private:
    std::vector<std::string> m_serialResults;
};

#endif