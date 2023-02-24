#include "../include/CGI.hpp"
#include <stdlib.h>

CGI::CGI()
{
	this->_root = "";
}

CGI& CGI::operator=(const CGI& obj)
{
	if (this != &obj) {
		this->_root = obj._root;
		this->_path = obj._path;
		this->_ext = obj._ext;
	}
	return *this;
}


std::string									CGI::get_root() const
{
	return this->_root;
}

std::map<std::string *, std::string *>			CGI::get_path() const
{
	return this->_path;
}

std::vector<std::string>					CGI::get_ext() const
{
	return this->_ext;
}

void										CGI::set_root(std::string root)
{
	this->_root = root;
}

void										CGI::set_path(std::string *program, std::string *path)
{
	this->_path.insert(std::map<std::string *, std::string *>::value_type(program, path));
}

void										CGI::set_ext(std::string ext)
{
	this->_ext.push_back(ext);
}

std::ostream &operator<<(std::ostream &os, const CGI &cgi)
{
	//os << cgi.get_root() << std::endl;
	for (std::map<std::string *, std::string *>::iterator it = cgi.get_path().begin(); it != cgi.get_path().end(); it++)
	{
		os << *(it->first) << std::endl;
		os << *(it->second) << std::endl;
	}
	// for (std::vector<std::string>::iterator it = cgi.get_ext().begin(); it != cgi.get_ext().end(); ++it)
	// {
	// 	os << (*it).data() << std::endl;
	// }
	std::cout << "success?" << std::endl;
	return os;
}
