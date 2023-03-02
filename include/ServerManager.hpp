#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include <poll.h>
# include "Server.hpp"
# include "ConfigParser.hpp"

class ServerManager {
	
    private:
		std::vector<Server> _servers;
		std::vector<Config> _configs;
		size_t				_nfds;
		struct pollfd*		_fds;
		int					_nready;

        // 	ServerManager(ServerManager const &copy);
        //  ServerManager &operator=(ServerManager const &rhs);
    
    public:
		ServerManager(std::vector<Config> configs);

        ~ServerManager();

		void 	pollfd_init();
		int		run_servers();
		int		check_connection();
		int		check_request_respond();

};

#endif