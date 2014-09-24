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


int main()
{
    SimpleUDP udp("192.168.207.255", 1234);
    udp.send("rgb 100 0 0"); usleep(300000);
    udp.send("rgb 0 100 0"); usleep(300000);
    udp.send("rgb 0 0 100"); usleep(300000);
    udp.send("rgb 0 0 0");
    cout << "waiting for a message from network..." << endl;
    cout << udp.receive() << endl;
    cout << endl;

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
