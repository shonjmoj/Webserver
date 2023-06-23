#include "./configfile/configFile.hpp"
#include "./server/Server.hpp"
#include "./HttpRequestResponse/HttpRequest.hpp"
#include <ctime>

int main(int argc, char **argv)
{
    configFile *config; 
    try
    {
        if (argc == 2)
            config = new configFile(argv[1]);
        else if (argc == 1)
            config = new configFile("./conf/default.conf");
        else
            throw "Invalid arguments";
    }
    catch(const char *err_msg)
    {
        std::cerr << err_msg << std::endl;
        return (1);
    }
    catch(const std::string err_msg)
    {
        std::cerr << err_msg << std::endl;
        return (1);
    }
    signal(SIGPIPE, SIG_IGN);
    std::vector<std::string>ports = config->getPorts();
    std::vector<HttpServer> arr(ports.size());
    for(size_t i = 0; i < ports.size(); i++)
        arr[i] = HttpServer(ports[i]);
    Server servers(arr);
    servers.start();
    char buffer[500000];
    servers.setPollFd();
    std::map <int, std::string> response;

    int read;
    int sent;
    int idx = 0;
    std::map <int, int> fds_clock;
    while (1)
    {
        int status = poll(&servers._pollfds[0], servers._pollfds.size(), -1);
        if (status < 0)
            continue;
        for (size_t i = 0; i < servers._pollfds.size(); i++)
        {
            if (servers._pollfds[i].revents & (POLLHUP | POLLERR | POLLNVAL) )
            {
                close(servers._pollfds[i].fd);
                delete servers._req[servers._pollfds[i].fd];
                servers._req[servers._pollfds[i].fd] = NULL;
                response[servers._pollfds[i].fd] = "";
                servers._pollfds.erase(servers._pollfds.begin() + i);
                continue;
            }
            if (servers._pollfds[i].revents & POLLIN)
            {
                if (i < servers._servers.size())
                {
                    int newfd = accept(servers._servers[i].getSocket(), servers._servers[i]._result->ai_addr, &servers._servers[i]._result->ai_addrlen);
                    if (newfd < 0)
                        continue;
                    idx = i;
                    fcntl(newfd, F_SETFL, O_NONBLOCK);
                    servers._fd.insert(newfd);
                    struct pollfd c_fd;
                    c_fd.fd = newfd;
                    c_fd.events = POLLIN | POLLOUT ;
                    c_fd.revents = 0;
                    servers._pollfds.push_back(c_fd);
                }
                else
                {
                    bzero(buffer, sizeof(buffer));
                    read = recv(servers._pollfds[i].fd, buffer, 20000, 0);
                    if (read > 0)
                    {
                        std::string tmp(buffer, buffer + read);
                        fds_clock[servers._pollfds[i].fd] = time(nullptr);
                        if (servers._req[servers._pollfds[i].fd] && (servers._req[servers._pollfds[i].fd]->getMethod() != "POST" || !servers._req[servers._pollfds[i].fd]->bodyReadingFinished))
                            servers._req[servers._pollfds[i].fd]->readBody(tmp);
                        else if (!servers._req[servers._pollfds[i].fd])
                            servers._req[servers._pollfds[i].fd] = new HttpRequest(tmp, config, servers._servers[idx].getHost()+":"+servers._servers[idx].getPort());
                    }
                    else if (read <= 0)
                    {
                        close (servers._pollfds[i].fd);
                        delete servers._req[servers._pollfds[i].fd];
                        servers._req[servers._pollfds[i].fd] = NULL;
                        response[servers._pollfds[i].fd] = "";
                        servers._pollfds.erase(servers._pollfds.begin() + i);
                        continue;
                    }
                }
            }
            else if (servers._req[servers._pollfds[i].fd] && (servers._req[servers._pollfds[i].fd]->getMethod() != "POST" || servers._req[servers._pollfds[i].fd]->bodyReadingFinished) && servers._pollfds[i].revents & POLLOUT)
            {
                if (response[servers._pollfds[i].fd].empty())
                    response[servers._pollfds[i].fd] = servers._req[servers._pollfds[i].fd]->sendResponse();
                sent = send(servers._pollfds[i].fd, response[servers._pollfds[i].fd].c_str(), response[servers._pollfds[i].fd].length(), 0);
                if (sent < 0)
                    continue;
                response[servers._pollfds[i].fd] = response[servers._pollfds[i].fd].substr(sent);
                if (response[servers._pollfds[i].fd].empty())
                {
                    close(servers._pollfds[i].fd);
                    delete servers._req[servers._pollfds[i].fd];
                    servers._req[servers._pollfds[i].fd] = NULL;
                    response[servers._pollfds[i].fd] = "";
                    servers._pollfds.erase(servers._pollfds.begin() + i);
                }
            }
            else if(servers._req[servers._pollfds[i].fd] && fds_clock[servers._pollfds[i].fd])
            {
                if(time(nullptr) - fds_clock[servers._pollfds[i].fd] > 8 )
                {
                    fds_clock[servers._pollfds[i].fd] = 0;
                    servers._req[servers._pollfds[i].fd]->setstatus("408");
                    servers._req[servers._pollfds[i].fd]->setStatuString("Request Timeout");
                    servers._req[servers._pollfds[i].fd]->setResponse(GeneratedHtmlError("408 Request Timeout"));
                    servers._req[servers._pollfds[i].fd]->bodyReadingFinished = true;
                }
            }
        }
    }
}
