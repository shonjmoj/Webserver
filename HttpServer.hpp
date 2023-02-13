#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include "Location.hpp"

class HttpServer
{
    std::string _name;
    std::string _root;
    std::vector<Location> _location;
    struct sockaddr_in _addr;
    socklen_t _addrlen;
    int _port;
    int _socket;

public:
    HttpServer();
    HttpServer(std::string name, int port, std::string root, const std::vector<Location> & location);
    ~HttpServer();

    const int &getPort() const;
    const int &getSocket() const;
    const std::string &getRoot() const;
    const std::string &getName() const;
    struct sockaddr_in getSockAddr() const;
    socklen_t getSockAddrLen() const;

    void setPort(const int &port);
    void setSocket(const int &socket);
    void setName(const std::string & name);

    void setRoot(const std::string &root);
    void setLocation(const std::vector<Location> &location);
};

#endif