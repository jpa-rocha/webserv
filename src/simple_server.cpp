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

#include "../include/httpHeader.hpp"

#include <sstream>

const int MAX_CONN = 5;


void send_response(int client_socket, const std::string& content)
{
    // Generate the HTTP response headers
    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 200 OK\r\n";
    response_stream << "Content-Length: " << content.length() << "\r\n";
    response_stream << "Content-Type: text/plain\r\n";
    response_stream << "Connection: close\r\n";
    response_stream << "\r\n";

    // Add the content to the response body
    response_stream << content;

    // Send the response to the client
    std::string response = response_stream.str();
    send(client_socket, response.c_str(), response.length(), 0);

    // Close the connection
    close(client_socket);
}


int main()
{
    int sockfd;
    int port = 8081;
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

    //Bind socket to port
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    //Listen on socket
    if (listen(sockfd, MAX_CONN) < 0)
    {
        perror("listen");
        return 1;
    }

    //initialize poll
    struct pollfd fds[MAX_CONN];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    for (int i = 1; i < MAX_CONN; i++)
    {
        fds[i].fd = -1;
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
                
                if ((n = read(connfd, buf, sizeof(buf))) < 0)
                {
                    if (errno != ECONNRESET)
                    {
                        perror("read");
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
                    httpHeader request(buf);
                    request.printHeader();
					printf("%s", buf);
					memset(buf, 0, 1024);
					send_response(connfd, "Hello");
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
