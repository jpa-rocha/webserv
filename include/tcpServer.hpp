#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <stdio>
#include <iostream>
#include <sstream>


namespace ft
{
	class tcpServer
	{
		private:

			std::string			_ipAddr;
			int					_port;
			int					_socketFD;
			int 				_newSocket;
			int 				_incomingMsg;
			struct sockaddr_in 	_serverAddr;
			int					_serverAddrLen;
			std::string			_serverMsg;
			

		public:

			tcpServer(/* args */){};
			tcpServer(std::string IPaddr, int port);
			~tcpServer();

			void		startListen();

		private : 

			int			startServer();
			void		acceptConnection(int &newSocket);
			void		sendResponse();
			std::string	buildResponse():
			void		closeServer();
	};

}


#endif