#include <stdlib.h>
#include "../include/ConfigParser.hpp"

int main (int argc, char** argv)
{
	(void) argv;
	if (argc > 2) {
		// too many arguments
	}
	else if (argc == 2) {
		// error check and config parse
		
	}
	else {
		ConfigParser config;
		if (config.get_error_code() != 0)
			return EXIT_FAILURE;
		Config configs = config.get_config(0);
		//for (int i = 0; i < config.get_n_servers(); i++)
		//	OurServer(config.get_config(i));
		// run default config file	
		std::cout << GREEN << config.get_config(0) << RESET << std::endl;
	}
	
	return EXIT_SUCCESS;
}