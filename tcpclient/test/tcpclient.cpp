#include<sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <string.h>
#include <iostream>
#include <string>
 
int main(int argc , char *argv[])
{
    static const unsigned int constexpr REPLY_BUFFER_MAX{100000};
    std::string hostName{"127.0.0.1"};
    struct sockaddr_in socketAddress{};
    std::string receivedMessage{""};

    char replyFromServer[REPLY_BUFFER_MAX];
     
    //Create socket
    int setSocketResult{socket(AF_INET , SOCK_STREAM , 0)};
    if (setSocketResult == -1) {
        printf("Could not create socket");
    }
    std::cout << "Socket created successfully" << std::endl;
     
    socketAddress.sin_addr.s_addr = inet_addr(hostName.c_str());
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(setSocketResult , (struct sockaddr *)&socketAddress , sizeof(socketAddress)) < 0) {
        std::cout << "Connection failed" << std::endl;
        return 1;
    }
     
    std::cout << "Connection successful" << std::endl;
     
    //keep communicating with server
    do {
        memset(replyFromServer, 0, sizeof(replyFromServer));
        std::string userInput{""};
        std::cout << "Enter message to send, or press CTRL+C to quit: ";
        std::getline(std::cin, userInput);
        if (userInput.empty()) {
            continue;
        }
        
        //Send some data
        if( send(setSocketResult, userInput.c_str(), userInput.length(), 0) < 0) {
            std::cout << "Failed to send data" << std::endl;
            return 1;
        }
        sockaddr_in receivedAddress{};
        unsigned int socketSize{sizeof(sockaddr)};
        //Receive a reply from the server
        auto result = recvfrom(setSocketResult,
                      replyFromServer,
                      sizeof(replyFromServer)-1,
                      0,
                      (sockaddr *)&receivedAddress,
                      socketSize);
        if (result < 0) {
            std::cout << "Failed to receive data" << std::endl;
            return 1;
        }
         
        std::cout << "Received: " << std::string{replyFromServer} << std::endl;
    } while (true);
     
    close(setSocketResult);
    return 0;