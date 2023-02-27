#ifndef CGI_HPP
# define CGI_HPP

# include <string> 
# include <vector>
# include <map>
# include <iostream>

class CGI {
	private:
		std::string								_root;
		std::map<std::string, std::string>		_path;
		std::vector<std::string>				_ext; 
	public:
		CGI();
		CGI& operator=(const CGI& obj);
		// Setters
		void set_root(std::string root);
		void set_path(std::string program, std::string path);
		void set_ext(std::string ext);

		// Getters
		std::string								get_root() const;
		std::map<std::string, std::string>		get_path() const;
		std::vector<std::string>				get_ext() const; 
};

std::ostream &operator<<(std::ostream &os, const CGI &cgi);

#endif