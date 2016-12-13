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
    std::unique_ptr<Arduino> arduino{std::make_unique<Arduino>(ArduinoType::NANO, "/dev/ttyUSB0")};
    do {
        for (auto &it : pins) {
            std::cout << "Writing pin " << it << " LOW...";
            std::cout << (arduino->digitalWrite(it, 0).first == IOStatus::OPERATION_SUCCESS ? "success" : "failure") << std::endl;

            GeneralUtilities::delayMilliseconds(300);
            
            std::cout << "Writing pin " << it << " HIGH...";
            std::cout << (arduino->digitalWrite(it, 1).first == IOStatus::OPERATION_SUCCESS ? "success" : "failure") << std::endl;

        }
        for (int i = 0; i < 3; i++) {
            for (auto &it : pins) {
               std::cout << "Writing pin " << it << " LOW...";
               std::cout << (arduino->digitalWrite(it, 0).first == IOStatus::OPERATION_SUCCESS ? "success" : "failure") << std::endl;
            }
            GeneralUtilities::delayMilliseconds(300);
            for (auto &it : pins) {
                std::cout << "Writing pin " << it << " HIGH...";
                std::cout << (arduino->digitalWrite(it, 1).first == IOStatus::OPERATION_SUCCESS ? "success" : "failure") << std::endl;
            }
        }
    } while(true);
    return 0;
}
