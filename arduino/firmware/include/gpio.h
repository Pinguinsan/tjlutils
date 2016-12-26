#ifndef ARDUINOPC_GPIO_H
#define ARDUINOPC_GPIO_H

#include <Arduino.h>

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
#include <vector>

#include "firmwareutilities.h"
#include "fastio.h"

#ifndef HIGH
    #define HIGH true
#endif
#ifndef LOW
    #define LOW false
#endif

enum class IOType { DIGITAL_INPUT, DIGITAL_OUTPUT, ANALOG_INPUT, ANALOG_OUTPUT, DIGITAL_INPUT_PULLUP, UNSPECIFIED };

class GPIO
{
public:
    GPIO(short pinNumber, IOType ioType);
    bool g_digitalRead();
    bool g_softDigitalRead();
    void g_digitalWrite(bool logicState);
    short g_analogRead();
    short g_softAnalogRead();
    void g_analogWrite(short state);
    
    IOType ioType() const;
    short pinNumber() const;
    
    void setIOType(IOType ioType);
    void setPinNumber(short pinNumber);

    short getIOAgnosticState();
    std::vector<unsigned char> getEEPROMWritableState();
    static std::vector<unsigned char> toEEPROMWritableState(short longState);

    static const short ANALOG_MAX;
    static void setAnalogToDigitalThreshold(short threshold);
    static short analogToDigitalThreshold();

    friend bool operator==(const GPIO &lhs, const GPIO &rhs)
    {
        return ((lhs.m_pinNumber == rhs.m_pinNumber) &&
                (lhs.m_ioType == rhs.m_ioType));
    }

    friend bool operator<(const GPIO &lhs, const GPIO &rhs)
    {
        return (lhs.m_pinNumber < rhs.m_pinNumber);
    }

private:
    short m_pinNumber;
    IOType m_ioType;
    bool m_logicState;
    short m_analogState;

    static short s_analogToDigitalThreshold;
};

#endif //ARDUINOPC_GPIO_H