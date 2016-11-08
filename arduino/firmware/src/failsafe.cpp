#include "../include/failsafe.h"

FailSafe::FailSafe(GPIO *gpio, bool state) :
    m_gpio{gpio},
    m_state{state}
{
    
}

FailSafe::FailSafe() :
    m_gpio{nullptr},
    m_state{false}
{
    
}    
    
void FailSafe::setGPIO(GPIO *gpio)
{
    this->m_gpio = gpio;
}

void FailSafe::setState(bool state)
{
    this->m_state = state;
}

bool FailSafe::state() const
{
    return this->m_state;
}

GPIO *FailSafe::gpio() const
{
    return this->m_gpio;
}