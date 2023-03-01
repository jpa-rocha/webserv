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

void send_response(int client_socket, const std::string& path, Config &config)
{
    std::string			response_body;
    std::string			respond_path;
	std::string			response;
	std::ostringstream	response_stream;

	std::string root = config.get_root();
    if (path == "/favicon.ico" || path == "/")
        respond_path = config.get_index();
    else
        respond_path = path;
    
    respond_path = root + respond_path;
    std::ifstream file(respond_path.c_str());
    if (!file.is_open())
    {
        // if the file cannot be opened, send a 404 error
        std::ifstream error404((root + config.get_error_path(404)).c_str());
        if (!error404.is_open())
            std::cerr << RED << _404_ERROR << RESET << std::endl;
        else
        {
            std::stringstream	file_buffer;
            file_buffer << error404.rdbuf();
            response_body = file_buffer.str();
            response_stream << "HTTP/1.1 404 Not Found\r\n\r\n";
            error404.close();
        }
    }
    else
    {
        std::stringstream	file_buffer;
        file_buffer << file.rdbuf();
        response_body = file_buffer.str();
		// Generate the HTTP response headers
    	response_stream << "HTTP/1.1 200 OK\r\n";	
        // Add the content to the response body
    }
    response_stream << response_body;
	// Send the response to the client
	response = response_stream.str();
    //std::cerr << RED << response_body << RESET <<std::endl;
    if ( send(client_socket, response.c_str(), response.length(), 0) < 0  )
        std::cerr << RED << _RES_ERROR << RESET << std::endl;

    // Close the file
    file.close();
	close(client_socket);
    client_socket = -1;
}


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

                printf("Received new connection\n");

                //Add new connection to poll
                int j;
                for (j = 1; j < MAX_CONN * config.get_n_servers(); j++)
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
                    continue;
                }
            }
        }
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
                
                if ( (n = read(connfd, servers[i].get_buf(), sizeof(servers[i].get_buf()))) < 0 )
                {
                    if (errno != ECONNRESET)
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
                    httpHeader request(servers[i].get_buf());
                    request.printHeader();
					//printf("%s", buf);
					memset(servers[i].get_buf(), 0, 1024);
					//std::cout << GREEN << request.getUri() << RESET << std::endl;
					send_response(connfd, request.getUri(), config.get_config(0));
                }

                if (--nready <= 0)
                {
                    break;
                }
            }
            fds[i].fd = -1;
        }
    }

    return 0;
}