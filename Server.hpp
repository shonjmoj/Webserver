#ifndef SERVER_HPP
#define SERVER_HPP

#include "HttpServer.hpp"
#include <vector>
#include <set>
#include <stdio.h>

class Server
{
    std::vector<HttpServer> _servers;
    std::set<int> _sockets;
    std::set<int> _ports;

public:
    Server();
    Server(const std::vector<HttpServer> &server);
    ~Server();

    void setServer(const std::vector<HttpServer> &server);


    void createsockets();
    void start();
    void initialisePorts();
};

#endif