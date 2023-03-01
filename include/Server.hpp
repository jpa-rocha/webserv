#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"
# include "Config.hpp"

class Server
{
	private:
		struct sockaddr_in	_serv_addr;
		Config      	   	_config;
		int 				_sockfd;
    	int 				_port;
    	char* 				_buf;
		int					_error;
	
		/* Server();
		Server(const Server& copy); */
		// Server& operator=(const Server& rhs);
    public:
		Server(Config config);
		~Server();

		int     get_sockfd() const;
		char*   get_buf() const;
        int     get_port() const;
		int	    getError()	const;
};

#endif