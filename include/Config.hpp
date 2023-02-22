#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <arpa/inet.h>
#include <vector>
#include <map>

#include "Location.hpp"

	class Config {
		private:
			uint16_t					_port;
  			in_addr_t					_host;                  
  			std::string					_server_name;
 			std::map<int, std::string>	_default_error;
  			int 						_client_max_body_size;
			bool						_autoindex;
  			std::string					_root;
  			std::string					_index;
  			std::vector<Location>		_location;              
	};

#endif