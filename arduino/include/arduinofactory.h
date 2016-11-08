#ifndef TJLUTILS_ARDUINOFACTORY_H
#define TJLUTILS_ARDUINOFACTORY_H

#include <functional>
#include <serialport.h>

#include "arduino.h"
#include "arduinouno.h"
#include "arduinonano.h"
#include "arduinomega.h"
#include "globallogger.h"
#include "arduinopcstrings.h"


class ArduinoFactory
{
public:
    ArduinoFactory() = delete;
    static std::shared_ptr<Arduino> makeArduino(std::shared_ptr<SerialPort> serialPort);

};

#endif //TJLUTILS_ARDUINOFACTORY_H
