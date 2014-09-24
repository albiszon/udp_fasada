#pragma once
#include "UDP.h"
#include <iostream>
using namespace std;

class SimpleUDP
{
  protected:
    UDP *udp;
  public:
    enum UDP_CODES { OK = 0, UNKNOWN = -1, SOCKET = -2, BIND = -3, EXCEPTION = -4 };
    SimpleUDP(string ip, uint16_t port);
    virtual ~SimpleUDP();
    virtual int send(string msg);
    virtual string receive();
};
