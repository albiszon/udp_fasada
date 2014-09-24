#include <iostream>
#include <cstring>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <netinet/in.h>

#include "SimpleUDP.h"

using namespace std;

class Board
{
    protected:
        SimpleUDP *udp;
    public:
        Board()
        {
            udp = new SimpleUDP("192.168.207.255", 1234);
        }

        virtual ~Board() { delete udp; }

        virtual void led(uint8_t r, uint8_t g, uint8_t b)
        {
            if(r > 100) r = 100;
            if(g > 100) g = 100;
            if(b > 100) b = 100;
            string command = "rgb " + to_string(r) + " " + to_string(g) + " " + to_string(b);
            udp->send(command);
        }
};



int main()
{
    Board board;
    for(;;)
    {
        board.led(rand()%101, rand()%101, rand()%101);
        usleep(300000);
    }


/*
    SimpleUDP udp("192.168.207.255", 1234);
    udp.send("rgb 100 0 0"); usleep(300000);
    udp.send("rgb 0 100 0"); usleep(300000);
    udp.send("rgb 0 0 100"); usleep(300000);
    udp.send("rgb 0 0 0");
    cout << "waiting for a message from network..." << endl;
    cout << udp.receive() << endl;
    cout << endl;

*/

/*
    UDP *udp = new UDP();
    try {
        udp->set_address("192.168.207.255", 1234);
        udp->open_socket();
        udp->bind_socket();
        udp->enum_own_ips();
    } catch(...)
    {
        delete udp;
        udp = NULL;
    }

    if(udp)
    {
        try {
            udp->send("rgb 100 0 0");
        } catch(...) { }
        usleep(500000);
        try {
            udp->send("rgb 0 100 0");
        } catch(...) { }
        usleep(500000);
        try {
            udp->send("rgb 0 0 100");
        } catch(...) { }
        usleep(500000);
        try {
            udp->send("rgb 0 0 0");
        } catch(...) { }

        cout << "waiting for a message from network..." << endl;
        try {
            if(udp->good())
            {
                string msg;
                sockaddr_in remote;
                do { //ignore own message, wait for another message...
                    msg = udp->receive(remote);
                } while (udp->is_my_addr(remote));
                cout << msg << endl;
            }
        } catch(...) { }
        udp->close_socket();
        delete udp;
    }
*/
    return 0;
}
