#include "Location.hpp"

Location::Location()
{
    return;
}

Location::Location(const std::string redirectTo, const std::string locationRoot)
{
    this->setRedirectTo(redirectTo);
    this->setLoctionRoot(locationRoot);
}

Location::~Location()
{
    return;
}

bool Location::getRedirect() const
{
    return this->_redirect;
}

const std::string &Location::getRedirectTo() const
{
    return this->_redirectTo;
}

const std::string &Location::getLocationRoot() const
{
    return this->_loctionRoot;
}