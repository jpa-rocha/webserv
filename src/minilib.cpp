#include "../include/minilib.hpp"
#include "../include/Utils.hpp"
#include <sstream>
#include <sys/types.h>
#include <dirent.h>

std::string readFile(std::string filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		return (NULL);
	std::stringstream file_buffer;
	file_buffer << file.rdbuf();

	return (file_buffer.str());
}

bool		dir_exists(std::string path)
{
	DIR* dir = opendir(path.c_str());
	if (dir) {
		closedir(dir);
		return true;
	}
	else {
		closedir(dir);
		return false;
	}
	
}

bool		file_exists(std::string file)
{
	std::ifstream in_file;
	
	in_file.open(file.c_str(), std::ios::in);
	if (in_file.fail() == true) {
		in_file.close();
		return false;
	}
	in_file.close();
	return true;
}