#include "Server.hpp"
#include "HttpServer.hpp"

int main()
{
    std::vector<HttpServer> arr;
    HttpServer http1("http1", 8080);
    HttpServer http2("http2", 1337);
    arr.push_back(http1);
    arr.push_back(http2);
    Server servers(arr);
    servers._maxfd = 0;
    servers.start();
    servers.setPollFd();
}