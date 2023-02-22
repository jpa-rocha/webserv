
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/* 
	TCP/IP sockets
	- create the socket 
	- identify the socket
	- on the server, wait for an incoming connection
	- send & receive messages
	- close socket
 */
#define SERVER_PORT 8080

int	main(int argc, char **argv)
{

	int	server_fd;
	int	new_socket;
	long valread;
	/* structure describing internet socket address*/
	struct sockaddr_in serverAddress;
	int addrlen = sizeof(serverAddress);


	// creating a new socket, socket file descriptor :
	// this is just allocating resources, it is going to be an internet socket ( AF_INET ),
	// tcp stream socket ( SOCK_STREAM )
	/*  int socket(int __domain, int __type, int __protocol)
		Create a new socket of type TYPE in domain DOMAIN, using
		protocol PROTOCOL. If PROTOCOL is zero, one is chosen automatically.
		Returns a file descriptor for the new socket, or -1 for errors.
	*/
	/*  AF_INET -- address family that is used to designate the type of addresses 
	 		that the socket can communicate with (ex.IPV4)
	 	SOCK_STREAM -- shows that it is a tcp socket */
	
	if ( (server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Error in socket");
		exit(EXIT_FAILURE);
	}

	/*
		we need to set up the address, this is the address we are listening on
		because we are making server so the server, we are not connecting to other
		machines but accepting connections.
	*/
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // specifiying -- internet address any, responding to any add.
	serverAddress.sin_port = htons( SERVER_PORT ); // specifiying -- port the server is listening on

	bzero(&serverAddress, sizeof(serverAddress));
	//memset(address.sin_zero, '\0', sizeof address.sin_zero);

	/* 	
		now we are binding our listening socket we allocated before, to the address
		this basically tells the OS, this socket is going to listen to this address 
 	*/	
	/*  int bind(int __fd, const struct sockaddr *__addr, socklen_t __len)
		Give the socket FD the local address ADDR (which is LEN bytes long). 
	*/
	if ( bind(server_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0 )
	{
		perror("Error in bind");
		exit(EXIT_FAILURE);
	}

	/*
		int listen(int __fd, int __n)
		Prepare to accept connections on socket FD.
		N connection requests will be queued before further requests are refused.
		Returns 0 on success, -1 for errors.
	*/
	if ( listen(server_fd, 10) < 0 )
	{
		perror("Error in listen");
		exit(EXIT_FAILURE);
	}

	/*
		here we accepts connections & handles connections over & over again
		until we decide we no longer want that server
	*/
	/* 	char *msg = "Hello from server";
 	*/	char *msg = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

	while(1)
	{

		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		
		/*
			it says I wanna accept connections
		*/
		if ( (new_socket = accept(server_fd, (struct sockaddr *)&serverAddress, (socklen_t*)&addrlen)) < 0 )
        {
            perror("Error in accept");
            exit(EXIT_FAILURE);
        }

  		char buffer[30000] = {0};
		/*
			we are going to read from the socket
		*/
		valread = read( new_socket , buffer, 30000 );
        printf("%s\n",buffer );
        write(new_socket , msg , strlen(msg));
        printf("------------------ message sent -------------------\n");
        close(new_socket);

	}
	return 0;
}
