#include "configFile.hpp"

configFile::configFile(std::string fileName):_status("./conf/httpStatusCodes.conf")
{
	_fileName = fileName;
	_configData.open(_fileName);
	if (!_configData.is_open())
		throw "Error while trying opening " + _fileName;
	std::getline(_configData, _data, '\0');
	if (_configData.fail())
		throw "Error while getting content of " + _fileName;
	_configData.close();
	setTypes();
	getServerDirective();
}

configFile::~configFile()
{
}
void    configFile::setTypes(void)
{
    std::string                    	file_name = "./conf/mime.types";
    std::string                    	types;
    std::string                    	line;
    std::ifstream                	data;
    size_t                       	pos;
    std::vector<std::string>   		tmp;
    bool                        	valid = false;

    data.open(file_name);
    if (!data.is_open())
        throw "Error while trying opening " + file_name;
    std::getline(data, types, '\0');
    if (data.fail())
        throw "Error while getting content of " + file_name;
	data.close();
    while (1)
    {
        pos = types.find('\n');
        if (pos != line.npos)
        {
            line = types.substr(0,pos);
            types = types.substr(pos+1);
        }
        else
        {
            line = types;
            types = "";
        }
        line = trimWS(line);
        if (line.empty() && types.empty() && !valid)
            throw "Empty file " + file_name;
        if (line.empty() && types.empty())
            break;
        if (line.empty())
            continue;
        parsLine(line, tmp);
        if (tmp.size() < 2)
            throw "Invalid number of arguments in " + file_name;
        for(size_t i = 1; i < tmp.size(); i++)
            _types[tmp[i]] = tmp[0];
        tmp.clear();
        valid = true;
    }
    return ;
}
bool	configFile::isValid(std::string &str, std::string cmp)
{
	size_t		pos;
	std::string	line;

	while (1)
	{
		pos = str.find('\n');
		if (pos != str.npos)
		{
			line = str.substr(0, pos);
			str = str.substr(pos+1);
		}
		else
		{
			line = str;
			str = "";
		}
		line = trimWS(line);
		str = trimWS(str);
		if ((line[0] == '#' || line.empty()))
			continue;
		else if (line == cmp && str.empty())
			return (true);
		else
			return (false);
	}
	return (false);
}

void	configFile::getServerDirective(void)
{
	std::string	line;
	size_t		start;
	size_t		end;
	bool		empty = true;
	bool		valid = false;

	while (1)
	{
		_data = trimWS(_data);
		if (_data[0] == '#')
		{
			end = _data.find('\n');
			if (end != _data.npos)
				_data = _data.substr(_data.find('\n'));
			else if (!valid)
				throw "no server section in configuration";
			else
				break;
			continue;
		}
		start = _data.find("{");
		if (start == _data.npos && !empty)
		{
			while (1)
			{
				_data = trimWS(_data);
				if (_data[0] != '#')
					break;
				start = _data.find('\n');
				if (start == _data.npos)
				{
					_data = "";
					break;
				}
				else
					_data = _data.substr(start+1);
			}
			if (_data.size())
				throw "Bad Directive";
			else
				break;
		}
		end = getNextBraces(_data, start+1);
		if (end == _data.npos)
			throw "Bad Directive";
		line = _data.substr(0, start);
		if (line.empty())
			throw "Bad Directive";
		if (isValid(line, "server"))
		{
			servData *obj = new servData(_data.substr(start, end - start + 1));
			setServers(obj);
			if (!valid)
				_first = obj;
			valid = true;
			_data = _data.substr(end+1);
			empty = false;
		}
		else
			throw "Invalid Keyword";
	}
}

void	configFile::setServers(servData *obj)
{
	std::set<std::string> ports = obj->getPorts();
	std::set<std::string> ser_name = obj->getServersName();
	std::set<std::string>::iterator it_p;
	std::set<std::string>::iterator it_c;
	std::set<std::string>::iterator it_s;

	for(it_p = ports.begin(); it_p != ports.end(); it_p++)
	{
		_ports.insert(*it_p);
		if (_servers_h.find(*it_p) == _servers_h.end())
		{
			_servers_h[*it_p] = std::make_pair(false, obj);
			_firsts[*it_p] = obj;
		}
		else
			_servers_h[*it_p] = std::make_pair(true, obj);
	}
	for(it_s = ser_name.begin(); it_s != ser_name.end(); it_s++)
	{
		for(it_p = ports.begin(); it_p != ports.end(); it_p++)
		{
			if (_servers_n.find(make_pair(*it_p,*it_s)) == _servers_n.end())
				_servers_n[make_pair(*it_p,*it_s)] = obj;
		}
	}
}

servData	*configFile::getServer(std::pair<std::string,std::string> key)
{
	if (_servers_h.find(key.first) != _servers_h.end() && !_servers_h.find(key.first)->second.first)
		return ((_servers_h.find(key.first))->second.second);
	if (_servers_n.find(key) != _servers_n.end())
		return (_servers_n[key]);
	if (_firsts.find(key.first) != _firsts.end())
		return (_firsts[key.first]);
	return (_first);
}

std::vector<std::string>	configFile::getPorts(void)
{
	std::vector<std::string>		ports;
	std::set<std::string>::iterator	it_p;

	for(it_p = _ports.begin(); it_p != _ports.end(); it_p++)
		ports.push_back(*it_p);
	return (ports);
}

std::string	configFile::getType(std::string type)
{
	if (_types.find(type) != _types.end())
		return (_types.find(type)->second);
	return ("text/html");
}