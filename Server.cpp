#include "Server.hpp"

Server::Server()
{
    return;
}

Server::Server(const std::vector<HttpServer> & server)
{
    this->_servers = server;
}

Server::~Server()
{
    return;
}

void Server::setServer(const std::vector<HttpServer> &server)
{
    this->_servers = server;
}

void Server::initialisePorts()
{
    std::vector<HttpServer>::iterator it = this->_servers.begin();
    std::vector<HttpServer>::iterator ite = this->_servers.end();
    while (it != ite)
    {
        this->_ports.insert(it->getPort());
        printf("%s server has %d port\n", it->getName().c_str(), it->getPort());
        ++it;
    }
}

void Server::createsockets()
{
    std::vector<HttpServer>::iterator it = this->_servers.begin();
    std::vector<HttpServer>::iterator ite = this->_servers.end();
    while (it != ite)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
            printf("Socket %d creation failed", sock);
        else {
            it->setSocket(sock);
            printf("%s server socket %d created\n", it->getName().c_str() ,it->getSocket());
        }
        ++it;
    }
}

void Server::start() {
    std::vector<HttpServer>::iterator it = this->_servers.begin();
    std::vector<HttpServer>::iterator ite = this->_servers.end();

    while (it != ite) {
        if (listen(it->getSocket(), 0) < 0)
            printf("%s server listening failed !\n", it->getName().c_str());
        else
            printf("%s server listening on %d port\n", it->getName().c_str(), it->getPort());
        ++it;
    }
}