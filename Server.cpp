#include "Server.hpp"

Server::Server()
{
    return;
}

Server::~Server()
{
    return;
}

void Server::setServer(const std::vector<HttpServer> &server)
{
    this->_server = server;
}

void Server::initialisePorts()
{
    std::vector<HttpServer>::iterator it = this->_server.begin();
    std::vector<HttpServer>::iterator ite = this->_server.end();
    while (it != ite)
    {
        this->_ports.insert(it->getPort());
        ++it;
    }
}

void Server::createsockets()
{
    std::vector<HttpServer>::iterator it = this->_server.begin();
    std::vector<HttpServer>::iterator ite = this->_server.end();
    while (it != ite)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
            printf("Socket %d creation failed", sock);
        else
            it->setSocket(sock);
        ++it;
    }
}