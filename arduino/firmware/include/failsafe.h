#ifndef ARDUINOPC_FAILSAFE_H
#define ARDUINOPC_FAILSAFE_H

#include "gpio.h"

class FailSafe
{
public:
    FailSafe(GPIO *gpio, bool state);
    FailSafe();
    void setGPIO(GPIO *gpio);
    void setState(bool state);
    bool state() const;
    GPIO *gpio() const;

    friend bool operator==(const FailSafe &lhs, const FailSafe &rhs)
    {
        if ((!lhs.m_gpio) || (!rhs.m_gpio)) {
            return false;
        }
        return ((lhs.m_gpio->pinNumber() == rhs.m_gpio->pinNumber()) && (lhs.m_state == rhs.m_state));
    }
    
    friend bool operator<(const FailSafe &lhs, const FailSafe &rhs)
    {
        if ((!lhs.m_gpio) || (!rhs.m_gpio)) {
            return false;
        }
        return (lhs.m_gpio->pinNumber() < rhs.m_gpio->pinNumber());
    }
private:
    GPIO *m_gpio;
    bool m_state;
};

#endif //ARDUINOPC_FAILSAFE_H