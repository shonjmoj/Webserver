#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "../utils.hpp"

class	Location
{
	private:
		bool								_auto_index;
		bool								_cgi;
		bool								_exact;
		bool								_redirect;
		int									_methods;
		std::string							_location;
		std::string							_root;
		std::pair<int, std::string>			_redirects;
		std::vector<std::string>			_index;
		std::map<std::string, std::string>	_cgis;

	public:
		Location(std::vector<std::string> &data, std::string &block);
		~Location();
		void	validLocation(std::vector<std::string> &data);
		void	parsLocation(std::string &data);
		void	checkValidDirective(std::vector<std::string> &data);
		void	setRoot(std::vector<std::string> &data);
		void	setIndex(std::vector<std::string> &data);
		void	setAutoIndex(std::vector<std::string> &data);
		void	setMethods(std::vector<std::string> &data);
		void	setCgi(std::vector<std::string> &data);
		void	setRedirects(std::vector<std::string> &data);
		bool								isAutoIndex(void);
		bool								isExact(void);
		bool								isCgi(void);
		bool								getMethods(std::string);
		bool								checkFormat(std::string file);
		std::string							getCgi(std::string);
		std::string							getlocation(void);
		std::string							getRoot(void);
		std::vector<std::string>			getIndex(void);
		std::pair<int, std::string>			getRedirects(void);
};

#endif