#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "../utils.hpp"
#include "serverData.hpp"
#include "HttpStatusPars.hpp"

class	configFile
{
	private:
		std::ifstream												_configData;
		std::string													_fileName;
		std::string													_data;
		std::set<std::string>										_ports;
		servData													*_first;
		std::map<std::string,std::string>							_types;
		std::map<std::string, servData*>							_firsts;
		std::map<std::string, std::pair<bool, servData *> >			_servers_h;
		std::map<std::pair<std::string, std::string>, servData *>	_servers_n;
	public:
		HttpStatusPars												_status;
		configFile(std::string fileName);
		~configFile();
		void						getDataConf(void);
		void						getServerDirective(void);
		bool						isValid(std::string &str, std::string cmp);
		void						setServers(servData *obj);
		void						setTypes(void);
		servData					*getServer(std::pair<std::string,std::string> key);
		std::vector<std::string>	getPorts(void);
		std::string					getType(std::string type);
};

#endif