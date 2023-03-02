#include "../include/minilib.hpp"
#include "../include/Utils.hpp"
#include <sstream>

std::string readFile(std::string filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		return (NULL);
	std::stringstream file_buffer;
	file_buffer << file.rdbuf();

	return (file_buffer.str());
}