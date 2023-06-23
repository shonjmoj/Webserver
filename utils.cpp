#include "utils.hpp"

std::string trimWS(std::string str)
{
    std::string ws = " \n\r\t\f\v";
    size_t      start = str.find_first_not_of(ws);
    size_t      end = str.find_last_not_of(ws);

	if (start == str.npos && isspace(str[0]))
		return ("");
    if (start == str.npos)
        start = 0;
    if (end == str.npos)
        end = str.size();
    str = str.substr(start);
    str = str.substr(0, end - start + 1);
    return (str);
}

size_t	getNextBraces(std::string &str, size_t i)
{
	int		lvl = 1;
	bool	com = false;

	while (str[i])
	{
		if (str[i] == '\n')
			com = false;
		if (str[i] == '#')
			com = true;
		if (str[i] == '{' && !com)
			lvl++;
		else if (str[i] == '}' && !com)
			lvl--;
		if (!lvl)
			return (i);
		if (lvl < 0)
			return (str.npos);
		i++;
	}
	return (str.npos);
}

void	parsLine(std::string &line, std::vector<std::string> &tmp)
{
	std::stringstream	ss(line);
	std::string			key;
	size_t				i;

	while (ss >> key)
		tmp.push_back(key);
	i = tmp.size() - 1;
	if (tmp[i] == ";")
		tmp.pop_back();
	else if (tmp[i][tmp[i].size()-1] == ';')
		tmp[i].erase(tmp[i].size()-1);
	else if (tmp[0] == "location" && tmp[i] == "{")
		tmp.erase(tmp.begin()+i);
	else if (tmp[0] == "location")
	{
		if (tmp[i][tmp[i].size()-1] == '{')
			tmp[i].erase(tmp[i].size()-1);
	}	
	else
		throw "Bad directive";
	if (tmp.size() == 1)
		throw "Location must have valid path";
	return ;
}

std::string tooLower(std::string s) {
  for (std::string::iterator i = s.begin(); i != s.end(); ++i) {
    *i = tolower(*i);
  }
  return s;
}