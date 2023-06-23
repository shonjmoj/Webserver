
#include "HttpRequest.hpp"

void    HttpRequest::response(std::string path)
{
    std::ifstream input(path.c_str());
    std::stringstream ss;
	ss << input.rdbuf();
    if (input.fail())
    {
        this->setstatus("403");
        this->setStatuString("Forbidden");
        this->setResponse(GeneratedHtmlError("403 Forbidden"));
        setContentType("text/html");
        return;
    }
    input.close();
    this->setResourse(ss.str());
    if (this->isRange)
    {
        this->readRange(this->range);
        std::string s = ss.str();
        std::string s1;
        s1 = resourse.substr(this->rangeStart, this->rangeEnd - this->rangeStart + 1);
        this->setResponse(s1);
    }
    else
        this->setResponse(this->getResourse());
}
