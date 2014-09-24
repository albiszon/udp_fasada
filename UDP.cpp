#include "UDP.h"

#include <iostream>
#include <cstring>
#include <vector>

#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <netinet/in.h>

using namespace std;

bool UDP::good() { return _good; }

void UDP::show_own_ips()
{
    for(uint32_t i = 0; i<own_ips.size(); ++i)
        cout << "IP: " << own_ips.at(i) << endl;
}

void UDP::enum_own_ips()
{
    struct ifaddrs * ifAddrStruct = NULL;
    struct ifaddrs * ifa = NULL;
    void * tmpAddrPtr = NULL;
    getifaddrs(&ifAddrStruct);
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
        {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            //printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
            own_ips.push_back(addressBuffer);
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
}

UDP::UDP()
{
    _good = false;
    listener = NULL;
    net_socket = 0;
    receiver_thread_id = 0;
}

void UDP::set_address(string remote_ip, unsigned short port)
{
    this->remote_ip = remote_ip;
    this->port = port;
}

int UDP::open_socket()
{
    if(-1 == (net_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)))
        return -1;
    int b = 1;
    if (0 > setsockopt(net_socket, SOL_SOCKET, SO_BROADCAST, (void *) &b, sizeof(b)))
        return -2;
    memset((char*)&remote, 0, sizeof(remote));
    remote.sin_addr.s_addr = inet_addr(remote_ip.c_str());
    remote.sin_port = htons(port); //LittleEndian --> BigEndian

    return 0;
}

int UDP::bind_socket()
{
    sockaddr_in local;
    memset((char*)&local, 0, sizeof(local));
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(1234);
    if( -1 == bind(net_socket, (sockaddr*)&local, sizeof(local))) {
        cerr << "problem: bind" << endl;
        return -3;
    }
    _good = true;
    return 0;
}


int UDP::send(string s)
{
    if (-1 == sendto(net_socket, s.c_str(), s.size(), 0, (sockaddr*)&remote, sizeof(remote)))
        return -1;
    return 0;
}

void UDP::close_socket()
{
    if(net_socket) shutdown(net_socket, SHUT_RDWR);
    if(receiver_thread_id) pthread_join(receiver_thread_id, NULL);
}

UDP::~UDP()
{
    close_socket();
}


string UDP::receive(sockaddr_in &remote)
{
    int rxdatalen;
    socklen_t sockaddrlen = sizeof(remote);
    char buf[BUFLEN];
    if( -1 == (rxdatalen = recvfrom(net_socket, buf, BUFLEN, 0, (sockaddr*)&remote, &sockaddrlen)))
    {
        throw -1;
    }
    buf[rxdatalen] = '\0';
    return buf;
}


bool UDP::is_my_addr(sockaddr_in ip)
{
    string str_ip = inet_ntoa(ip.sin_addr);
    for(uint32_t i = 0; i<own_ips.size(); ++i)
        if(str_ip == own_ips.at(i)) return true;
    return false;
}

void* UDP::receiver(void* arg)
{
    UDP *me = static_cast<UDP*>(arg);
    char buf[BUFLEN];
    while(1)
    {
        int rxdatalen;
        sockaddr_in remote;
        socklen_t sockaddrlen = sizeof(remote);
        if( -1 == (rxdatalen = recvfrom(me->net_socket, buf, BUFLEN, 0, (sockaddr*)&remote, &sockaddrlen)))
        {
            pthread_exit(NULL);
            return NULL;
        }
        buf[rxdatalen] = '\0';
        string sender = inet_ntoa(remote.sin_addr);
            sender += ":";
            sender += to_string(ntohs(remote.sin_port));
        string message = buf;
        if(me->listener) me->listener(sender, message);
    }
}

void UDP::start_receiver( void (*listener)(string sender, string message) )
{
    this->listener = listener;
    pthread_create(&receiver_thread_id, NULL, receiver, (void*)this);
}



/*void my_listener(string sender, string message)
{
    cout << "From: " << sender << " \"" << message << "\"" << endl;
}*/
