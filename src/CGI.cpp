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
	/* std::cout << "here" << std::endl;
	std::cout << this->_root << std::endl;
	std::cout << this->_path.at("python3") << std::endl;
	std::cout << this->_ext.at(0) << std::endl;
	std::cout << obj._ext.at(0) << std::endl; */
	return *this;
}


std::string									CGI::get_root() const
{
	return this->_root;
}

std::map<std::string, std::string>			CGI::get_path() const
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

void										CGI::set_path(std::string program, std::string path)
{
	this->_path.insert(std::make_pair(program, path));
}

void										CGI::set_ext(std::string ext)
{
	this->_ext.push_back(ext);
}

std::ostream &operator<<(std::ostream &os, const CGI &cgi)
{
	//os << cgi.get_root() << std::endl;
	// std::map<std::string, std::string>::iterator it;
	// std::map<std::string, std::string>::iterator end;
	// it = cgi.get_path().begin();
	// end = cgi.get_path().end();
	// for (; it != end; ++it)
	// {
	// 	os << "printing" << std::endl;
	// 	os << it->first << std::endl;
	// 	os << it->second << std::endl;
	// }
	os << cgi.get_root() << std::endl;
	os << cgi.get_path().at("python3") << std::endl;
	os << cgi.get_path().at("bash") << std::endl;
	os << cgi.get_ext().at(0) << std::endl;
	os << cgi.get_ext().at(1) << std::endl;
	// for (std::vector<std::string>::iterator it = cgi.get_ext().begin(); it != cgi.get_ext().end(); ++it)
	// {
	// 	os << (*it).data() << std::endl;
	// }
	return os;
}
