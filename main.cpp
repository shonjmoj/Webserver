#include "Server.hpp"

int main()
{
    std::vector<HttpServer> arr;
    HttpServer http1("http1", 8080);
    HttpServer http2("http2", 8081);
    arr.push_back(http1);
    arr.push_back(http2);
    Server servers(arr);
    servers._maxfd = 0;
    servers.start();
    servers.setPollFd();
    int i = 0;
    char buffer[1024];
    std::ifstream file("test.html");
    printf("maxfd = %d\n", servers._maxfd);
    std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 11\n\nHello World";
    while (1)
    {
        int status = poll(servers._fds, servers._maxfd + 1, -1);
        for (int i = 0; i < servers._maxfd + 1; i++)
        {
            bzero(buffer, sizeof(buffer));
            if (servers._fds[i].revents & POLLIN)
            {
                struct sockaddr_in client;
                socklen_t clientlen = sizeof(client);
                int client_fd = accept(servers._fds[i].fd, (struct sockaddr *)&client, &clientlen);
                recv(client_fd, buffer, sizeof(buffer), 0);
                send(client_fd, header.c_str(), header.length(), 0);
                close(client_fd);
            }
        }
    }
}