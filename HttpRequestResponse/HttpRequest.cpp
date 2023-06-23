  #include "HttpRequest.hpp"

HttpRequest::~HttpRequest()
{
}

void    HttpRequest::ParseRequest()
{
    if (requestMessage.empty() || requestMessage[0] == '\0')
        return;
    size_t pos = requestMessage.find(' ');
    this->method = requestMessage.substr(0, pos);
    size_t pos2 = requestMessage.find(' ', pos + 1);
    size_t queryPos = requestMessage.find('?', pos + 1);
    if (queryPos != std::string::npos && queryPos < pos2)
    {
        this->path = requestMessage.substr(pos + 1, queryPos - pos - 1);
        this->query = requestMessage.substr(queryPos + 1, pos2 - queryPos - 1);
    }
    else
        this->path = requestMessage.substr(pos + 1, pos2 - pos - 1);
    size_t pos3 = requestMessage.find("\r\n", pos2 + 1);
    this->version = requestMessage.substr(pos2 + 1, pos3 - pos2 - 1);
    size_t headerStart = pos3 + 2;
    size_t headerEnd = requestMessage.find("\r\n\r\n", headerStart);
    std::string headersStr = requestMessage.substr(headerStart, headerEnd - headerStart);
    std::stringstream headersStream(headersStr);
    std::string headerLine;
    while (std::getline(headersStream, headerLine, '\r'))
    {
        headerLine.erase(std::remove_if(headerLine.begin(), headerLine.end(), ::isspace), headerLine.end());
        if (headerLine.empty())
            continue;
        size_t colonPos = headerLine.find(':');
        std::string headerName = headerLine.substr(0, colonPos);
        std::string headerValue = headerLine.substr(colonPos + 1);

        headers[headerName] =  headerValue;
        if (headerName == "Content-Length" ||  headerName == "content-length")
            this->setContentLength(sts(headerValue));
        if(headerName == "Range")
        {
            setRange(headerValue);
            this->isRange = true;
        }
        if (headerName == "Content-Type")
        {
            size_t pos = headerValue.find(';');
            if (pos == headerValue.npos)
                continue;
            boundary = headerValue.substr(pos+10);
        }
        if(headerName == "Cookie")
            this->cookie = headerValue;
        if (headerName == "Host")
        {
            size_t portPos = headerValue.find(':');
            this->port = headerValue.substr(portPos + 1);
            if (portPos != std::string::npos)
            {
                this->port = headerValue.substr(portPos + 1);
                this->serverName = headerValue.substr(0, portPos);
            }
            else
            {
                this->setServerName(headerValue);
            }
        }
    }
    this->CheckRequestMessage();
    std::string s = requestMessage.substr(headerEnd + 4);
    if (this->method == "POST")
    {
        bodyReadingFinished = false;
        this->readBody(s);
    }
}

////////////////////////// SETTER //////////////////////////

void    HttpRequest::setPort(std::string _port)
{
    this->port = _port;
}

void    HttpRequest::setServerName(std::string _serverName)
{
    this->serverName = _serverName;
}

void HttpRequest::setRequest(std::string ResquestMessage)
{
    this->requestMessage = ResquestMessage;
}

void HttpRequest::setResponse(std::string ResponseMessage)
{
    this->responseMessage = ResponseMessage;
}

void HttpRequest::setMethod(std::string method)
{
    this->method = method;
}

void HttpRequest::setPath(std::string path)
{
    this->path = path;
}

void HttpRequest::setVersion(std::string version)
{
    this->version = version;
}

void HttpRequest::setBody(std::string body)
{
    this->body = body;
}

void HttpRequest::setstatus(std::string Status){
    this->status = Status;
}

void HttpRequest::setContentType(std::string ContentType){
    this->ContentType = ContentType;
}

void    HttpRequest::setLocation(std::string location)
{
    this->location = location;
}

void HttpRequest::setUri(std::string uri){
    this->Uri = uri;
}
void HttpRequest::setStatuString(std::string status){
    this->statusString = status;
}

void HttpRequest::setAutoIndex(bool autoIndex){
    this->autoIndex = autoIndex;
}

void HttpRequest::setindex(std::vector<std::string> _index){
    this->index = _index;
}

void HttpRequest::setResourse(std::string _resourse){
    this->resourse = _resourse;
}

void HttpRequest::setRange(std::string _range){
    this->range = _range;
}

void HttpRequest::setContentLength(size_t _contentLength){
    this->contentLength = _contentLength;
}

void HttpRequest::setResponseLocation(std::string location)
{
    responseLocation = location;
}

/////////////// GETTER //////////////////////

std::string  HttpRequest::getPort()
{
    return this->port;
}

std::string HttpRequest::getServerName()
{
    return this->serverName;
}

std::string HttpRequest::getPath()
{
    return this->path;
}


std::string HttpRequest::getRequest()
{
    return this->requestMessage;
}

std::string HttpRequest::getLocation()
{
    return this->location;
}

std::string HttpRequest::getUri(){
    return this->Uri;
}

std::string HttpRequest::getVersion() {
    return this->version;
}

std::string HttpRequest::getMethod() {
    return this->method;
}

std::string HttpRequest::getBody() {
    return this->body;
}

std::string HttpRequest::getStatuString() {
    return this->statusString;
}

std::string HttpRequest::getContentType() {
    if (Uri.find_last_of('.') == Uri.npos)
        return config->getType("");
    return config->getType(Uri.substr(Uri.find_last_of(".") + 1));
}

size_t HttpRequest::getContentLength() {
    return this->contentLength;
}

bool HttpRequest::getAutoIndex() {
    return this->autoIndex;
}

std::string HttpRequest::getResponse() {
    return this->responseMessage;
}

std::vector<std::string> HttpRequest::getindex() {
    return this->index;
}


size_t HttpRequest::getContentRange() {
    return this->contentRange;
}

std::string   HttpRequest::getStatus()
{
    return this->status;
}

std::string    HttpRequest::getResourse()
{
    return this->resourse;
}

std::string HttpRequest::getRange()
{
    return this->range;
}


HttpRequest::HttpRequest(std::string requestMessage, configFile *_config, std::string ip) : config(_config)
{
    this->ip = ip;
    this->rangeEnd = 0;
    this->rangeStart = 0;
    this->isRange = false;
    this->bodyReadingFinished = true;
    this->setRequest(requestMessage);
    this->ParseRequest();
    if (this->method != "POST")
        creatResponse();
}
void HttpRequest::creatResponse()
{
    if (this->getPath().find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != this->getUri().npos)
    {
        std::cout << "request.getPath().find_first_not_of:" << this->getPath() << std::endl;
        this->setstatus("400");
        this->setStatuString("Bad Request");
        setResponse(GeneratedHtmlError("400 Bad Request--"));
        return;
    }
    if (this->getPath().size() > 2048)
    {
        this->setstatus("414");
        this->setStatuString("Request-URI Too Long");
        setResponse(GeneratedHtmlError("Request-URI Too Long"));
        return;
    }
    this->path = url_encoding(this->getPath());
    servReq = config->getServer(make_pair(this->ip,this->getServerName()));
    this->setUri(servReq->getRoot());
    this->client_max_body_size = servReq->getCMBS();
    this->getFindLocation(servReq->getLocations());
    if(Uri.empty())
        return;
    if (this->getMethod() == "GET")
    {
        this->get(*this);
    }
    else if (this->getMethod() == "POST")
    {
        this->post(*this);
    }
    else if (this->getMethod() == "DELETE")
    {
        this->Delete(*this);
    }
}
