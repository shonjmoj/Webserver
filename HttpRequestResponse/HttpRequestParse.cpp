#include "HttpRequest.hpp"

void     HttpRequest::CheckRequestMessage()
{
    if (CheckVersion(getVersion()) || CheckMethod(getMethod()))
    {
        this->setStatuString("Bad Request");
        this->setstatus("400");
        this->setResponse(GeneratedHtmlError("400 Bad Request"));
        return;
    }
    if (getMethod() == "POST") 
    {
        if (this->headers["Content-Length"].empty())
        {
            this->setStatuString("Bad Request");
            this->setstatus("400");
            this->setResponse(GeneratedHtmlError("400 Bad Request"));
            return;
        }
    }
}

int     CheckMethod(std::string method)
{
    if (method != "GET" && method != "DELETE" && method != "POST")
        return(1);
    return(0);
}

int     CheckVersion(std::string version)
{
    if (version != "HTTP/1.1")
        return(1);
    return(0);
}