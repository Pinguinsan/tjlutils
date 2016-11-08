#include "gpio.h"

GPIO::GPIO(int pinNumber, IOType ioType) :
    m_pinNumber{pinNumber},
    m_ioType{ioType}
{

}

void GPIO::setIOType(IOType ioType)
{
    this->m_ioType = ioType;
}

int GPIO::pinNumber() const
{
    return this->m_pinNumber;
}

IOType GPIO::ioType() const
{
    return this->m_ioType;
}
