#include <iostream>
#include <string>
#include <vector>
#include <generalutilities.h>
#include <arduino.h>

int main()
{
    //std::string serialPortName{SerialPort::doUserSelectSerialPortName()};
    //std::unique_ptr<Arduino> arduino{std::make_unique<Arduino>(ArduinoType::NANO, serialPortName)};

    std::vector<int> pins{2, 3, 4, 5, 6};
    const std::string SERIAL_PORT_NAME{"/dev/ttyACM0"};
    std::cout << "Creating Arduino object using serial port " << std::quoted(SERIAL_PORT_NAME) << "..."; 
    std::unique_ptr<Arduino> arduino{std::make_unique<Arduino>(ArduinoType::MEGA, SERIAL_PORT_NAME)};
    std::cout << "success" << std::endl;
    do {
        for (auto &it : pins) {
            std::cout << "Writing pin " << it << " LOW...";
            std::cout << (arduino->digitalWrite(it, 0).first == IOStatus::OPERATION_SUCCESS ? "success" : "failure") << std::endl;

            GeneralUtilities::delayMilliseconds(300);
            
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
            GeneralUtilities::delayMilliseconds(300);
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
        }
    } while(true);
    return 0;
}
