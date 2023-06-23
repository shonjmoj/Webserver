#include "location.hpp"
#include "algorithm"

Location::Location(std::vector<std::string> &data, std::string &block) : _auto_index(false), _cgi(false), _exact(false), _redirect(false), _methods(0),_redirects(std::make_pair(0,""))
{
	validLocation(data);
	size_t	pos = block.find_first_of('{');
	if (pos == block.npos)
		throw "Server Block should be between {}";
	else
		block[pos] = ' ';
	pos = block.find_last_of('}');
	if (pos == block.npos)
		throw "Server Block should be between {}";
	else
		block[pos] = ' ';
	parsLocation(block);
	if (!_root.size() && !_redirect)
		throw "you should set a root or return directive";
}

Location::~Location()
{
}

void	Location::validLocation(std::vector<std::string> &data)
{
	if (data.size() == 3 && data[1] != "=")
		throw "Invalid location directive";
	else if (data[1] == "=")
	{
		_exact = true;
		data.erase(data.begin()+1);
	}
	_location = data[1];
	_location = tooLower(_location);
	return ;
}

void	Location::parsLocation(std::string &data)
{
	size_t						pos;
	std::string					line;
	std::vector<std::string>	tmp;
	bool						flag = false;

	while (1)
	{
		pos = data.find('\n');
		if (pos != data.npos)
			line = data.substr(0, pos);
		else
		{
			line = data;
			flag = true;
		}
		line = trimWS(line);
		if (flag && line.empty())
			break;
		else if (line.empty() || line[0] == '#' || line == "{")
		{
			data = data.substr(pos+1);
			continue;
		}
		parsLine(line, tmp);
		if (line[line.size()-1] != ';' && tmp[0] != "location")
			throw "Simple directives should end with ';'";
		else
		{
			checkValidDirective(tmp);
			tmp.clear();
		}
		if (pos != data.npos)
			data = data.substr(pos+1);
		else
			data = "";
	}
	
}

void	Location::checkValidDirective(std::vector<std::string> &data)
{
	if (data[0] == "root")
		setRoot(data);
	else if (data[0] == "index")
		setIndex(data);
	else if (data[0] == "autoindex")
		setAutoIndex(data);
	else if (data[0] == "methods")
		setMethods(data);
	else if (data[0] == "cgi")
		setCgi(data);
	else if (data[0] == "return")
		setRedirects(data);
	else
		throw "Invalid directive '" + data[0] + "'";
	return ;
}

void	Location::setRoot(std::vector<std::string> &data)
{
	if (data.size() != 2)
		throw "root directive invalid number of arguments";
	else if (!_root.empty())
		throw "root directive can't be empty";
	_root = data[1];
	return ;
}

void	Location::setIndex(std::vector<std::string> &data)
{
	for(size_t i = 1; i < data.size(); i++)
		_index.push_back(data[i]);
}

void	Location::setAutoIndex(std::vector<std::string> &data)
{
	if (data.size() != 2)
		throw "autoindex directive invalid number of arguments";
	else if (data[1] != "on" && data[1] != "off")
		throw "autoindex directive invalid argument '" + data[1] + "'";
	if (data[1] == "on")
		_auto_index = true;
	return ;
}

void	Location::setMethods(std::vector<std::string> &data)
{
	for(size_t i = 1; i < data.size(); i++)
		data[i] = tooLower(data[i]);
	for(size_t i = 1; i < data.size(); i++)
	{
		if (data[i] == "post")
		{
			if (_methods % 2 == 0)
				_methods += 1;
		}
		else if (data[i] == "get")
		{
			if (_methods < 6)
				_methods += 2;
		}
		else if (data[i] == "delete")
		{
			if (_methods < 4)
				_methods += 4;
		}
		else
			throw "methods directive invalid argument '" + data[i] + "'";
	}
	return ;
}

void	Location::setCgi(std::vector<std::string> &data)
{
	if (data.size() != 3)
		throw "cgi directive invalid number of arguments";
	if (data[1] != ".py" && data[1] != ".rb")
		throw "cgi directive invalid format '" + data[1] + "'";
	_cgi = true;
	_cgis[data[1]] = data[2];
	return ;
}

void	Location::setRedirects(std::vector<std::string> &data)
{
	if (data.size() != 3)
		throw "return directive invalid number of arguments";
	if (data[1].size() > 3)
		throw "invalid return code '" + data[1] + "'";
	for(size_t i = 0; i < data[1].size(); i++)
		if (!isdigit(data[1][i]))
			throw "invalid return code '" + data[1] + "'";
	_redirect = true;
	_redirects.first = atoi(data[1].c_str());
	_redirects.second = data[2];
}

std::string	Location::getlocation(void)
{
	return (_location);
}

std::string	Location::getRoot(void)
{
	return (_root);
}

std::vector<std::string>	Location::getIndex(void)
{
	return (_index);
}

bool	Location::isAutoIndex(void)
{
	return (_auto_index);
}

bool	Location::isExact(void)
{
	return (_exact);
}

bool	Location::isCgi(void)
{
	return (_cgi);
}

std::string	Location::getCgi(std::string format)
{
	size_t pos;

	pos = format.find_last_of('.');
	return (_cgis[format.substr(pos)]);
}

std::pair<int, std::string>	Location::getRedirects(void)
{
	return (_redirects);
}

bool	Location::getMethods(std::string method)
{
	method = tooLower(method);
	if (method == "post" && ((_methods >> 0) & 1))
		return (true);
	else if (method == "get" && ((_methods >> 1) & 1))
		return (true);
	else if (method == "delete" && ((_methods >> 2) & 1))
		return (true);
	return (false);
}

bool	Location::checkFormat(std::string file)
{
	size_t pos;

	pos = file.find_last_of('.');
	if (pos == file.npos)
		return (false);
	if (_cgis.find(file.substr(pos)) == _cgis.end())
		return (false);
	return (true);
}