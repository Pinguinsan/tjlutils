#ifndef ARDUINOPC_GPIO_H
#define ARDUINOPC_GPIO_H

#include <Arduino.h>

#include "firmwareutilities.h"
#include "digitalwritefast.h"

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
    GPIO(int pinNumber, IOType ioType);
    bool g_digitalRead();
    bool g_softDigitalRead();
    IOType ioType() const;
    void setPinNumber(int pinNumber);

    void g_digitalWrite(bool logicState);

    int g_analogRead();
    int g_softAnalogRead();

    void g_analogWrite(int state);
    void setIOType(IOType ioType);
    int pinNumber() const;

    static const int ANALOG_MAX;
    static void setAnalogToDigitalThreshold(int threshold);
    static int analogToDigitalThreshold();
    

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
    int m_pinNumber;
    IOType m_ioType;
    bool m_logicState;
    int m_analogState;

    static int s_analogToDigitalThreshold;
};

#endif //ARDUINOPC_GPIO_H