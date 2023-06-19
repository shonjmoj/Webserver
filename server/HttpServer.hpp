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
    std::string _host;
    std::string _port;
    int _socket;

public:
    HttpServer() {return;};
    HttpServer(std::string port);
    ~HttpServer();

    struct addrinfo _hints;
    struct addrinfo *_result;

    std::string getPort() const;
    std::string getHost() const;
    const int &getSocket() const;

    void setPort(std::string port);
    void setHost(std::string host);
    void setSocket(const int &socket);
};

#endif