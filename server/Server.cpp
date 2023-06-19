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
    for (size_t i = 0; i < this->_servers.size(); i++)
    {
        this->_pollfds[i].fd = this->_servers[i].getSocket();
        this->_pollfds[i].events = POLLIN;
        this->_pollfds[i].revents = 0;
    }
}

void Server::createSockets()
{
    int yes = 1;
    for (size_t i = 0; i < this->_servers.size(); i++)
    {
        memset(&this->_servers[i]._hints, 0, sizeof(this->_servers[i]._hints));
        this->_servers[i]._hints.ai_family = AF_INET;
        this->_servers[i]._hints.ai_socktype = SOCK_STREAM;
        this->_servers[i]._hints.ai_flags = AI_PASSIVE;
        this->_pollfds.push_back(pollfd());
        getaddrinfo(this->_servers[i].getHost().c_str(), this->_servers[i].getPort().c_str(), &this->_servers[i]._hints, &this->_servers[i]._result);
        this->_servers[i].setSocket(socket(AF_INET, SOCK_STREAM, 0));
        setsockopt(this->_servers[i].getSocket(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (this->_servers[i].getSocket() < 0)
            exit(1);
        else
        {
            this->_fd.insert(this->_servers[i].getSocket());
            if (fcntl(this->_servers[i].getSocket(), F_SETFL, O_NONBLOCK) == -1)
                exit(1);
            printf("%s socket %d created\n", this->_servers[i].getPort().c_str(), this->_servers[i].getSocket());
        }
    }
}

void Server::listenOn()
{

    for (size_t i = 0; i < this->_servers.size(); i++)
    {
        if (listen(this->_servers[i].getSocket(), 0) < 0)
            exit(1);
        else
            std::cout << "listen successful on " << this->_servers[i].getPort() << " with the port " << this->_servers[i].getPort() << std::endl;
    }
}

void Server::bindAddr()
{
    for (size_t i = 0; i < this->_servers.size(); i++)
    {
        if (bind(this->_servers[i].getSocket(), this->_servers[i]._result->ai_addr, this->_servers[i]._result->ai_addrlen) < 0) {
            perror("bind ");
            exit(1);
        }
        else
            printf("%s socket %d bound successfuly\n", this->_servers[i].getPort().c_str(), this->_servers[i].getSocket());
    }
}

void Server::start()
{
    this->createSockets();
    this->bindAddr();
    this->listenOn();
}
