#ifndef SERVER_HPP
#define SERVER_HPP

#include "HttpServer.hpp"
#include <vector>
#include <set>
#include <stdio.h>

class Server
{
    std::vector<HttpServer> _server;
    std::vector<int> _sockets;
    std::set<int> _ports;

public:
    Server();
    ~Server();

    void createsockets();
    void start();

    void setServer(const std::vector<HttpServer> &server);
    void initialisePorts();
};

#endif