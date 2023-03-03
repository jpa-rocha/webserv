#include "../include/Location.hpp"

Location::Location()
{
	this->_root = "";
	this->_index = "";
	this->_autoindex = false;
	// needs to be included later
	// TODO chage to get on merge
	this->_methods.insert(std::make_pair(0, false));
	this->_methods.insert(std::make_pair(1, false));
	this->_methods.insert(std::make_pair(2, false));
	this->_methods.insert(std::make_pair(3, false));
	this->_methods.insert(std::make_pair(4, false));
	this->_methods.insert(std::make_pair(5, false));
	this->_methods.insert(std::make_pair(6, false));
	this->_methods.insert(std::make_pair(7, false));
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

std::map<short, bool>							Location::get_methods()
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

bool										Location::check_method_at(short method)
{
	if (method > 7 || method < 0)
		return false;
	else
	 	return this->_methods.at(method);
}

// Setters

void										Location::set_root(std::string	root)
{
	if (root[root.size() - 1] != '/')
		root = root + "/";
	this->_root = root;
}

void										Location::set_autoindex(bool autoindex)
{
	this->_autoindex = autoindex;
}

void										Location::set_methods(short method, bool allowed)
{
	this->_methods.at(method) = allowed;
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

int								Location::check_location()
{
	// root check
	if (this->get_root().size() == 0) {
		// TODO print error
		return 25;
	}
	if (dir_exists(this->get_root()) == false) {
		return 26;
	}

	// index check
	if (this->get_index().size() == 0) {
		// TODO print error
		return 27;
	}
	if (file_exists(this->get_root() + this->get_index()) == false) {
		// TODO print error
		return 28;
	}


	// methods check
	std::map<short, bool> methods = this->get_methods();
	int method_check = 0;
	for (std::map<short, bool>::iterator it = methods.begin(); it != methods.end(); it++) {
		if (it->second == true)
			method_check = 1;
			// TODO print error
	}
	if (method_check == 0)
		return 29;
	return EXIT_SUCCESS;
}

std::ostream &operator<<(std::ostream &os, Location &location)
{
	os << location.get_root() << std::endl;
	os << location.get_autoindex() << std::endl;
	os << location.check_method_at(0) << std::endl;
	os << location.check_method_at(1) << std::endl;
	os << location.check_method_at(2) << std::endl;
	os << location.check_method_at(3) << std::endl;
	os << location.check_method_at(4) << std::endl;
	os << location.check_method_at(5) << std::endl;
	os << location.check_method_at(6) << std::endl;
	os << location.check_method_at(7) << std::endl;
	os << location.get_index() << std::endl;
	os << location.get_redirection() << std::endl;
	os << location.get_alias() << std::endl;
	return os;
}
