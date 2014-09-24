#include "SimpleUDP.h"

SimpleUDP::SimpleUDP(std::string ip, uint16_t port)
{
    udp = new UDP();
    try {
        udp->set_address(ip, port);
        udp->open_socket();
        udp->bind_socket();
        udp->enum_own_ips();
    } catch(...)
    {
        delete udp;
        udp = NULL;
    }
}

SimpleUDP::~SimpleUDP()
{
    if(udp)
    {
        udp->close_socket();
        delete udp;
    }
}

int SimpleUDP::send(std::string msg)
{
    if(!udp) return SimpleUDP::UNKNOWN;
    try {
        if(udp->good())
            return udp->send(msg);
        else
            return SimpleUDP::UNKNOWN;
    } catch(...)
    {
        return SimpleUDP::EXCEPTION;
    }
}

std::string SimpleUDP::receive()
{
    if(!udp) return "";
    try {
        if(udp->good())
        {
            std::string msg;
            sockaddr_in remote;
            do { //ignore own message, wait for another message...
                msg = udp->receive(remote);
            } while (udp->is_my_addr(remote));
            return msg;
        }
        else
            return "";
    } catch(...)
    {
        return "";
    }
}

