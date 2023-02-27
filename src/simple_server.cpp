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


const int MAX_CONN = 5;


void send_response(int client_socket, const std::string& path)
{
    std::string			response_body;
    std::string			respond_path;
	std::string			response;
	std::ostringstream	response_stream;

    if (path == "/favicon.ico" || path == "/")
        respond_path = "/index.html";
    else
        respond_path = path;
    
    respond_path = "docs/www" + respond_path;

    std::ifstream file(respond_path.c_str());
    if (!file.is_open())
    {
        // if the file cannot be opened, send a 404 error

        std::ifstream error404("docs/www/errors/404_NotFound.html");
        if (!error404.is_open())
            perror("error opening 404 file\n");
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
	{
		perror("error while sending the response");
	}

    // Close the file
    file.close();
	close(client_socket);
    client_socket = -1;
}


int main(int argc, char** argv)
{
(void) argv;
    if (argc > 2) {
		// too many arguments
	}
	ConfigParser config;

	// if (argc == 2) {
	// 	// error check and config parse
	// }
	// else {
	// 	if (config.get_error_code() != 0)
	// 		return EXIT_FAILURE;
	// 	if (config.get_error_code() != 0)
	// 		return EXIT_FAILURE;
	// 	Config configs = config.get_config(0);
	// 	//for (int i = 0; i < config.get_n_servers(); i++)
	// 	//	OurServer(config.get_config(i));
	// 	// run default config file	
	std::cout << GREEN << config.get_config(0) << RESET << std::endl;
    config.get_config(0).get_cgi().get_root()
    return (0);
	//}
	

	//return EXIT_SUCCESS;
    int sockfd[config.get_n_servers()];
	//int port = config.get_config(0).get_port()
    int port[config.get_n_servers()];
    char buf[1024];

    //Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }

    //Set socket options
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt");
        return 1;
    }

    struct sockaddr_in serv_addr[config.get_n_servers()];
    //Bind socket to port
    for (int i = 0; i < config.get_n_servers(); i++)
    {
        serv_addr[i].sin_family = AF_INET;
        serv_addr[i].sin_port = htons(port[i]);
        serv_addr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(sockfd[i], (struct sockaddr *)&serv_addr[i], sizeof(serv_addr[i])) < 0)
        {
            perror("bind");
            return 1;
        }
    }

    //Listen on socket
    if (listen(sockfd, MAX_CONN) < 0)
    {
        perror("listen");
        return 1;
    }

    //initialize poll
    struct pollfd fds[config.get_n_servers()][MAX_CONN];

    for (int j = 0; j < config.get_n_servers(); j++)
    {
        fds[j][0].fd = sockfd[j];
        fds[j][0].events = POLLIN;
        for (int i = 1; i < MAX_CONN; i++)
        {
            fds[j][i].fd = -1;
        }
    }

    int nfds = 1;

    while (true)
    {
        int nready = poll(fds, nfds, -1);
        if (nready == -1)
        {
            perror("poll");
            return 1;
        }

        //Check for new connection
        if (fds[0].revents & POLLIN)
        {
            int connfd;
            struct sockaddr_in cli_addr;
            socklen_t cli_len = sizeof(cli_addr);
            if ((connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len)) < 0)
            {
                perror("accept");
                return 1;
            }

            printf("Received new connection\n");

            //Add new connection to poll
            int i;
            for (i = 1; i < MAX_CONN; i++)
            {
                if (fds[i].fd == -1)
                {
                    fds[i].fd = connfd;
                    fds[i].events = POLLIN;
                    break;
                }
            }

            if (i == MAX_CONN)
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

        //Check for data on all connections
        for (int i = 1; i < nfds; i++)
        {
            int connfd = fds[i].fd;
            if (connfd == -1)
            {
                continue;
            }

            if (fds[i].revents & (POLLIN | POLLERR))
            {
                int n;
                
                if ( (n = read(connfd, buf, sizeof(buf))) < 0 )
                {
                    if (errno != ECONNRESET)
                    {
						std::cout << "this is n - " << n << std::endl;
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
                    httpHeader request(buf);
                    request.printHeader();
					//printf("%s", buf);
					memset(buf, 0, 1024);
					//std::cout << GREEN << request.getUri() << RESET << std::endl;
					send_response(connfd, request.getUri());
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
