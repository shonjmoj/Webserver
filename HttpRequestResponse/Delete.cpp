#include "HttpRequest.hpp"
#include <ctime>
#include <vector>
#include <string>

std::string getRealPath(std::string path) {
    char resolvedPath[PATH_MAX];
    if (realpath(path.c_str(), resolvedPath) != NULL) {
        if(path[path.size() - 1] == '/')
            return std::string(resolvedPath) + "/";
        return std::string(resolvedPath);
    }
    return std::string();
}

bool isPathInPath(std::string path1, std::string path2) {
    std::string realPath1 = getRealPath(path1);
    std::string realPath2 = getRealPath(path2);

    return (realPath1.length() <= realPath2.length()) && (realPath2.compare(0, realPath1.length(), realPath1) == 0);
}

void delete_directory(const std::string& path) {
    DIR* dir_ptr = opendir(path.c_str());
    
    struct dirent* dir_entry;

    while ((dir_entry = readdir(dir_ptr)) != nullptr) {
        std::string name = dir_entry->d_name;

        if (name == "." || name == "..") {
            continue;
        }

        std::string full_path = path + "/" + name;

        if (dir_entry->d_type == DT_DIR) {
            delete_directory(full_path);

            if (rmdir(full_path.c_str()) != 0) {
                std::cerr << "Error deleting directory " << full_path << std::endl;
            }
        } 
        else {
            if (unlink(full_path.c_str()) != 0) {
                std::cerr << "Error deleting file " << full_path << std::endl;
            }
        }
    }

    closedir(dir_ptr);
}

std::string HttpRequest::getCurentTime()
{
    std::string str;
    time_t now = time(0);
    char * dt = ctime(&now);
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    str = dt;
    std::vector<std::string> v(5);
    size_t found;

    for (size_t i = 0; i < 5; i++)
    {

        found = str.find_first_of(" ");
        if (found != std::string::npos)
        {
            v[i] = str.substr(0,found);
            str = str.substr(found + 1, str.size());
        }
    }

    found = str.find_first_of("\n");
    if (found != std::string::npos)
    {
        str = str.erase(found);
    }
    return (v[0] + ", " + v[2] + " " + v[1] + " " + str + " " + v[3] +" " + v[4] +" GMT");
}

char    hex_to_char(std::string num)
{
    int r, len, hex = 0;

    len = num.size();
    for (size_t i = 0; i < num.size(); i++)
    {
        len--;
        if (num[i] >= '0' && num[i] <= '9')
            r = num[i] - 48;
        else if(num[i] >= 'A' && num[i] <= 'F')
            r = num[i] - 55;
        hex += r * pow(16, len);
    }
    return ((char)hex);
}

std::string  url_encoding(std::string str)
{
    size_t found;

    found = str.find_first_of("%");
    if (found != std::string::npos && found + 2 < str.size())
    {
        std::string suffix, preffix, di;
        char c;
        str.erase(str.begin() + found);
        preffix = str.substr(0,found);
        suffix = str.substr(found + 2, str.size());
        di = str.substr(found, 2);

        c = hex_to_char(di);
        str = preffix + c + suffix;
        str = url_encoding(str);
    }
    return str;
}