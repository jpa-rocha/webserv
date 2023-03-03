#ifndef MINILIB_HPP
# define MINILIB_HPP

# include <iostream>

std::string readFile(std::string filename);
bool		dir_exists(std::string path);
bool		file_exists(std::string file);

#endif