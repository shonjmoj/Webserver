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
    delete[] this->_fds;
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

    this->_fds = new struct pollfd[this->_servers.size()];
    while (it != ite)
    {
        this->_fds[i].fd = it->getSocket();
        this->_fds[i].events = POLLIN;
        std::cout << "pollfd " << i << " has fd " << this->_fds[i].fd << std::endl;
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
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            printf("Socket %d creation failed\n", sock);
        }
        else
        {
            it->setSocket(sock);
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
        struct sockaddr_in tmp = it->getSockAddr();
        tmp.sin_family = AF_INET;
        tmp.sin_port = htons(it->getPort());
        tmp.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (bind(it->getSocket(), (const struct sockaddr *)&tmp, it->getSockAddrLen()) < 0)
        {
            perror("bind ");
        }
        else
        {
            printf("%s socket %d bound successfuly\n", it->getName().c_str(), it->getSocket());
        }
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
