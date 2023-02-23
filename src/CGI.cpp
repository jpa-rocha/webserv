#include "../include/CGI.hpp"

CGI::CGI()
{

}

CGI::~CGI()
{

}

std::string									CGI::get_root()
{
	return this->_root;
}

std::map<std::string, std::string>			CGI::get_path()
{
	return this->_path;
}

std::vector<std::string>					CGI::get_ext()
{
	return this->_ext;
}

void										CGI::set_root(std::string root)
{
	this->_root = root;
}

void										CGI::set_path(std::map<std::string, std::string> path)
{
	this->_path = path;
}

void										CGI::set_ext(std::vector<std::string> ext)
{
	this->_ext = ext;
}

std::ostream &operator<<(std::ostream &os, CGI &cgi)
{
	os << cgi.get_root() << std::endl;
	os << cgi.get_path() << std::endl;
	os << cgi.get_ext() << std::endl;
	return os;
}
