#include "./tcpServer.hpp"

using namespace ft;

int main()
{

	tcpServer	myServer = tcpServer("0.0.0.0", 8080);
	myServer.startListen();

	return (0);

}