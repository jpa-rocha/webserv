#include "../include/ServerManager.hpp"

ServerManager::ServerManager(std::vector<Config> configs): _configs(configs), _nfds(0) {


    for (size_t i = 0; i < this->_configs.size(); i++)
    {
		/*
			TODO
			if config[i].get_error_code == 0
			try
				push to server
			catch
				faulty config
		*/
		try {
			this->_configs[i].check_config();
        	Server server = Server(this->_configs[i]);
       		this->_servers.push_back(server);
		}
		catch (std::logic_error &e) {
			std::cout << e.what() << std::endl;
		}
        /* if (server.getError() != 0)
            continue ; */
    }
	
	this->_fds = new struct pollfd[MAX_CONN * this->_servers.size()];
    this->pollfd_init();
	this->_nfds = this->_servers.size();
	this->run_servers();
}

ServerManager::~ServerManager()
{
	for (size_t i = 0; i < this->get_servers().size(); i++)
		this->get_server_at(i).clean_fd();
	delete [] this->_fds;
}

void ServerManager::pollfd_init()
{
	for (size_t i = 0; i < this->_configs.size(); i++)
    {
        this->_fds[i].fd = this->_servers[i].get_sockfd();
        this->_fds[i].events = POLLIN;
    }
}

int ServerManager::run_servers()
{
	// This whole thing probably has memory leaks
	bool	close_connection = false;
	bool	compress_array = false;
	while (SWITCH)
    {
        this->_nbr_fd_ready = poll(this->_fds, this->_nfds, -1);
        if (this->_nbr_fd_ready == -1)
        {
            // TODO avoid killing the server, implement test how to deal with it 
			perror("poll");
            return 1;
        }
		int	current_size = this->_nfds;
		for (int i = 0; i < current_size; i++)
		{
			if (this->_fds[i].revents == 0)
				continue ;
			if (this->_fds[i].revents != POLLIN) {
				std::cout << RED << "[ UNEXPECTED REVENTS VALUE ]" << RESET << std::endl;
				return (1);
			}
			if (this->_fds[i].fd == this->_servers[i].get_sockfd())
			{
				int	connection_fd;
				connection_fd = accept(this->_servers[i].get_sockfd(), NULL, NULL);
				if (connection_fd < 0)
				{
                     // TODO avoid killing the server, implement test how to deal with it (This fix might work, need to test it out)
					perror("accept");
					continue ;
                }
				this->_fds[this->_nfds].fd = connection_fd;
				this->_fds[this->_nfds].events = POLLIN;
				this->_nfds++;
			}
			else
			{
				close_connection = false;
				bool	file_read = false;
				while (true)
				{
					//TODO implement client max body size
					std::map<int, int>::iterator it = this->_map_server_fd.find(i);
					char	buffer[this->_servers[it->second].get_config().get_client_max_body_size()];
					int		received;

					// recv shows an error while tring to acces favicon
					std::cout << this->_fds[i].fd << std::endl;
					if (file_read == false)
					{

						received = recv(this->_fds[i].fd, buffer, sizeof(buffer), MSG_CTRUNC);
						if (received > this->_servers[it->second].get_config().get_client_max_body_size())
						{
							std::cout << "Client intended to send too large body." << std::endl;
							break ;
						}
						if (received < 0)
						{
							// removed the errno if statement, needs to be tested
							close_connection = true;
							perror("recv");
							break ;
						}
						if (received == 0)
						{
							printf("Connection closed\n");
							close_connection = true;
							break ;
						}
					}
					else
					{
						close_connection = true;
						break ;
					}
					/* [ SEND_RESPONSE ] */
					file_read = true;
					httpHeader request(buffer);
					request.printHeader();
					memset(buffer, 0, this->_servers[it->second].get_config().get_client_max_body_size());
					//this->_servers[it->second].send_response(this->_fds[i].fd, request.getUri());
					
					Response obj(this->_fds[i].fd, this->_servers[it->second].get_sockfd(), \
						this->_servers[it->second].get_config(), request.getUri());

				}
				if (close_connection)
				{
					close(this->_fds[i].fd);
					this->_fds[i].fd = -1;
					compress_array = true;
				}
			}
		}
		if (compress_array)
		{
			compress_array = false;
			for (size_t i = 2; i < this->_nfds; i++)
			{
				if (this->_fds[i].fd == -1)
				{
					for (size_t j = this->_nfds - 1; j > i; j--)
					{
						if (this->_fds[j].fd != -1)
						{
							this->_fds[i].fd = this->_fds[j].fd;
							this->_fds[i].events = this->_fds[j].events;
							this->_fds[i].revents = this->_fds[j].revents;
							this->_fds[j].fd = -1;
							this->_fds[j].events = 0;
							this->_fds[j].revents = 0;	
							break ;
						}
					}
					i--;
					this->_nfds--;
				}
			}
		}
    }
	for (size_t i = 0; i < this->_nfds; i++)
	{
		if (this->_fds[i].fd >= 0)
		{
			close(this->_fds[i].fd);
			this->_fds[i].fd = -1;
		}
	}
	return EXIT_SUCCESS;
}

// int		ServerManager::check_request_respond()
// {
// 	for (size_t i = this->_servers.size(); i < this->_nfds; i++)
// 	{
// 		int connfd = this->_fds[i].fd;
// 		if (connfd == -1)
// 		{
// 			continue;
// 		}

// 		if (this->_fds[i].revents & (POLLIN | POLLERR))
// 		{
// 			int n;
// 			// 
// 				std::map<int, int>::iterator it = this->_map_server_fd.find(i);

// 			//
// 			char buff[this->_servers[it->second].get_config().get_client_max_body_size()];
// 			memset(buff, 0, 1024);
// 			n = read(connfd, buff, sizeof(buff));            
// 			std::cout << "read return: " << n << std::endl;
// 			if ( n < 0 )
// 			{
// 				if (errno != ECONNRESET) //TODO cannot use errno
// 				{
// 					std::cerr << "this is n - " << n << std::endl;
// 					perror("error while reading the fd");
// 				}
// 			}
// 			else if (n == 0)
// 			{
// 				printf("Connection closed\n");
// 				close(connfd);
// 				this->_fds[i].fd = -1;
// 			}
// 			else
// 			{
// 				httpHeader request(buff);
// 				request.printHeader();
// 				memset(buff, 0, 1024);
// 				/*
				
				
// 					TEST CGI
				
				
// 				*/
// 				this->_servers[it->second].send_response(connfd, request.getUri());
// 			}

// 			if (--this->_nbr_fd_ready <= 0)
// 			{
// 				break;
// 			}
// 			this->_map_server_fd.erase(it);
// 		}
// 		close(connfd);
// 		this->_fds[i].fd = -1;
// 	}
// 	return EXIT_SUCCESS;
// }

// int		ServerManager::check_connection()
// {
// 		size_t server_num;
//         for (server_num = 0; server_num < this->_servers.size(); server_num++)
//         {
//             if (this->_fds[server_num].revents & POLLIN)
//             {
//                 int connfd;
//                 struct sockaddr_in cli_addr;
//                 socklen_t cli_len = sizeof(cli_addr);
//                 if ((connfd = accept(this->_servers[server_num].get_sockfd(), (struct sockaddr *)&cli_addr, &cli_len)) < 0)
//                 {
//                     perror("accept");
//             		return 1;
//                 }

//                std::cout << GREEN << "Received new connection\n" << RESET << std::endl;

//                 //Add new connection to poll
// 				size_t connection;
//                 for (connection = this->_servers.size(); connection < MAX_CONN * this->_servers.size(); connection++)
//                 {
//                     if (this->_fds[connection].fd == -1)
//                     {
//                         this->_fds[connection].fd = connfd;
//                         this->_fds[connection].events = POLLIN;
//                         break;
//                     }
//                 }

//                 if (connection == MAX_CONN * this->_servers.size())
//                 {
//                     std::cerr << "Too many connections" << std::endl;
//                 }
//                 else
//                 {
//                     this->_nfds++;
//                 }
//                 if (--this->_nbr_fd_ready <= 0)
//                 {
//                     break ;
//                 }
// 				this->_map_server_fd.insert(std::make_pair(connection, server_num));
//             }
//         }
// 		return EXIT_SUCCESS;
// }


std::vector<Server>	ServerManager::get_servers()
{
	return this->_servers;
}

Server	ServerManager::get_server_at(int i)
{
	return this->_servers[i];
}