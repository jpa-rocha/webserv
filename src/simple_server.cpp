#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <sstream>
#include "../include/Utils.hpp"
#include "../include/httpHeader.hpp"
#include "../include/ConfigParser.hpp"
#include "../include/Server.hpp"


int main(int argc, char** argv)
{
    if (argc > 2) {
		std::cerr << RED << TOO_MANY_ARGS << RESET << std::endl;
		return EXIT_FAILURE;
	}
	ConfigParser config;

	if (argc == 2) {
		config = ConfigParser(argv[1]);
	}
	else {
	 	if (config.get_error_code() != 0)
	 		return EXIT_FAILURE;
		Config configs = config.get_config(0);
	}

    std::vector<Server> servers;
    for (int i = 0; i < config.get_n_servers(); i++)
    {
        Server server = Server(config.get_config(i));
        if (server.getError() != 0)
            continue ;
        servers.push_back(server);
    }

    //initialize poll
    struct pollfd fds[MAX_CONN * config.get_n_servers()];
    for (int i = 0; i < config.get_n_servers(); i++)
    {
        fds[i].fd = servers[i].get_sockfd();
        fds[i].events = POLLIN;
    }

    for (int i = config.get_n_servers(); i < MAX_CONN * config.get_n_servers(); i++)
    {
        fds[i].fd = -1;
    }

    int nfds = servers.size();

    while (true)
    {
        int nready = poll(fds, nfds, -1);
        if (nready == -1)
        {
            perror("poll");
            return 1;
        }
        //Check for new connection
        for (int i = 0; i < config.get_n_servers(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                int connfd;
                struct sockaddr_in cli_addr;
                socklen_t cli_len = sizeof(cli_addr);
                if ((connfd = accept(servers[i].get_sockfd(), (struct sockaddr *)&cli_addr, &cli_len)) < 0)
                {
                    perror("accept");
                    return 1;
                }

               std::cout << GREEN << "Received new connection\n" << RESET << std::endl;

                //Add new connection to poll
                int j;
                for (j = config.get_n_servers(); j < MAX_CONN * config.get_n_servers(); j++)
                {
                    if (fds[j].fd == -1)
                    {
                        fds[j].fd = connfd;
                        fds[j].events = POLLIN;
                        break;
                    }
                }

                if (j == MAX_CONN * config.get_n_servers())
                {
                    std::cerr << "Too many connections" << std::endl;
                }
                else
                {
                    nfds++;
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
        for (int i = config.get_n_servers(); i < nfds; i++)
        {
            int connfd = fds[i].fd;
            if (connfd == -1)
            {
                continue;
            }

            if (fds[i].revents & (POLLIN | POLLERR))
            {
		        int n;
                char buff[servers[0].get_config().get_client_max_body_size()];
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
                    //close(connfd);
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
					servers[0].send_response(connfd, request.getUri());
                }

                if (--nready <= 0)
                {
                    break;
                }
            }
        }
    }

    return 0;
}