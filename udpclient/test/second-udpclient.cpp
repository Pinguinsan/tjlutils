#include <iostream>
#include <udpclient.h>
#include <thread>
#include <chrono>
#include <string>

int main()
{

    UDPClient udpClient{"pinguinsan1224.asuscomm.com", 62004};
    while (true) {
        udpClient.writeString("Hello");
        std::this_thread::sleep_for(std::chrono::milliseconds(750));
    }
    return 0;
}
