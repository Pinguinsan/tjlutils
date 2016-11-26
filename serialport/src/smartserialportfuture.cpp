#include "smartserialportfuture.h"

SmartSerialPortFuture::SmartSerialPortFuture(std::shared_ptr<SerialPort> serialPort) :
    m_serialPort{serialPort},
    m_isStarted{false},
    m_isCompleted{false}
{

}

bool SmartSerialPortFuture::isStarted() const
{
    return this->m_isStarted;
}

bool SmartSerialPortFuture::isCompleted()
{
    if (this->m_isStarted) {
        this->m_isCompleted = (this->m_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready);
        if (this->m_isCompleted) {
            this->m_isStarted = false;
        }
        return this->m_isCompleted;
    } else {
        return false;
    }
}

void SmartSerialPortFuture::start()
{
    if (this->m_isStarted) {
        this->m_future = std::async(std::launch::async, this->m_serialPort->readString);
        this->m_isStarted = true;
        this->m_isCompleted = false;
        throw std::runtime_error();
    } else {
        this->m_future = std::async(std::launch::async, this->m_serialPort->readString);
        this->m_isStarted = true;
        this->m_isCompleted = false;
    }
}

std::string SmartSerialPortFuture::result()
{
    if (this->m_isCompleted) {
        try {
            return this->m_future.get();
        } catch (std::exception &e) {
            throw e;
        }
    } else {
        throw std::runtime_error();
    }
}

void SmartSerialPortFuture::stop()
{
    this->m_isStarted = false;
    this->m_isCompleted = false;
}

