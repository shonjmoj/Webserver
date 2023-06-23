#include "HttpRequest.hpp"
#include <iostream>
#include <vector>
#include <string>

int counLongtMatching(const std::string& path, const std::string& location) {
    int count = 0;
    size_t i = 0;
    size_t j = 0;
    if (!(location.size() > 0 && location[0] == '/') || !(path.size() > 0 && path[0] == '/') || (path.size() < location.size()))
        return -1;
    while (i < path.size() && path[i] == '/') {
        i++;
    }

    while (j < location.size() && location[j] == '/') {
        j++;
    }
    while (i < path.size() && j < location.size()) {
        size_t i2 = path.find('/', i);
        size_t j2 = location.find('/', j);
        std::string seg1 = path.substr(i, i2 - i);
        std::string seg2 = location.substr(j, j2 - j);

        if (seg1 == seg2) {
            count++;
        }
        else
            return -1;
        i = i2;
        j = j2;


        while (i < path.size() && path[i] == '/') {
            i++;
        }

        while (j < location.size() && location[j] == '/') {
            j++;
        }
    }

    if (i == path.size() && j == location.size()) {
        count++;

    }
    return count;
}


void HttpRequest::getFindLocation(const std::vector<std::pair<std::string, Location*> > locations) {
    int maxCount = -1;
    std::string matchingLocation;
    Location* locat = nullptr;

    this->location = this->path;
    for (size_t i = 0; i < locations.size(); i++) {
        int count = counLongtMatching(this->path, locations[i].first);
        if (count != -1 && count > maxCount)
        {
            maxCount = count;
            matchingLocation = locations[i].first;
            locat = locations[i].second;
        }
    }
    if(!matchingLocation.empty())
    {
        this->isLocationMatched = true;
        if(locat->getRedirects().first)
        {
            setstatus(to_string(locat->getRedirects().first));
            setStatuString(config->_status.getStatus(to_string(locat->getRedirects().first)));
            setResponseLocation(locat->getRedirects().second);
            getErrorhtml(status + " " + statusString);
            Uri = "";
            return;
        }
        if (!locat->getMethods(this->getMethod()))
        {
            isAllowed = false;
            return;
        }
        else
        {
            isAllowed = true;
            size_t pos = path.find(matchingLocation);
            if (pos != std::string::npos)
                this->path = this->path.substr(pos + matchingLocation.length());
            setUri(locat->getRoot());
            this->setUri(this->getUri() + "/" + this->getPath());
            this->Uri = getRealPath(this->Uri);
            if(Uri.empty())
            {
                setstatus("404");
                setStatuString("Not Found");
                getErrorhtml("404 Not Found");
                return;
            }
            if(Uri.find(locat->getRoot()) != 0)
            {
                setstatus("403");
                setStatuString("Forbiden");
                getErrorhtml("403 Forbiden");
                Uri = "";
                return;
            }
            setindex(locat->getIndex());
            setAutoIndex(locat->isAutoIndex());
            index = locat->getIndex();
            loc = locat;
        }
    }
    else {
        this->isLocationMatched = false;
    }
}