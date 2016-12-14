

#include <iostream>
#include <string>
#include <udpserver.h>
int main()
{
    UDPServer udpServer{8888};
    udpServer.startListening();
    
    do {
        if (udpServer.available()) {
            std::cout << "Rx << " << udpServer.readString() << std::endl;
        }
    } while (true);
    return 0;

}
/*

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <mutex>
#include <future>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main()
{
    sockaddr_in si_me, si_other;
    int s;
    assert((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))!=-1);
    int port=8888;
    int broadcast=1;

    setsockopt(s, SOL_SOCKET, SO_BROADCAST,
                &broadcast, sizeof broadcast);

    memset(&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = INADDR_ANY;

    assert(::bind(s, (sockaddr *)&si_me, sizeof(sockaddr))!=-1);

    while(1)
    {
        char buf[10000];
        unsigned slen=sizeof(sockaddr);
        recvfrom(s, buf, sizeof(buf)-1, 0, (sockaddr *)&si_other, &slen);

        printf("recv: %s\n", buf);
    }
}
*/
