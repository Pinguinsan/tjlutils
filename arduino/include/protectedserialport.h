#ifndef TJLUTILS_PROTECTEDSERIALPORT_H
#define TJLUTILS_PROTECTEDSERIALPORT_H

#include <memory>
#include <mutex>
#include "serialport.h"

class ProtectedSerialPort
{
public:
    ProtectedSerialPort(std::shared_ptr<SerialPort> serialPort);
    ~ProtectedSerialPort();
    std::shared_ptr<SerialPort> serialPort() const;
    std::shared_ptr<std::mutex> ioMutex() const;

private:
    std::shared_ptr<SerialPort> m_serialPort;
    std::shared_ptr<std::mutex> m_ioMutex;
};

#endif //TJLUTILS_PROTECTEDSERIALPORT_H
