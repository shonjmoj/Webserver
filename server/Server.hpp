#ifndef SERVER_HPP
#define SERVER_HPP

#include "HttpServer.hpp"
#include "../HttpRequest/HttpRequest.hpp"
#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <vector>
#include <set>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sstream>
#include <map>


class Server
{

public:
    std::vector<HttpServer> _servers;
    std::vector<struct pollfd> _pollfds;
    std::set<int> _fd;
    std::map <int, HttpRequest *> _req;
    Server();
    Server(const std::vector<HttpServer> &server);
    ~Server();

    void setServer(const std::vector<HttpServer> &server);
    void setPollFd();
    void createSockets();
    void listenOn();
    void bindAddr();
    void start();
};

#endif