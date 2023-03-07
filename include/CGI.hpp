#ifndef CGI_HPP
# define CGI_HPP

# include <string> 
# include <vector>
# include <map>
# include <iostream>
# include <stdlib.h>
# include "Utils.hpp"

class CGI {
	private:
		std::string								_root;
		std::map<std::string, std::string>		_path;
		std::vector<std::string>				_ext;
		int										_error_code;
	public:
		CGI();
		CGI(std::ifstream& config_file, std::string line);
		CGI& operator=(const CGI& obj);
		// Setters
		void set_root(std::string root);
		void set_path(std::string program, std::string path);
		void set_ext(std::string ext);
		void set_error_code(int error_code);
		
		// Getters
		std::string								get_root() const;
		std::map<std::string, std::string>		get_path() const;
		std::vector<std::string>				get_ext() const; 
		int 									get_error_code();

		int										cgi_check();
		void									clean_path(std::string line);
		void									clean_ext(std::string line);
};

std::ostream &operator<<(std::ostream &os, const CGI &cgi);

#endif