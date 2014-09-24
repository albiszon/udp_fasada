#pragma once
#include <iostream>
#include <vector>
#include <arpa/inet.h>


class UDP {
    int net_socket;
    sockaddr_in remote;
    std::string remote_ip;
    std::vector< std::string > own_ips;
    unsigned short port;
    bool _good;
    pthread_t receiver_thread_id;
    static const int BUFLEN = (1536+1);
    void (*listener)(std::string sender, std::string message);

  public:
    UDP();
    virtual ~UDP();
    virtual bool good();
    virtual void show_own_ips();
    virtual void enum_own_ips();
    virtual void set_address(std::string remote_ip, unsigned short port);
    virtual int open_socket();
    virtual int bind_socket();
    virtual int send(std::string s);
    virtual void close_socket();
    virtual std::string receive(sockaddr_in &remote);
    virtual bool is_my_addr(sockaddr_in ip);
    virtual void start_receiver( void (*listener)(std::string sender, std::string message) );
    static void* receiver(void* arg);
};
