#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <unistd.h>

std::string trimWS(std::string str);
size_t		getNextBraces(std::string &str, size_t i);
void		parsLine(std::string &line, std::vector<std::string> &tmp);
std::string tooLower(std::string s);
#endif