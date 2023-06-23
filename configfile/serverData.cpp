#include "serverData.hpp"

servData::servData(std::string data) : _cmbs(-1)
{
	size_t	pos = data.find_first_of('{');
	if (pos == data.npos)
		throw "Server Block should be between {}";
	else
		data[pos] = ' ';
	pos = data.find_last_of('}');
	if (pos == data.npos)
		throw "Server Block should be between {}";
	else
		data[pos] = ' ';
	data = trimWS(data);
	if (data.empty())
		throw "Config file Can't be empty";
	getDataServ(data);
	if (!_ports.size())
		throw "Server Directive should have at least one port to listen on it";
	if (_root.empty())
		throw "You need to setup a root";
}

servData::~servData()
{
}

void	servData::getDataServ(std::string &data)
{
	std::string 				line;
	std::string 				block;
	size_t						pos;
	std::vector<std::string>	tmp;
	bool						flag = false;

	while(1)
	{
		pos = data.find_first_of('\n');
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
			pos = data.find_first_of('\n');
			if (pos != data.npos)
				data = data.substr(pos+1);
			else
				data = "";
			if (data.empty())
				break;
			continue;
		}
		if (line[line.size()-1] != ';' && line[line.size()-1] != '{' && line.substr(0, line.find(' ')) != "location")
			throw "Simple directives should end with ';'";
		else
		{
			parsLine(line, tmp);
			if (tmp[0] == "location")
			{
				block = getBlock(data);
				setLocations(tmp, block);
			}
			else
				checkValidDirective(tmp);
			tmp.clear();
		}
		pos = data.find_first_of('\n');
		if (block.size())
			block.clear();
		else if (pos != data.npos)
			data = data.substr(pos+1);
		else
			data = "";
	}
	return ;
}

void	servData::checkValidDirective(std::vector<std::string> &data)
{
	if (data[0] == "listen")
		setPort(data);
	else if (data[0] == "server_name")
		setServerName(data);
	else if (data[0] == "root")
		setRoot(data);
	else if (data[0] == "client_max_body_size")
		setCMBS(data);
	else if (data[0] == "error_page")
		setErrorPage(data);
	else
		throw "Invalid directive '" + data[0] + "'";
	return ;
}

void	servData::setPort(std::vector<std::string> &data)
{
	size_t	pos;
	if (data.size() != 2)
		throw "listen directive invalid number of arguments";
	if (_ports.find(data[1]) != _ports.end())
		throw "Duplicate port '" + data[1] + "'";
	pos = data[1].find(':');
	if(pos == std::string::npos || !validPort(data[1].substr(pos+1)) || !validHost(data[1].substr(0,pos)))
		throw "Invalid port '" + data[1] + "'";
	_ports.insert(data[1]);
	return ;
}

void	servData::setServerName(std::vector<std::string> &data)
{
	if (data.size() < 2)
		throw "server_name directive invalid number of arguments";
	for(size_t i = 1; i < data.size(); i++)
		_servers_name.insert(data[i]);
	return ;
}

void	servData::setRoot(std::vector<std::string> &data)
{
	if (data.size() != 2)
		throw "root directive invalid number of arguments";
	else if (!_root.empty())
		throw "Invalid root";
	_root = data[1];
	return ;
}

void	servData::setCMBS(std::vector<std::string> &data)
{
	char	c;

	if (data.size() != 2)
		throw "client_max_body_size directive invalid number of arguments";
	if (_cmbs != -1)
		throw "client_max_body_size directive is duplicate";
	c = validCMBS(data[1]);
	if (data[1].size() > 17 || !c)
		throw "client_max_body_size directive invalid value";
	_cmbs = atoll(data[1].c_str());
	if (c == 'k' || c == 'K')
	{
		if (_cmbs > 1e9)
			throw "client_max_body_size directive invalid value";
		_cmbs *= 1024;
	}
	else if (c == 'm' || c == 'M')
	{
		if (_cmbs > 1e6)
			throw "client_max_body_size directive invalid value";
		_cmbs *= 1024 * 1024;
	}
	else if (c == 'g' || c == 'G')
	{
		if (_cmbs > 1e3)
			throw "client_max_body_size directive invalid value";
		_cmbs *= 1024 * 1024 * 1024;
	}
}

void	servData::setLocations(std::vector<std::string> &data, std::string &block)
{
	Location *obj = new Location(data, block);
	if (!_locations.size())
		_locations.push_back(make_pair(obj->getlocation(), obj));
	else
	{
		for(size_t i = 0; i < _locations.size(); i++)
		{
			if (obj->getlocation().size() > _locations[i].first.size())
			{
				_locations.insert(_locations.begin()+i, make_pair(obj->getlocation(), obj));
				return;
			}
		}
		_locations.push_back(make_pair(obj->getlocation(), obj));
	}
}

void	servData::setErrorPage(std::vector<std::string> &data)
{
	std::string	errpage;

	if (data.size() < 3)
		throw "error_page directive invalid number of arguments";;
	errpage = data[data.size() - 1];
	for(size_t i = 1; i < data.size() - 1; i++)
	{
		if (data[i].size() != 3 || atoi(data[i].c_str()) < 300 || atoi(data[i].c_str()) >= 600)
			throw "value '" + data[i] + "' must be between '300' and '599'";
		_error_page[atoi(data[i].c_str())] = errpage;
	}
}

int	servData::validHost(std::string host)
{
	std::stringstream			ss;
	std::string					tmp(host);
	std::string					str;
	std::vector<std::string>	v;
	int							p = 0;

	for(int i = 0; host[i]; i++)
	{
		if (host[i] == '.')
		{
			host[i] = ' ';
			p++;
		}
		if (p > 3)
			throw "Invalid host '" + tmp + "'";
	}
	ss << host;
	while (ss >> str)
		v.push_back(str);
	if (v.size() != 4)
		throw "Invalid host '" + tmp + "'";
	for(size_t i = 0; i < v.size(); i++)
	{
		for(size_t j = 0; j < v[i].size(); j++)
		{
			if (!isdigit(v[i][j]))
				throw "Invalid host '" + tmp + "'";
		}
		if (atoi(v[i].c_str()) < 0 || atoi(v[i].c_str()) > 255)
				throw "Invalid host '" + tmp + "'";
	}
	return (1);
}

int	servData::validPort(std::string port)
{
	int	p = atoi(port.c_str());
	if (p < 1024 || p > 65535)
		return (0);
	for(size_t i = 0; i < port.size(); i++)
		if (!isdigit(port[i]))
			return (0);
	return (1);
}

char	servData::validCMBS(std::string &cmbz)
{
	for(size_t i = 0; i < cmbz.size() - 1; i++)
		if (!isdigit(cmbz[i]))
			return (0);
	if (isalpha(cmbz[cmbz.size() - 1]))
	{
		if (cmbz[cmbz.size() - 1] == 'm' || cmbz[cmbz.size() - 1] == 'M')
			return (cmbz[cmbz.size() - 1]);
		if (cmbz[cmbz.size() - 1] == 'g' || cmbz[cmbz.size() - 1] == 'G')
			return (cmbz[cmbz.size() - 1]);
		if (cmbz[cmbz.size() - 1] == 'k' || cmbz[cmbz.size() - 1] == 'K')
			return (cmbz[cmbz.size() - 1]);
	}
	if (isdigit(cmbz[cmbz.size() - 1]))
		return (1);
	return (0);
}

std::set<std::string>	servData::getPorts(void)
{
	return (_ports);
}

std::set<std::string>	servData::getServersName(void)
{
	return (_servers_name);
}

std::string	servData::getRoot(void)
{
	return (_root);
}

long long	servData::getCMBS(void)
{
	return (_cmbs);
}

std::string	servData::getErrorPage(int error)
{
	if(_error_page.size() == 0)
		return ("");
	if (_error_page.find(error) != _error_page.end())
	{
		return (_error_page[error]);
	}
	return ("");
}

std::string	servData::getBlock(std::string &block)
{
	size_t		start;
	size_t		end;
	std::string	ret;

	start = block.find("{");
	end = getNextBraces(block, start+1);
	ret = block.substr(start, end - start + 1);
	block = block.substr(end+1);
	return (ret);
}

std::vector<std::pair<std::string, Location *> >	servData::getLocations(void)
{
	return (_locations);
}