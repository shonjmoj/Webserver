#include "HttpServer.hpp"

HttpServer::HttpServer()
{
    this->setName("http1");
    return;
}

HttpServer::HttpServer(std::string name, int port, std::string root, Location const & location) {
    this->setName(name);
    this->setPort(port);
    this->setRoot(root);
    this->_location.push_back(location);
}

HttpServer::~HttpServer()
{
    this->_location.clear();
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

const std::string &HttpServer::getName() const {
    return this->_name;
}

void HttpServer::setName(const std::string & name) {
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

void HttpServer::setRoot(const std::string &root)
{
    this->_root = root;
}

void HttpServer::setLocation(const std::vector<Location> &location)
{
    this->_location = location;
}