#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <stdlib.h>
# include <string_view>
# include "Config.hpp"
# include "Utils.hpp"

static std::string config_variables[] = {"listen", "host", "server_name", "cgi_ext",
										"error_page", "allow_methods", "client_max_body_size",
										"root", "index", "autoindex", "return", "cgi_path"};

class ConfigParser {
	private:
		std::vector<Config> _config;
		int					_n_servers;
	public:
		ConfigParser();
		ConfigParser(std::string config_file);
		

		// Getter
		Config get_Config();

		// Utils
		std::string remove_comments(std::string line);

		// Error Handling
		int exit_with_error(int err_code, std::ifstream& in_file);
		bool check_server_context(std::ifstream& config_file, std::string& line);
};
		


#endif