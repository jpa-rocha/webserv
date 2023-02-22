
#include "../include/tcpServer.hpp"


namespace ft
{

	const int BUFFER_SIZE = 30000;
	
/* 	tcpServer::tcpServer(): _ipAddr(), _port(){}; */

	tcpServer::tcpServer(std::string IPaddr, int port):
		_ipAddr(IPaddr), _port(port), _socketFD(), _newSocket(),
		_incomingMsg(), _serverAddr(),_serverAddrLen(sizeof(_serverAddr)),
		_serverMsg( buildResponse() )
	{
		_serverAddr.sin_family = AF_INET;
		_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // inet_addr(_ipAddr.c_str());
		_serverAddr.sin_port = htons( _port );

		if ( startServer() != 0)
		{
			std::ostringstream	ss;
			ss << "Failed to start the server with PORT: " << ntohs(_serverAddr.sin_port);
			std::cout << ss.str() << std::endl;
		}
	};


	tcpServer::~tcpServer()
	{
		closeServer();
	};


	int tcpServer::startServer()
	{
 		_socketFD = socket(AF_INET, SOCK_STREAM, 0);

		if ( _socketFD < 0 )
		{
			std::cout << "Error in creating socket" << std::endl;
			exit(EXIT_FAILURE);
		}

		if ( bind(_socketFD, (struct sockaddr *)&serverAddr, sizeof(serverAddress)) < 0 )
		{
			std::cout << "Error in binding the socket to the address" << std::endl;
			exit(EXIT_FAILURE);
		}

		return EXIT_SUCCESS;
	}


	void tcpServer::startListen()
	{
		if ( listen(_socketFD, 10) < 0 )
		{
			std::cout << "Error in listen" << std::endl;
			exit(EXIT_FAILURE);
		}

		std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(_serverAddr.sin_addr) 
		<< " PORT: " << ntohs(_serverAddr.sin_port) 
		<< " ***\n\n";
       	std::cout << ss.str() << std::endl;

		int		bytesRead;

		while(1)
		{
			
			std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n" << std::endl;
	
			acceptConnection(_newSocket);

  			char buffer[BUFFER_SIZE] = {0};
		/*
			we are going to read from the socket
		*/
			bytesRead = read( _newSocket , buffer, BUFFER_SIZE );
			if ( bytesRead < 0 )
			{
				std::cout << "Error in reading bytes from client socket connection" << std::endl;
        	    exit(EXIT_FAILURE);
			}

			std::ostringstream ss;
        	ss << "------ Received Request from client ------\n\n";
        	std::cout << ss.str() << std::endl;
	
			sendResponse();
        	close(new_socket);
		}
	};

	void tcpServer::acceptConnection(int &newSocket)
	{
		new_socket = accept(_socketFD, (sockaddr *)&_serverAddr, (socklen_t*)&_serverAddrLen));
		if ( new_socket < 0 )
        {
			std::cout << "Error in accepting a connection" << std::endl;
            exit(EXIT_FAILURE);
        }
	};

	void tcpServer::sendResponse()
	{
		long	byteSent;

		byteSent = write(_newSocket, _serverMsg.c_str(), _serverMsg.size());
	
		if ( byteSent == _serverMsg.size() )
			std::cout << "Server response sent to client" << std::endl;
		else
			std::cout << "Error at server response" << std::endl;
	};

	std::string	tcpServer::buildResponse()
	{
		std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();

	};

	void tcpServer::closeServer()
	{
		close(_socketFD);
		close(_newSocket);
		exit(0);
	};

}

