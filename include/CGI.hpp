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

		// Setters
		void set_root(std::string root);
		void set_path(std::map<std::string, std::string> path);
		void set_ext(std::vector<std::string> ext);

		// Getters
		std::string								get_root();
		std::map<std::string, std::string>		get_path();
		std::vector<std::string>				get_ext(); 
};

#endif