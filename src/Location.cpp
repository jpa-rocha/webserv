#include "../include/Location.hpp"

Location::Location()
{
	this->_root = "";
	this->_index = "";
	this->_autoindex = false;
	// needs to be included later
	this->_methods.push_back(0);
	this->_alias = "";
	this->_redirection = "";
}

std::string									Location::get_root()
{
	return this->_root;
}

bool										Location::get_autoindex()
{
	return this->_autoindex;
}

std::vector<short>							Location::get_methods()
{
	return this->_methods;
}

std::string									Location::get_index()
{
	return this->_index;
}

std::string									Location::get_redirection()
{
	return this->_redirection;
}

std::string									Location::get_alias()
{
	return this->_alias;
}

// Setters

void										Location::set_root(std::string	root)
{
	this->_root = root;
}

void										Location::set_autoindex(bool autoindex)
{
	this->_autoindex = autoindex;
}

void										Location::set_methods(std::vector<short> methods)
{
	this->_methods = methods;
}

void										Location::set_index(std::string	index)
{
	this->_index = index;
}

void										Location::set_redirection(std::string redirection)
{
	this->_redirection = redirection;
}

void										Location::set_alias(std::string	alias)
{
	this->_alias = alias;
}

std::ostream &operator<<(std::ostream &os, Location &location)
{
	os << location.get_root() << std::endl;
	os << location.get_autoindex() << std::endl;
	//os << location.get_methods() << std::endl;
	os << location.get_index() << std::endl;
	os << location.get_redirection() << std::endl;
	os << location.get_alias() << std::endl;
	return os;
}
