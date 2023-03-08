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
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include "../include/Utils.hpp"
#include "../include/httpHeader.hpp"
#include "../include/ConfigParser.hpp"
#include "../include/ServerManager.hpp"
#include "../include/minilib.hpp"

//TODO lable headers

int main(int argc, char** argv)
{
    if (argc > 2) {
		std::cerr << RED << TOO_MANY_ARGS << RESET << std::endl;
		return EXIT_FAILURE;
	}
	ConfigParser configs;

	if (argc == 2) {
		// TODO assignment operator
		configs = ConfigParser(argv[1]);
	}
	if (configs.get_error_code() != 0)
		return EXIT_FAILURE;
	
	//TODO check for error files paths
	/*
		test
	*/
	
		if (configs.get_config(0).check_config() != 0) {
			std::cout << YELLOW << configs.get_config(0).check_config() << RESET << std::endl;
			return EXIT_FAILURE;
		}

	/*
		end
	*/
	signal(SIGINT, signal_callback_handler);
	//signal(SIGQUIT, signal_callback_handler);
	ServerManager manager(configs.get_configs());

    return EXIT_SUCCESS;
}