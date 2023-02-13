#ifndef SERVER_HPP
#define SERVER_HPP

#include "HttpServer.hpp"
#include <netinet/in.h>
#include <vector>
#include <set>
#include <stdio.h>

#define OK 200
#define MOVED_PERM 301
#define BAD_REQUEST 400
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define PAYLOAD_LARGE 413
#define SERVER_ERROR 500
#define NOT_IMPLEMENTED 501
#define BAD_GATEWAY 502
#define NOT_SUPPORTED 505

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
    void initialisePorts();
    void listenOn();
    void bindAddr();
};

#endif