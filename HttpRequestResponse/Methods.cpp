#include "HttpRequest.hpp"
#include <sstream>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>
#include <signal.h>

int is_directory(const char* path)
{
    struct stat buf;
    if (stat(path, &buf) != 0) {
        return -1;
    }
    if (S_ISDIR(buf.st_mode)) {
        return 1;
    } else {
        return 2;
    }
}

void HttpRequest::getErrorhtml(std::string err)
{
    std::stringstream ss(err);
    int errint; ss >> errint;
    std::string test = servReq->getErrorPage(errint);
    this->setContentType("text/html");
    if(test.empty())
        return setResponse(GeneratedHtmlError(err));
    test = servReq->getRoot()+ "/" + test;
    if (is_directory(test.c_str()) != 2)
        return setResponse(GeneratedHtmlError(err));
    response(test);
}

void    HttpRequest::get(HttpRequest &request)
{
    if (!request.isLocationMatched)
    {
        request.setstatus("404");
        request.setStatuString("Not Found");
        getErrorhtml("404 Not Found");
        return;
    }
    if (this->isAllowed == false)
    {
        request.setstatus("405");
        request.setStatuString("Method Not Allowed");
        getErrorhtml("405 Method Not Allowed");
        return;
    }
    if (access(request.getUri().c_str(), F_OK) != 0)
    {
        request.setstatus("404");
        request.setStatuString("Not Found");
        getErrorhtml("404 Not Found");
        return;
    }
    else if (is_directory(request.getUri().c_str() ) == 1)
    {
        DIR* dir_ptr = opendir(request.getUri().c_str());
        if (dir_ptr == NULL)
        {
            request.setstatus("403");
            request.setStatuString("Forbidden");
            request.setResponse(GeneratedHtmlError("403 Forbidden"));
            return;
        }
        closedir(dir_ptr);
        if ((request.getLocation()).find_last_of("/") != request.getLocation().size() - 1)
        {
            request.setstatus("301");
            request.setStatuString("Moved Permanently");
            request.setResponseLocation(request.getLocation() + "/");
            getErrorhtml("301 Moved Permanently");
            return;
        }
        std::vector<std::string> indexSet = request.getindex();
        for (std::vector<std::string>::iterator it = indexSet.begin(); it != indexSet.end(); ++it)
        {
            std::string tmp = request.getUri() + *it;
            if (is_directory(tmp.c_str()) == 2)
            {
                if (loc->isCgi() && loc->checkFormat(*it))
                {
                    Uri = tmp;
                    cgiResp = cgi();
                    return ;
                }
                request.setUri(tmp);
                this->bodyReadingFinished = true;
                request.setstatus("200");
                request.setStatuString("OK");
                request.setContentType(request.findContentType(request.getUri()));
                request.response(request.getUri());
                return;
            }
        }
        if (request.getAutoIndex() == false)
        {
            request.setstatus("403");
            request.setStatuString("Forbidden");
            getErrorhtml("403 Forbidden");
            return;
        }
        request.setstatus("200");
        request.setStatuString("OK");
        this->setResponse(request.generateHTMLPage(request.getUri(), request.getPath()));
        return;
    }
    if (loc->isCgi() && loc->checkFormat(Uri))
    {
        cgiResp = cgi();
        return ;
    }
    request.setstatus("200");
    request.setStatuString("OK");
    request.setContentType(request.findContentType(request.getUri()));
    request.response(request.getUri());
    std::cout << "-----------------Uri: " << request.getUri() << std::endl;
}

///////////////////////////////////////// POST /////////////////////////////////////////

void    HttpRequest::post(HttpRequest &request)
{
    if (request.getPath().find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != request.getUri().npos)
    {
        request.setstatus("400");
        request.setStatuString("Bad Request");
        getErrorhtml("400 Bad Request-");
        return;
    }
    if (request.getUri().size() > 2048)
    {
        request.setstatus("414");
        request.setStatuString("Request-URI Too Long");
        getErrorhtml("414 Request-URI Too Long");
        return;
    }
    if (request.client_max_body_size >= 0 && static_cast<long long>(request.body.size()) > request.client_max_body_size)
    {
        request.setstatus("413");
        request.setStatuString("Request Entity Too Large");
        getErrorhtml("413 Request Entity Too Large");
        return;
    }
    if (!request.isLocationMatched)
    {
        request.setstatus("404");
        request.setStatuString("Not Found");
        getErrorhtml("404 Not Found");
        return;
    }
    if (this->isAllowed == false)
    {
        request.setstatus("405");
        request.setStatuString("Method Not Allowed");
        getErrorhtml("405 Method Not Allowed");
        return;
    }
    if (access(request.getUri().c_str(), F_OK) != 0)
    {
        request.setstatus("404");
        request.setStatuString("Not Found");
        getErrorhtml("404 Not Found");
        return;
    }
    else if (is_directory(request.getUri().c_str()) == 1)
    {
        DIR* dir_ptr = opendir(request.getUri().c_str());
        if (dir_ptr == NULL)
        {
            request.setstatus("403");
            request.setStatuString("Forbidden");
            request.setResponse(GeneratedHtmlError("403 Forbidden"));
            return;
        }
        closedir(dir_ptr);
        if ((request.getLocation()).find_last_of("/") != request.getLocation().size() - 1)
        {
            request.setstatus("301");
            request.setStatuString("Moved Permanently");
            request.setResponseLocation(request.getLocation() + "/");
            getErrorhtml("301 Moved Permanently");
            return;
        }
        if (request.bodyReadingFinished)
        {
            std::vector<std::string> indexSet = request.getindex();
            for (std::vector<std::string>::iterator it = indexSet.begin(); it != indexSet.end(); ++it)
            {
                std::string tmp = request.getUri() + *it;
                if (is_directory(tmp.c_str() ) == 2)
                {
                    if (loc->isCgi() && loc->checkFormat(*it))
                    {
                        cgiResp = cgi();
                        return ;
                    }
                }
            }
        if (this->fileName.empty())
        {
            this->setstatus("200");
            this->setStatuString("OK");
            setResponse(GeneratedHtmlError("200 Ok"));
            this->setContentType("text/html");
            return;
        }
            request.setstatus("201");
            request.setStatuString("Created");
            getErrorhtml("201 Created");
            request.creatFille(this->getUri() +  "/" + this->fileName);
        }
    }
    if (loc->isCgi() && loc->checkFormat(Uri))
    {
        cgiResp = cgi();
        return ;
    }
}

///////////////////////////////////////// DELETE /////////////////////////////////////////

void    HttpRequest::Delete(HttpRequest &request)
{
    if (request.getUri().find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=") != request.getUri().npos)
    {
        request.setstatus("400");
        request.setStatuString("Bad Request");
        getErrorhtml("400 Bad Request");
        return;
    }
    if (request.getUri().size() > 2048)
    {
        request.setstatus("414");
        request.setStatuString("Request-URI Too Long");
        getErrorhtml("414 Request-URI Too Long");
        return;
    }
    if (!request.isLocationMatched)
    {
        request.setstatus("404");
        request.setStatuString("Not Found");
        getErrorhtml("404 Not Found");
        return;
    }
    if (this->isAllowed == false)
    {
        request.setstatus("405");
        request.setStatuString("Method Not Allowed");
        getErrorhtml("405 Method Not Allowed");
        return;
    }
    if (access(request.getUri().c_str(), F_OK) != 0)
    {
        request.setstatus("404");
        request.setStatuString("Page Not Found");
        getErrorhtml("404 Not Found");
        return;
    }
    if (is_directory(request.getUri().c_str()) == 1)
    {
         if (request.getUri().find_last_of("/") != request.getUri().size() - 1)
        {
            request.setstatus("409");
            request.setStatuString("Conflict");
            getErrorhtml("409 Conflict");
            return;
        }
        DIR* dir_ptr = opendir(request.getUri().c_str());
        if (dir_ptr == NULL)
        {
            request.setstatus("403");
            request.setStatuString("Forbidden");
            request.setResponse(GeneratedHtmlError("403 Forbidden"));
            return;
        }
        closedir(dir_ptr);
        delete_directory(request.getUri().c_str());
        if (rmdir(request.getUri().c_str()) != 0)
        {
            request.setstatus("500");
            request.setStatuString("Internal Server Error");
            getErrorhtml("500 Internal Server Error");
            return;
        }
        else
        {
            request.setstatus("204");
            request.setStatuString("No Content");
            getErrorhtml("204 No Content");
            return;
        }
    
    }
    else
    {
        if (unlink(request.getUri().c_str()) != 0)
        {
            request.setstatus("500");
            request.setStatuString("Internal Server Error");
            request.setResponse(GeneratedHtmlError("500 Internal Server Error"));
            return;
        }
        else
        {
            request.setstatus("204");
            request.setStatuString("No Content");
            request.setResponse(GeneratedHtmlError("204 No Content"));
            return;
        }
    }
        request.setstatus("403");
        request.setStatuString("Forbidden");
        request.setResponse(GeneratedHtmlError("403 Forbidden"));
        return;
}

void    HttpRequest::setenv(char **env)
{
    std::string Path;
    std::string scpt;
    size_t      pos;


    pos = Uri.find_last_of('/');
    Path = Uri.substr(0, pos);
    scpt = Uri.substr(pos+1);
    
    env[0] = strdup(("REQUEST_METHOD="+method).c_str());
    env[1] = strdup(("SCRIPT_NAME="+location).c_str());
    env[2] = strdup(("QUERY_STRING="+query).c_str());
    env[3] = strdup(("HTTP_HOST="+serverName).c_str());
    env[4] = strdup(("REQUEST_URI="+path+query).c_str());
    env[5] = strdup(("REMOTE_PORT="+port).c_str());
    env[6] = strdup(("SERVER_PORT="+port).c_str());
    env[7] = strdup(("CONTENT_TYPE="+ContentType).c_str());
    env[8] = strdup(("CONTENT_LENGTH="+to_string(contentLength)).c_str());
    env[9] = strdup(("SCRIPT_FILENAME="+Uri).c_str());
    env[10] = strdup(("SCRIPT_NAME="+scpt).c_str());
    env[11] = strdup(("PATH="+Uri).c_str());
    env[12] = strdup(("HTTP_COOKIE="+cookie).c_str());
    env[13] = NULL;

}

std::string    HttpRequest::cgi()
{
    std::stringstream	res;
	std::string			ret;
	pid_t				pid;
	pid_t				result;
    time_t              start, current;
	int					fds[2];
	int 				rd;
    int                 status;
	char				buff[256];

	if (pipe(fds) == -1)
		ret = "Internal Server Error";
	pid = fork();
	if (pid == -1)
		ret = "Internal Server Error";
	else if (pid == 0)
	{
        int     fd;
		char	*args[3] = {strdup(loc->getCgi(Uri).c_str()), strdup(Uri.c_str()), NULL};
		char	*env[14];
        std::ofstream file("/tmp/http_body.txt");

        setenv(env);
        if (!file.is_open() || file.fail())
            exit(1);
        file << bodyCentent;
        file.close();
        fd = open("/tmp/http_body.txt", O_RDWR);
        if (dup2(fd, 0) == -1)
            exit(1);
		if (dup2(fds[1], 1) == -1)
            exit(1);
        if (close(fd) == -1)
            exit(1);
		if (close(fds[1]) == -1)
            exit(1);
		if (close(fds[0]) == -1)
            exit(1);
		if (execve(args[0], args, env) == -1)
            exit(1);
	}
	else
	{
		if (close(fds[1]) == -1)
		{
            this->setstatus("500");
            this->setStatuString("Internal Server Error");
            this->setResponse(GeneratedHtmlError("500 Internal Server Error"));
            return ("");
        }
        start = time(NULL);
        current = time(NULL);
        while (current - start < 8)
        {
            result = waitpid(pid, &status, WNOHANG);
            if (result == 0)
                current = time(NULL);
            else
                break;
        }
        if (waitpid(pid, &status, WNOHANG) == 0)
        {
            kill(pid, SIGTERM);
            this->setstatus("500");
            this->setStatuString("Internal Server Error");
            this->setResponse(GeneratedHtmlError("500 Internal Server Error"));
            return ("");
        }
        else if (status != 0)
        {
            this->setstatus("500");
            this->setStatuString("Internal Server Error");
            this->setResponse(GeneratedHtmlError("500 Internal Server Error"));
            return ("");
        }
		while ((rd = read(fds[0], buff, sizeof(buff))) != 0)
		{
			if (rd == -1)
			{
                this->setstatus("500");
                this->setStatuString("Internal Server Error");
                this->setResponse(GeneratedHtmlError("500 Internal Server Error"));
                return ("");
            }
			res.write(buff, rd);
		}
		if (close(fds[0]) == -1)
		{
            this->setstatus("500");
            this->setStatuString("Internal Server Error");
            this->setResponse(GeneratedHtmlError("500 Internal Server Error"));
            return ("");
        }
		ret = res.str();
	}
    return (ret);
}