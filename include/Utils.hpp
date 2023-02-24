#ifndef UTILS_HPP
# define UTILS_HPP

# define RESET   "\033[0m"
# define BLACK   "\033[30m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define PURPLE "\033[35m"
# define CYAN    "\033[36m"
# define WHITE   "\033[37m"

#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include "httpHeader.hpp"
#include <fstream>
#include <sys/socket.h>


enum httpMethods 
{
	GET,
	POST,
	DELETE,
	PUT,
	HEAD,
	OPTIONS,
	TRACE,
	CONNECT,
	NONE
};

#endif