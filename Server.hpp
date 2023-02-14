#ifndef SERVER_HPP
#define SERVER_HPP

#include "HttpServer.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
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
    std::set<int> _fds;
    std::set<int> _ports;


public:
    typedef std::vector<HttpServer>::iterator iterator;
    typedef std::vector<HttpServer>::iterator r_iterator;


    Server();
    Server(const std::vector<HttpServer> &server);
    ~Server();

    void getFds(); 
    void getPorts();

    void setServer(const std::vector<HttpServer> &server);

    void createSockets();
    void initialisePorts();
    void listenOn();
    void bindAddr();
    void start();
};

#endif