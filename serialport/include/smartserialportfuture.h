#ifndef TJLUTILS_SMARTSERIALPORTFUTURE_H
#define TJLUTILS_SMARTSERIALPORTFUTURE_H

#include <future>
#include <string>
#include <memory>

#include "serialport.h"

class SerialPort;

class SmartSerialPortFuture
{
public:
    SmartSerialPortFuture(std::shared_ptr<SerialPort> serialPort);

    bool isCompleted();
    bool isStarted() const;

    void stop();
    void start();
    std::string result();

private:
    std::shared_ptr<SerialPort> m_serialPort;
    std::future<std::string> m_future;
    bool m_isStarted;
    bool m_isCompleted;
};

#endif //TJLUTILS_SMARTSERIALPORTFUTURE_H
