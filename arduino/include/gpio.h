#ifndef TJLUTILS_GPIO_H
#define TJLUTILS_GPIO_H

#include "arduino.h"

class GPIO
{
public:
    GPIO(int pinNumber, IOType ioType);
    int pinNumber() const;
    IOType ioType() const;
    void setIOType(IOType iotype);
    friend bool operator==(const GPIO &lhs, const GPIO &rhs)
    {
        return (lhs.pinNumber() == rhs.pinNumber());
    }

private:
    int m_pinNumber;
    IOType m_ioType;
};

#endif // TJLUTILS_GPIO_H
