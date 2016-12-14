#include <iostream>
#include <string>
#include <udpserver.h>

int main()
{
    UDPServer udpServer{8888};

    do {
        if (udpServer.available()) {
            std::cout << "Rx << " << udpServer.readString() << std::endl;
        }
    } while (true);
    return 0;

}
