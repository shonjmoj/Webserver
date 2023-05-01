#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

class HttpServer
{
    std::string _name;
    std::string _root;
    int _port;
    int _socket;

public:
    HttpServer();
    HttpServer(std::string name, int port);
    ~HttpServer();

    struct addrinfo _hints;
    struct addrinfo *_result;

    const int &getPort() const;
    const int &getSocket() const;
    const std::string &getName() const;

    void setPort(const int &port);
    void setSocket(const int &socket);
    void setName(const std::string &name);
};

#endif