#ifndef CGI_HPP
# define CGI_HPP

# include "Utils.hpp"
# include "Config.hpp"

class CGI {
	private:
		std::string				_path;
		std::string				_response_body;
		Config					_config;
		
	public:
		CGI(const CGI& obj);
		CGI(Config config, const std::string path, std::string response_body);
		CGI& operator=(const CGI& obj);
		~CGI();

		int		handle_cgi(std::ostringstream &response_stream);
		void	exec_script(int *pipe, std::string path, std::string program);
};



#endif