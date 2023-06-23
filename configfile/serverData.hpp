#ifndef SERVERDATA_HPP
#define SERVERDATA_HPP

#include "../utils.hpp"
#include "location.hpp"

class	servData
{
	private:
		std::set<std::string>								_ports;
		std::set<std::string>								_servers_name;
		std::string											_root;
		long long											_cmbs;
		std::map<int,std::string>							_error_page;
		std::vector<std::pair<std::string, Location *> >	_locations;
	
	public:
		servData(std::string data);
		~servData();
		void	getDataServ(std::string &data);
		void	checkValidDirective(std::vector<std::string> &data);
		void	setPort(std::vector<std::string> &data);
		void	setServerName(std::vector<std::string> &data);
		void	setRoot(std::vector<std::string> &data);
		void	setCMBS(std::vector<std::string> &data);
		void	setLocations(std::vector<std::string> &data, std::string &block);
		void	setErrorPage(std::vector<std::string> &data);
		int		validPort(std::string port);
		int		validHost(std::string port);
		char	validCMBS(std::string &cmbs);
		std::string											getBlock(std::string &block);
		std::set<std::string>								getPorts(void);
		std::set<std::string>								getServersName(void);
		std::string											getRoot(void);
		long long											getCMBS(void);
		std::string											getErrorPage(int error);
		std::vector<std::pair<std::string, Location *> >	getLocations(void);
};

#endif
