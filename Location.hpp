#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>

class Location
{
    bool _redirect;
    std::string _redirectTo;
    std::string _loctionRoot;

public:
    Location();
    Location(const std::string redirectTo, const std::string locationRoot);
    ~Location();

    bool getRedirect() const;
    const std::string &getRedirectTo() const;
    const std::string &getLocationRoot() const;

    void setRedirect(bool redirect);
    void setRedirectTo(const std::string &redirectTo);
    void setLoctionRoot(const std::string &locationRoot);
};

#endif