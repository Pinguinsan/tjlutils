#include <iostream>
#include <thread>
#include <chrono>
#include <udpclient.h>

int main()
{

    UDPClient udpClient{"pinguinsan1224.asuscomm.com", 62004};
    //UDPClient udpClient{"127.0.0.1", 8888};
    do {
        auto result = udpClient.writeString("Hello, World");
        std::cout << "Sent " << result << " bytes" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } while (true);
    return 0;
}
