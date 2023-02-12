#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <string>
#include <sys/types.h>
#include <vector>
#include "Location.hpp"

class HttpServer
{
    int _port;
    int _socket;
    std::string _root;
    std::vector<Location> _location;

public:
    HttpServer();
    ~HttpServer();

    const int &getPort() const;
    const int &getSocket() const;
    const std::string &getRoot() const;

    void setPort(const int &port);
    void setSocket(const int &socket);

    void setRoot(const std::string &root);
    void setLocation(const std::vector<Location> &location);
};

#endif