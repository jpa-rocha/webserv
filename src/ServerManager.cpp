#include "../include/ServerManager.hpp"

ServerManager::ServerManager(std::vector<Config> configs): _configs(configs), _nfds(0) {


    for (size_t i = 0; i < this->_configs.size(); i++)
    {
        Server server = Server(this->_configs[i]);
        if (server.getError() != 0)
            continue ;
       this->_servers.push_back(server);
    }

	struct pollfd fds[MAX_CONN * this->_servers.size()];
    this->pollfd_init(fds);
	this->_nfds = this->_servers.size();
	while (true)
    {
        int nready = poll(fds, this->_nfds, -1);
        if (nready == -1)
        {
            perror("poll");
			// TODO return error
            return ;
        }
        //Check for new connection
        for (size_t i = 0; i < this->_servers.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                int connfd;
                struct sockaddr_in cli_addr;
                socklen_t cli_len = sizeof(cli_addr);
                if ((connfd = accept(this->_servers[i].get_sockfd(), (struct sockaddr *)&cli_addr, &cli_len)) < 0)
                {
                    perror("accept");
                    // TODO return error
            		return ;
                }

               std::cout << GREEN << "Received new connection\n" << RESET << std::endl;

                //Add new connection to poll
				size_t j;
                for (j = this->_servers.size(); j < MAX_CONN * this->_servers.size(); j++)
                {
                    if (fds[j].fd == -1)
                    {
                        fds[j].fd = connfd;
                        fds[j].events = POLLIN;
                        break;
                    }
                }

                if (j == MAX_CONN * this->_servers.size())
                {
                    std::cerr << "Too many connections" << std::endl;
                }
                else
                {
                    this->_nfds++;
                }
                if (--nready <= 0)
                {
                    break ;
                }
            }
        }
        if (!nready)
            continue ;
        //Check for data on all connections
        for (size_t i = this->_servers.size(); i < this->_nfds; i++)
        {
            int connfd = fds[i].fd;
            if (connfd == -1)
            {
                continue;
            }

            if (fds[i].revents & (POLLIN | POLLERR))
            {
		        int n;
                char buff[this->_servers[0].get_config().get_client_max_body_size()];
				memset(buff, 0, 1024);
                n = read(connfd, buff, sizeof(buff));            
                std::cout << "read return: " << n << std::endl;
                if ( n < 0 )
                {
                    if (errno != ECONNRESET) //TODO cannot use errno
                    {
						std::cerr << "this is n - " << n << std::endl;
                        perror("error while reading the fd");
                    }
                }
                else if (n == 0)
                {
                    printf("Connection closed\n");
                    close(connfd);
                    fds[i].fd = -1;
                }
                else
                {
                    //make object of class
					//print function of class
                    httpHeader request(buff);
                    request.printHeader();
					memset(buff, 0, 1024);
					//std::cout << GREEN << request.getUri() << RESET << std::endl;
					this->_servers[0].send_response(connfd, request.getUri());
                }

                if (--nready <= 0)
                {
                    break;
                }
            }
			close(connfd);
			fds[i].fd = -1;
        }
    }

}

ServerManager::~ServerManager() {}

void ServerManager::pollfd_init(struct pollfd* fds)
{
	for (size_t i = 0; i < this->_configs.size(); i++)
    {
        fds[i].fd = this->_servers[i].get_sockfd();
        fds[i].events = POLLIN;
    }
	for (size_t i = this->_servers.size(); i < MAX_CONN * this->_servers.size(); i++)
    {
        fds[i].fd = -1;
        fds[i].events = -1;
    }
}