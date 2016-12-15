#include <iostream>
#include <string>
#include <vector>
#include <serialport.h>
#include <udpduplex.h>
#include <tstream.h>
#include <generalutilities.h>
#include <arduino.h>

int main()
{
    /*
    //std::string serialPortName{SerialPort::doUserSelectSerialPortName()};
    //std::unique_ptr<Arduino> arduino{std::make_unique<Arduino>(ArduinoType::NANO, serialPortName)};

    //const std::string SERIAL_PORT_NAME{"/dev/ttyACM0"};
    std::shared_ptr<TStream> serialPort{std::make_shared<SerialPort>(serialPortName,
                                                                        Arduino::FIRMWARE_BAUD_RATE,
                                                                        Arduino::FIRMWARE_DATA_BITS,
                                                                        Arduino::FIRMWARE_STOP_BITS,
                                                                        Arduino::FIRMWARE_PARITY)};
    */
    std::vector<int> pins{2, 3, 4, 5, 6};
    std::shared_ptr<TStream> serialPort{std::make_shared<UDPDuplex>("127.0.0.1", 8888, 8887)};
    std::cout << "Creating Arduino object using serial port " << std::quoted(serialPort->portName()) << "..."; 
    std::unique_ptr<Arduino> arduino{std::make_unique<Arduino>(ArduinoType::MEGA, serialPort)};
    std::cout << "success" << std::endl;
    arduino->setStreamSendDelay(125);
    serialPort->openPort();
    std::cout << "Hello" << std::endl;
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
        GeneralUtilities::delayMilliseconds(500);
        for (auto &it : pins) {
            std::cout << "Writing pin " << it << " LOW...";
            std::cout << (arduino->digitalWrite(it, 0).first == IOStatus::OPERATION_SUCCESS ? "success" : "failure") << std::endl;

            GeneralUtilities::delayMilliseconds(500);
            
            std::cout << "Writing pin " << it << " HIGH...";
            std::cout << (arduino->digitalWrite(it, 1).first == IOStatus::OPERATION_SUCCESS ? "success" : "failure") << std::endl;

        }
        for (int i = 0; i < 3; i++) {
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
            GeneralUtilities::delayMilliseconds(500);
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
            GeneralUtilities::delayMilliseconds(500);
        }
    } while(true);
    return 0;
}
