#include "HttpServer.hpp"

HttpServer::HttpServer()
{
    return;
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

const std::string &HttpServer::getRoot() const
{
    return this->_root;
}

void HttpServer::setPort(const int &port)
{
    this->_port = port;
}

void HttpServer::setSocket(const int &socket)
{
    this->_socket = socket;
}

void HttpServer::setRoot(const std::string &root)
{
    this->_root = root;
}

void HttpServer::setLocation(const std::vector<Location> &location)
{
    this->_location = location;
}