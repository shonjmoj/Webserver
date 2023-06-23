#include "HttpRequest.hpp"

std::string GeneratedHtmlError(std::string code)
{
    std::string content = "<html>"
                         "<head><title>" + code + " - </title></head>"
                         "<body style=\"font-family: Arial, sans-serif;\">"
                         "<div style=\"width: 80%; margin: 0 auto; padding-top: 50px;\">"
                         "<h1 style=\"font-size: 36px; margin-bottom: 20px;\"> " + code + "</h1>"
                         "<p style=\"font-size: 18px;\">Sorry, the requested page could not be found. Ask lagala</p>"
                         "</div>"
                         "</body>"
                         "</html>";
    return content;
}


std::string HttpRequest::generateHTMLPage(const std::string& directoryPath, const std::string& uri)
{
    std::string output;

    DIR* directory = opendir(directoryPath.c_str());
    if (!directory)
    {
        return "";
    }

    output += "<html>\n";
    output += "<head>\n";
    output += "<title>"+uri+"</title>\n";
    output += "<style>\n";
    output += "body { background-color: #f2f2f2; }\n";
    output += "h1 { color: #333; text-align: center; margin-top: 50px; }\n";
    output += "ul { list-style-type: none; padding: 0; }\n";
    output += ".file { margin: 10px 0; }\n"; // Style for files
    output += ".directory { margin: 10px 0; font-size: 22px; color: gray; }\n"; // Style for directories
    output += "a { text-decoration: none; color: #555; }\n";
    output += "a:hover { text-decoration: underline; color: gray; }\n";
    output += ".indent { padding-left: 20px; }\n";
    output += "</style>\n";
    output += "</head>\n";
    output += "<body>\n";
    output += "<h1>"+uri+"</h1>\n";
    output += "<ul>\n";

    struct dirent* entry;
    while ((entry = readdir(directory)))
    {
        std::string fileName = entry->d_name;
        std::string filePath = fileName;

        if (entry->d_type == DT_REG)
            output += "<li class=\"file\"><a href=\"" + filePath + "\">" + "<span class=\"indent\"></span>" + fileName + "</a></li>\n";
        else if (entry->d_type == DT_DIR && fileName != "." && fileName != "..")
            output += "<li class=\"directory\"><a href=\"" + filePath + "/\">" + "<span class=\"indent\"></span>" + fileName + "/</a></li>\n";
    }

    output += "</ul>\n";
    output += "</body>\n";
    output += "</html>\n";

    closedir(directory);
    return output;
}

