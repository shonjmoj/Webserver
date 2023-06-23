#include "HttpStatusPars.hpp"

HttpStatusPars::HttpStatusPars(std::string dir)
{
    std::ifstream file;
    file.open(dir);
    if(!file.is_open())
    {
        std::cout << dir << std::endl;
        exit(1) ;
    }
    std::string str;
    while(std::getline(file, str, '\n'))
    {
        if(str.find(",") == str.npos)
            exit(1) ;
        _map[str.substr(0, str.find(','))] = str.substr(str.find(',') + 1);
    }
    file.close();
}

std::string HttpStatusPars::getStatus(std::string key)
{
    if(_map.find(key) == _map.end())
        return "Unassigned";
    return _map[key];

}

HttpStatusPars::~HttpStatusPars()
{
}