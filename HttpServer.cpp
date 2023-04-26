#include "HttpServer.hpp"

HttpServer::HttpServer()
{
    return;
}

HttpServer::HttpServer(std::string name, int port)
{
    this->_addrlen = sizeof(this->_addr);
    this->setName(name);
    this->setPort(port);
}

HttpServer::~HttpServer()
{
    return;
}

const int &HttpServer::getPort() const
{
    return this->_port;
}

const int &HttpServer::getSocket() const
{
    return this->_socket;
}

const std::string &HttpServer::getName() const
{
    return this->_name;
}

struct sockaddr_in HttpServer::getSockAddr() const
{
    return this->_addr;
}

socklen_t HttpServer::getSockAddrLen() const
{
    return this->_addrlen;
}

void HttpServer::setName(const std::string &name)
{
    this->_name = name;
}

void HttpServer::setPort(const int &port)
{
    this->_port = port;
}

void HttpServer::setSocket(const int &socket)
{
    this->_socket = socket;
}
