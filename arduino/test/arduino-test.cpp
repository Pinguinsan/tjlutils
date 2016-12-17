#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <serialport.h>
#include <udpduplex.h>
#include <tstream.h>
#include <generalutilities.h>
#include <arduino.h>


template <typename T>
bool alwaysTrue(T param)
{
    (void)param;
    return true;
}

int main()
{
    /*
    //std::string serialPortName{SerialPort::doUserSelectSerialPortName()};
    //std::unique_ptr<Arduino> arduino{std::make_unique<Arduino>(ArduinoType::NANO, serialPortName)};

    //const std::string SERIAL_PORT_NAME{"/dev/ttyACM0"};
    //std::shared_ptr<TStream> serialPort{std::make_shared<SerialPort>(serialPortName,
                                                                        Arduino::FIRMWARE_BAUD_RATE,
                                                                        Arduino::FIRMWARE_DATA_BITS,
                                                                        Arduino::FIRMWARE_STOP_BITS,
                                                                        Arduino::FIRMWARE_PARITY)};
    
    
    */    
    std::vector<int> pins{2, 3, 4, 5, 6};
    std::string clientHostName{UDPDuplex::doUserSelectClientHostName()};
    uint16_t clientPortNumber{UDPDuplex::doUserSelectClientPortNumber()};
    uint16_t serverPortNumber{UDPDuplex::doUserSelectServerPortNumber()};
    uint32_t delayBetween{GeneralUtilities::doUserEnterNumericParameter("On-Off Delay",
                                                                        static_cast<std::function<bool(uint32_t)>>(alwaysTrue<uint32_t>),
                                                                        std::numeric_limits<uint32_t>::min(),
                                                                        std::numeric_limits<uint32_t>::max())};
            
    
    uint32_t arduinoIOTryCount{GeneralUtilities::doUserEnterNumericParameter("Arduino IO Try Count",
                                                                        static_cast<std::function<bool(uint32_t)>>(alwaysTrue<uint32_t>),
                                                                        std::numeric_limits<uint32_t>::min()+1,
                                                                        std::numeric_limits<uint32_t>::max())};
            
    uint32_t arduinoStreamDelay{GeneralUtilities::doUserEnterNumericParameter("Arduino Stream Delay",
                                                                        static_cast<std::function<bool(uint32_t)>>(alwaysTrue<uint32_t>),
                                                                        std::numeric_limits<uint32_t>::min()+1,
                                                                        std::numeric_limits<uint32_t>::max())};

    std::cout << "Using ClientHostName=" << clientHostName << std::endl;
    std::cout << "Using ClientPortNumber=" << clientPortNumber << std::endl;
    std::cout << "Using ServerPortNumber=" << serverPortNumber << std::endl;
    std::cout << "Using DelayBetween=" << delayBetween << std::endl;
    std::cout << "Using ArduinoIOTryCount=" << arduinoIOTryCount << std::endl;
    std::cout << "Using ArduinoStreamDelay=" << arduinoStreamDelay << std::endl << std::endl;
    //std::shared_ptr<TStream> serialPort{SerialPort::doUserSelectSerialPort()};
    std::shared_ptr<TStream> serialPort{std::make_shared<UDPDuplex>(clientHostName, clientPortNumber, serverPortNumber, UDPObjectType::UDP_DUPLEX)};
    std::cout << "Creating Arduino object using serial port " << std::quoted(serialPort->portName()) << "..."; 
    std::unique_ptr<Arduino> arduino{std::make_unique<Arduino>(ArduinoType::MEGA, serialPort)};
    std::cout << "success" << std::endl;
    arduino->setIOTryCount(arduinoIOTryCount);
    arduino->setStreamSendDelay(arduinoStreamDelay);
    serialPort->openPort();
    do {
        std::cout << "Writing all pins HIGH...";
        auto result = arduino->digitalWriteAll(1);
        if (result.first == IOStatus::OPERATION_SUCCESS) {
            std::cout << "success (";
            for (auto it = result.second.begin(); it != result.second.end(); it++) {
                std::cout << *it;
                if (it+1 != result.second.end()) {
                    std::cout << ", ";
                }
            }
            std::cout << ")" << std::endl;
        } else {
            std::cout << "failed" << std::endl;
        }
        GeneralUtilities::delayMilliseconds(delayBetween);
        for (auto &it : pins) {
            std::cout << "Writing pin " << it << " LOW...";
            std::cout << (arduino->digitalWrite(it, 0).first == IOStatus::OPERATION_SUCCESS ? "success" : "failure") << std::endl;

            GeneralUtilities::delayMilliseconds(delayBetween);
            
            std::cout << "Writing pin " << it << " HIGH...";
            std::cout << (arduino->digitalWrite(it, 1).first == IOStatus::OPERATION_SUCCESS ? "success" : "failure") << std::endl;
            
            GeneralUtilities::delayMilliseconds(delayBetween);
        }
        for (int i = 0; i < 3; i++) {
            if (i != 0) {
                std::cout << "Writing all pins HIGH...";
                auto result = arduino->digitalWriteAll(1);
                if (result.first == IOStatus::OPERATION_SUCCESS) {
                    std::cout << "success (";
                    for (auto it = result.second.begin(); it != result.second.end(); it++) {
                        std::cout << *it;
                        if (it+1 != result.second.end()) {
                            std::cout << ", ";
                        }
                    }
                    std::cout << ")" << std::endl;
                } else {
                    std::cout << "failed" << std::endl;
                }
                GeneralUtilities::delayMilliseconds(delayBetween*2);
            }
            std::cout << "Writing all pins LOW...";
            result = arduino->digitalWriteAll(0);
            if (result.first == IOStatus::OPERATION_SUCCESS) {
                std::cout << "success (";
                for (auto it = result.second.begin(); it != result.second.end(); it++) {
                    std::cout << *it;
                    if (it+1 != result.second.end()) {
                        std::cout << ", ";
                    }
                }
                std::cout << ")" << std::endl;
            } else {
                std::cout << "failed" << std::endl;
            }
            GeneralUtilities::delayMilliseconds(delayBetween*2);
        }
    } while(true);
    return 0;
}
