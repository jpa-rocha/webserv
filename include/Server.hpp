#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"
# include "Config.hpp"
# include "minilib.hpp"
# include <sstream>
# include <fstream>
# include <iostream>
# include <string>
# include <cstring>
# include <fcntl.h>

class Server
{
	private:
		struct sockaddr_in	_serv_addr;
		Config      	   	_config;
		int 				_sockfd;
    	int 				_port;
		int					_error;

	
		/* Server();
		Server(const Server& copy); */
		// Server& operator=(const Server& rhs);
    public:
		Server(Config config);
		~Server();

		int		init_socket();
		int		bind_socket();
		int		listen_socket();
		
		// getters
		
		int     get_sockfd() const;
        int     get_port() const;
		int	    getError()	const;
		Config &get_config();
		
		// utils
		
		void	send_response(int client_socket, const std::string& path);
		int		clean_fd();
		int		handle_cgi(std::string& path);
		void	exec_script(int pipe_end, std::string path);
};

#endif