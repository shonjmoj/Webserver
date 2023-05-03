#include "Server.hpp"

Server::Server()
{
    return;
}

Server::Server(const std::vector<HttpServer> &server)
{
    this->_servers = server;
}

Server::~Server()
{
    this->_servers.clear();
    return;
}

void Server::setServer(const std::vector<HttpServer> &server)
{
    this->_servers = server;
}

void Server::setPollFd()
{
    Server::iterator it = this->_servers.begin();
    Server::r_iterator ite = this->_servers.end();
    int i = 0;

    while (it != ite)
    {
        this->_serverClientFds[i].fd = it->getSocket();
        this->_serverClientFds[i].events = POLLIN;
        i++;
        it++;
        this->_maxfd = i;
    }
}

void Server::initialisePorts()
{
    Server::iterator it = this->_servers.begin();
    Server::r_iterator ite = this->_servers.end();
    while (it != ite)
    {
        printf("%s socket %d has %d port\n", it->getName().c_str(), it->getSocket(), it->getPort());
        it++;
    }
}

void Server::createSockets()
{
    Server::iterator it = this->_servers.begin();
    Server::r_iterator ite = this->_servers.end();
    while (it != ite)
    {
        memset(&it->_hints, 0, sizeof(it->_hints));
        it->_hints.ai_family = AF_INET;
        it->_hints.ai_socktype = SOCK_STREAM;
        it->_hints.ai_flags = AI_PASSIVE;
        getaddrinfo(NULL, std::to_string(it->getPort()).c_str(), &it->_hints, &it->_result);
        int sock = socket(it->_result->ai_family, it->_result->ai_socktype, it->_result->ai_protocol);
        if (sock < 0)
        {
            printf("Socket %d creation failed\n", sock);
        }
        else
        {
            it->setSocket(sock);
            fcntl(it->getSocket(), F_SETFL, O_NONBLOCK);
            printf("%s socket %d created\n", it->getName().c_str(), it->getSocket());
        }
        it++;
    }
}

void Server::listenOn()
{
    Server::iterator it = this->_servers.begin();
    Server::r_iterator ite = this->_servers.end();

    while (it != ite)
    {
        if (listen(it->getSocket(), 0) < 0)
            std::cout << "listen failed on " << it->getName().c_str() << std::endl;
        else
            std::cout << "listen successful on " << it->getName().c_str() << " with the port " << it->getPort() << std::endl;
        it++;
    }
}

void Server::bindAddr()
{
    Server::iterator it = this->_servers.begin();
    Server::r_iterator ite = this->_servers.end();

    while (it != ite)
    {
        if (bind(it->getSocket(), it->_result->ai_addr, it->_result->ai_addrlen) < 0)
            perror("bind ");
        else
            printf("%s socket %d bound successfuly\n", it->getName().c_str(), it->getSocket());
        it++;
    }
}

void Server::start()
{
    this->createSockets();
    this->initialisePorts();
    this->bindAddr();
    this->listenOn();
}
