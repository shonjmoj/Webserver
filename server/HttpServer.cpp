#include "HttpServer.hpp"

HttpServer::HttpServer(std::string port)
{
    size_t  pos;

    pos = port.find(':');
    this->setHost(port.substr(0, pos));
    this->setPort(port.substr(pos+1));
}

HttpServer::~HttpServer()
{
    freeaddrinfo(this->_result);
    return;
}

std::string HttpServer::getPort() const
{
    return this->_port;
}

std::string HttpServer::getHost() const
{
    return this->_host;
}

const int &HttpServer::getSocket() const
{
    return this->_socket;
}

void HttpServer::setPort(std::string port)
{
    this->_port = port;
}

void HttpServer::setHost(std::string host)
{
    this->_host = host;
}

void HttpServer::setSocket(const int &socket)
{
    this->_socket = socket;
}
