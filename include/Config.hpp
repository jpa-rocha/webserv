#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <arpa/inet.h>
#include <vector>
#include <map>
#include <iostream>
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
		public:
			Config();
			Config(bool test);

			// getters
			u_int16_t					get_port();
			in_addr_t					get_host();                  
  			std::string					get_server_name();
 			std::map<int, std::string>	get_default_error();
  			int 						get_client_max_body_size();
			bool						get_autoindex();
  			std::string					get_root();
  			std::string					get_index();
  			std::vector<Location>		get_location();

			// setters
			void					set_port(u_int16_t port);
			void					set_host(in_addr_t host);                  
  			void					set_server_name(std::string server_name);
 			void					set_default_error(int i, std::string default_error);
  			void 					set_client_max_body_size(int clien_max_body_size);
			void					set_autoindex(bool autoindex);
  			void					set_root(std::string root);
  			void					set_index(std::string index);
  			void					set_location(std::vector<Location> location);

			// Utils

			
	};
	std::ostream& operator<<(std::ostream& os, Config& config);
#endif