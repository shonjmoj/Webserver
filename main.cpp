#include "Server.hpp"

int main()
{
    std::vector<HttpServer> arr;
    HttpServer http1("http1", 8080);
    HttpServer http2("http2", 8081);
    HttpServer http3("http3", 8082);
    HttpServer http4("http4", 8083);
    HttpServer http5("http5", 8084);
    HttpServer http6("http6", 8085);
    arr.push_back(http1);
    arr.push_back(http2);
    arr.push_back(http3);
    arr.push_back(http4);
    arr.push_back(http5);
    arr.push_back(http6);
    Server servers(arr);
    servers._maxfd = 0;
    servers.start();
    int i = 0;
    char buffer[1024];
    std::ifstream file("test.html");
    servers.setPollFd();
    printf("maxfd = %d\n", servers._maxfd);
    std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
    std::stringstream ss;
    ss << file.rdbuf();
    header.append(std::to_string(ss.str().length()));
    header.append("\n\n");
    std::string response = header + ss.str();
    int read, sent = 0;
    while (1)
    {
        int status = poll(servers._serverClientFds, servers._maxfd + 1, 100);
        for (int i = 0; i < servers._maxfd + 1; i++)
        {
            int client_fd;
            if (status > 0)
            {
                struct sockaddr_in client;
                socklen_t clientlen = sizeof(client);
                if (servers._serverClientFds[i].revents & POLLIN)
                {
                    client_fd = accept(servers._serverClientFds[i].fd, (struct sockaddr *)&client, &clientlen);
                    servers._clientFds[i].fd = client_fd;
                    servers._clientFds[i].events = POLLIN;
                }
                if (servers._clientFds[i].revents & POLLIN)
                {
                    read = recv(servers._clientFds[i].fd, buffer, 10, 0);
                    if (read)
                    {
                        servers._clientFds[i].events = POLLOUT;
                    }
                }
                else
                {
                    sent = send(servers._clientFds[i].fd, response.c_str(), response.size(), 0);
                    close(client_fd);
                }
            }
        }
    }
}