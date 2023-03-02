#include "../include/ServerManager.hpp"

ServerManager::ServerManager(std::vector<Config> configs): _configs(configs), _nfds(0) {


    for (size_t i = 0; i < this->_configs.size(); i++)
    {
        Server server = Server(this->_configs[i]);
        if (server.getError() != 0)
            continue ;
       this->_servers.push_back(server);
    }

	this->_fds = new struct pollfd[MAX_CONN * this->_servers.size()];
    this->pollfd_init();
	this->_nfds = this->_servers.size();
	this->run_servers();
}

ServerManager::~ServerManager()
{
	delete this->_fds;
}

void ServerManager::pollfd_init()
{
	for (size_t i = 0; i < this->_configs.size(); i++)
    {
        this->_fds[i].fd = this->_servers[i].get_sockfd();
        this->_fds[i].events = POLLIN;
    }
	for (size_t i = this->_servers.size(); i < MAX_CONN * this->_servers.size(); i++)
    {
        this->_fds[i].fd = -1;
        this->_fds[i].events = -1;
    }
}

int ServerManager::run_servers()
{
	while (true)
    {
		// TODO this->_nready what does it mean? better name?
        this->_nready = poll(this->_fds, this->_nfds, -1);
        if (this->_nready == -1)
        {
            perror("poll");
			// TODO return error
            return 1;
        }

        //Check for new connection
		this->check_connection();
        if (!this->_nready)
            continue ;
        //Check for data on all connections
       check_request_respond();
    }
	return EXIT_SUCCESS;
}

int		ServerManager::check_connection()
{
        for (size_t i = 0; i < this->_servers.size(); i++)
        {
            if (this->_fds[i].revents & POLLIN)
            {
                int connfd;
                struct sockaddr_in cli_addr;
                socklen_t cli_len = sizeof(cli_addr);
                if ((connfd = accept(this->_servers[i].get_sockfd(), (struct sockaddr *)&cli_addr, &cli_len)) < 0)
                {
                    perror("accept");
                    // TODO return error
            		return 1;
                }

               std::cout << GREEN << "Received new connection\n" << RESET << std::endl;

                //Add new connection to poll
				size_t j;
                for (j = this->_servers.size(); j < MAX_CONN * this->_servers.size(); j++)
                {
                    if (this->_fds[j].fd == -1)
                    {
                        this->_fds[j].fd = connfd;
                        this->_fds[j].events = POLLIN;
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
                if (--this->_nready <= 0)
                {
                    break ;
                }
            }
        }
		return EXIT_SUCCESS;
}

int		ServerManager::check_request_respond()
{
	for (size_t i = this->_servers.size(); i < this->_nfds; i++)
	{
		int connfd = this->_fds[i].fd;
		if (connfd == -1)
		{
			continue;
		}

		if (this->_fds[i].revents & (POLLIN | POLLERR))
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
				this->_fds[i].fd = -1;
			}
			else
			{
				httpHeader request(buff);
				request.printHeader();
				memset(buff, 0, 1024);
				this->_servers[0].send_response(connfd, request.getUri());
			}

			if (--this->_nready <= 0)
			{
				break;
			}
		}


		close(connfd);
		this->_fds[i].fd = -1;
	}
	return EXIT_SUCCESS;
}