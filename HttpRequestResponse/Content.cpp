#include "HttpRequest.hpp"
#include <algorithm>

std::string toLower(std::string s) {
  for (std::string::iterator i = s.begin(); i != s.end(); ++i) {
    *i = tolower(*i);
  }
  return s;
}

size_t sts(std::string str) 
{
    std::istringstream iss(str);
    size_t ch;
    iss >> ch;
    return static_cast<size_t>(ch);
}

std::string HttpRequest::sendResponse()
{
    std::string _response;

    if (!cgiResp.empty())
    {
        _response = this->getVersion()  + " 200 OK\n" + cgiResp;
        return (_response);
    }
    if (this->isRange)
    {
        _response = this->getVersion() + " " +"206 Partial Content"+ "\r\nContent-Type: " +this->ContentType+ \
                     "\r\nContent-Length: " + to_string(this->getResponse().size()) + "\r\nContent-Range: bytes " +\
                    to_string(this->rangeStart)+ "-" + to_string(this->rangeEnd)  + "/" +
                    to_string(resourse.length());
    }
    else
    {
        _response = this->getVersion() + " " + this->getStatus() + " " + this->getStatuString()+ "\r\nDate: "+ this->getCurentTime()  + "\r\nContent-Type: "+ \
                        this->ContentType + "\r\nContent-Length: " + to_string(this->getResponse().length());
    }
    if(!responseLocation.empty())
        _response +=  "\r\nLocation: " + responseLocation;
    std::cout << _response << std::endl;
    _response +=  "\r\n\r\n" + this->getResponse();
    return _response;
}

void    HttpRequest::readRange(std::string range)
{
    this->rangeStart = sts(range.substr(range.find('=') + 1, range.find("-")));
    if (range.find("-") + 1 == range.size())
        this->rangeEnd =  (this->resourse.length() -1);
    else if (range.find("-") + 1 != range.size())
        this->rangeEnd = sts(range.substr(range.find("-")+1 , range.size()));
    if (this->rangeEnd >  this->resourse.length())
        this->rangeEnd =  this->resourse.length() - 1;
    this->contentRange = this->rangeEnd - this->rangeStart + 1;
    this->range = range;
}
void    HttpRequest::creatFille(std::string uri)
{
    this->setUri(uri);
    std::ofstream file(uri);
    file.write(this->body.c_str(), this->body.size());
    this->setstatus("201");
    this->setStatuString("Created");
    file.close();
}

void    HttpRequest::readBody(std::string &b)
{
    this->bodyCentent += b;
    if (this->bodyCentent.size() >= this->contentLength)
    {
        this->bodyReadingFinished= true;
        if (!boundary.empty())
        {
            size_t pos = bodyCentent.find("filename=\"");
            if (pos == std::string::npos)
            {
                this->setstatus("400");
                this->setStatuString("Bad Request");
                setResponse(GeneratedHtmlError("400 Bad Request"));
                this->setContentType("text/html");
                return ;
            }
            pos += strlen("filename=\"");
            size_t end_pos = bodyCentent.find('"', pos);
            if (end_pos == std::string::npos)
            {
                this->setstatus("400");
                this->setStatuString("Bad Request");
                setResponse(GeneratedHtmlError("400 Bad Request"));
                this->setContentType("text/html");
                return ;
            }
            this->fileName = bodyCentent.substr(pos, end_pos - pos);
            if (fileName.empty())
                fileName = "default";
            pos = bodyCentent.find("\r\n\r\n");
            if (pos == std::string::npos)
            {
                this->setstatus("400");
                this->setStatuString("Bad Request");
                setResponse(GeneratedHtmlError("400 Bad Request"));
                this->setContentType("text/html");
                return ;
            }
            pos += strlen("\r\n\r\n");
            end_pos = bodyCentent.find(this->boundary, pos);
            if (end_pos == std::string::npos)
            {
                this->setstatus("400");
                this->setStatuString("Bad Request");
                setResponse(GeneratedHtmlError("400 Bad Request"));
                this->setContentType("text/html");
                return;
            }
            this->body = bodyCentent.substr(pos, end_pos - pos);
        }
        else
            this->body = bodyCentent;
        creatResponse();
    }
    else
        this->bodyReadingFinished= false;
}

 std::string   HttpRequest::findContentType(std::string Uri) {
    
    if(Uri.find_last_of(".") == Uri.npos)
        return "text/html";
    std::string tmp = Uri.substr(Uri.find_last_of(".")+1);
    return config->getType(tmp);
}