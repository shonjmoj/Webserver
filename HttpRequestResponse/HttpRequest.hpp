#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <stdio.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <dirent.h>
#include <ctime>
#include <algorithm>
#include "../configfile/configFile.hpp"


class HttpRequest
{
private:
    std::map<std::string, std::string> headers;
    bool error_status;
    bool isRange;
    bool  autoIndex;
    bool isAllowed;
    bool isLocationMatched;
    size_t len;
    size_t contentLength;
    size_t rangeStart;
    size_t rangeEnd;
    size_t contentRange;
    long long   client_max_body_size;
    std::string ip;
    std::string port;
    std::string query;
    std::string cookie;
    std::string status;
    std::string requestMessage;
    std::string method;
    std::string path;
    std::string version;
    std::string Uri;
    std::string body;
    std::string bodyCentent;
    std::string location;
    std::string LocationRoute;
    std::string host;
    std::string statusString;
    std::string ContentType;
    std::string responseMessage;
    std::string fileName;
    std::string boundary;
    std::string range;
    std::string serverName;
    std::string resourse;
    std::string responseLocation;
    std::string cgiResp;
    std::vector<std::string> index;
    configFile  *config;
    servData *servReq;
    Location *loc;
public:
    bool bodyReadingFinished;
    void creatResponse();
    void ParseRequest();
    void parseBody(std::string& bodyCentent);
    void CheckRequestMessage();
    void findLocation(std::string path);
    void req(std::string s);
    void getFindLocation(std::vector<std::pair<std::string, Location *> > locations);
    void readRequest(HttpRequest& request);
    void creatFille(std::string uri);
    bool error();
    void readBody(std::string& str);
    void readRange(std::string range);
    void getErrorhtml(std::string err);
    void get(HttpRequest& request);
    void post(HttpRequest& request);
    void Delete(HttpRequest& request);
    void set_Port(std::string port);
    void set_Host(std::string host);
    void setRequest(std::string ResquestMessage);
    void setResponseLocation(std::string location);
    void setResponse(std::string responseMessage);
    void setstatus(std::string status);
    void setUri(std::string Uri);
    void setStatuString(std::string statusString);
    void setAutoIndex(bool autoIndex);
    void setindex(std::vector<std::string> index);
    void setLocation(std::string location);
    void setRoot(std::string Root);
    void setVersion(std::string version);
    void setMethod(std::string method);
    void setBody(std::string body);
    void setPath(std::string path);
    void setContentType(std::string ContentType);
    void setFileName(std::string fileName);
    void setBoundary(std::string boundary);
    void setContentLength(size_t contentLength);
    void setRange(std::string range);
    void setResourse(std::string resourse);
    void setServerName(std::string serverName);
    void setPort(std::string port);
    void setHost(std::string host);
    void isReqWellFormed(HttpRequest& request);
    void response(std::string path);
    void setenv(char **env);

    bool getAutoIndex();
    std::string findContentType(std::string Url);
    std::string generateHTMLPage(const std::string& directoryPath, const std::string& uri);
    std::string sendResponse();
    std::string getUri();
    std::string getMethod();
    std::string getRequest();
    std::string getVersion();
    std::string getBody();
    std::string getHeaderValue(std::string key);
    std::string getStatuString();
    std::string getStatus();
    std::string getRoot();
    std::string getLocation();
    std::string getContentType();
    std::string getResponse();
    std::vector<std::string> getindex();
    std::string getPath();
    std::string getFileName();
    std::string getBoundary();
    std::string getRange();
    std::string getResourse();
    size_t getContentRange();
    size_t getContentLength();
    std::string getCurentTime(void);
    std::string getServerName();
    std::string getPort();
    std::string getHost();
    std::string cgi(void);
    HttpRequest(std::string requestMessage, configFile *config, std::string ip);
    ~HttpRequest();
    template<typename T>
    std::string to_string(T value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
};
size_t sts(std::string str) ;
int CheckHeaders(std::string headers);
int CheckPath(std::string path);
int CheckVersion(std::string version);
int CheckMethod(std::string method);
int CheckBody(std::string body);
int CheckRequestMessage(std::string requestMessage);
bool isPathInPath(std::string path1, std::string path2);
void delete_directory(const std::string& path);
std::string GeneratedHtmlError(std::string code);
std::string  url_encoding(std::string str);
std::string getRealPath(std::string path);
#endif