#include <sstream>
#include <iostream>
#include <fstream>
#include <map>

class HttpStatusPars
{
    private:
        std::map<std::string, std::string> _map;
    public:
        HttpStatusPars(std::string file);
        ~HttpStatusPars();
        std::string getStatus(std::string key);
};


