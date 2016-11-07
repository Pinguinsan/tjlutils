#include "protectedserialport.h"

ProtectedSerialPort::ProtectedSerialPort(std::shared_ptr<SerialPort> serialPort) :
    m_serialPort{serialPort},
    m_ioMutex{std::make_shared<std::mutex>()}
{
    //Constructor
}

ProtectedSerialPort::~ProtectedSerialPort()
{
    //Destructor
}

std::shared_ptr<SerialPort> ProtectedSerialPort::serialPort() const
{
    return this->m_serialPort;
}

std::shared_ptr<std::mutex> ProtectedSerialPort::ioMutex() const
{
    return this->m_ioMutex;
}

