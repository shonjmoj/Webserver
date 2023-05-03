#include "HttpServer.hpp"

HttpServer::HttpServer()
{
    return;
}

HttpServer::HttpServer(std::string name, int port)
{
    this->setName(name);
    this->setPort(port);
}

HttpServer::~HttpServer()
{
    freeaddrinfo(this->_result);
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
