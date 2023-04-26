#ifndef SERVER_HPP
#define SERVER_HPP

#include "HttpServer.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
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

public:
    std::vector<HttpServer> _servers;
    struct pollfd *_fds;
    int _maxfd;
    typedef std::vector<HttpServer>::iterator iterator;
    typedef std::vector<HttpServer>::iterator r_iterator;

    Server();
    Server(const std::vector<HttpServer> &server);
    ~Server();

    void setServer(const std::vector<HttpServer> &server);
    void setPollFd();

    void createSockets();
    void initialisePorts();
    void listenOn();
    void bindAddr();
    void start();
};

#endif