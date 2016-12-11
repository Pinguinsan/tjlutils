#ifndef ARDUINOPC_SERIALPORTINFO_H
#define ARDUINOPC_SERIALPORTINFO_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>

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
#include <serstream>
#include <algorithm>
#include <iostream>
#include <sstream>

template <typename SerialType>
class SerialPortInfo
{
public:
    SerialPortInfo(SerialType *serialPort, 
                   std::ohserialstream *outputStream, 
                   int rxPin, 
                   int txPin, 
                   long long int baudRate, 
                   long long int timeout,
                   bool enabled) :
        m_serialPort{serialPort},
        m_outputStream{outputStream},
        m_rxPin{rxPin},
        m_txPin{txPin},
        m_baudRate{baudRate},
        m_timeout{timeout},
        m_isEnabled{enabled}
    {

    }
    
    void setEnabled(bool enabled) 
    { 
        this->m_isEnabled = enabled; 
    }

    SerialType *serialPort() const 
    { 
        return this->m_serialPort; 
    }
    
    std::ohserialstream *outputStream() const 
    { 
        return this->m_outputStream;
    }
    
    int rxPin() const 
    { 
        return this->m_rxPin; 
    }
    
    int txPin() const 
    { 
        return this->m_txPin; 
    }

    long long int baudRate() const
    {
        return this->m_baudRate;
    }

    long long int timeout() const
    {
        return this->m_timeout;
    }
    
    bool isEnabled() const 
    { 
        return this->m_isEnabled; 
    }

    bool initialize()
    {
        if (this->m_serialPort) {
            this->m_serialPort->begin(this->m_baudRate);
            this->m_serialPort->setTimeout(this->m_timeout);
            return true;
        } else {
            return false;
        }
    }

private:
    SerialType *m_serialPort;
    std::ohserialstream *m_outputStream;
    int m_rxPin;
    int m_txPin;
    long long int m_baudRate;
    long long int m_timeout;
    bool m_isEnabled;

};

#endif //ARDUINOPC_SERIALPORTINFO_H