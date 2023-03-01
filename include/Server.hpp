#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"
# include "Config.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>

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

		int     get_sockfd() const;
        int     get_port() const;
		int	    getError()	const;
		Config &get_config();

		void send_response(int client_socket, const std::string& path);
};

#endif