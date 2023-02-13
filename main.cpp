#include "Server.hpp"
#include "HttpServer.hpp"
#include "Location.hpp"

int main () {
    std::vector<Location> location;
    HttpServer http1("http1", 8080, "/", location);
    HttpServer http2("http2", 9000, "/root", location);
    HttpServer http3("http3", 9000, "/root", location);
    std::vector<HttpServer> arr;
    arr.push_back(http1);
    arr.push_back(http2);
    arr.push_back(http3);
    Server servers(arr);
    servers.createsockets();
    servers.initialisePorts();
    servers.start();
}