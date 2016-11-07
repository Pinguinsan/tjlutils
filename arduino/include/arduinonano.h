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

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }



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
