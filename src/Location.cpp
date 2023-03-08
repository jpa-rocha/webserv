#include "../include/Location.hpp"

Location::Location()
{
	this->_root = "";
	this->_index = "";
	this->_autoindex = false;
	// needs to be included later
	// TODO chage to get on merge
	this->init_methods();
	this->_alias = "";
	this->_redirection = "";
}

Location::Location(std::ifstream &config_file, std::string line)
{
	int	exit_context = 0;

	this->init_methods();
	while (exit_context == 0 && getline(config_file, line)) {
		line = remove_comments(line);
		if (line.find("}") != std::string::npos)
			exit_context = 1;
		if ((line.find(ROOT) != std::string::npos) && check_def_format(ROOT, line))
			this->set_root(get_value(line));
		if ((line.find(AUTOINDEX) != std::string::npos) && check_def_format(AUTOINDEX, line))
			this->set_autoindex(clean_loc_autoindex(line));
		if ((line.find(ALLOW_METHODS) != std::string::npos) && check_def_format(ALLOW_METHODS, line))
			this->clean_methods(line);
		if ((line.find(INDEX) != std::string::npos) && check_def_format(INDEX, line))
			this->set_index(get_value(line));
		if ((line.find(ALIAS) != std::string::npos)  && check_def_format(ALIAS, line))
			this->set_alias(get_value(line));
		if ((line.find(RETURN) != std::string::npos)  && check_def_format(RETURN, line))
			this->set_redirection(get_value(line));
	}	
	if (exit_context != 1 || (this->get_index().empty() || this->get_root().empty()))
		set_error_code(11);
	this->set_error_code(0);
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

int Location::get_error_code()
{
	return this->_error_code;
}

void Location::set_error_code(int error_code)
{
	this->_error_code = error_code;
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
	if (this->get_root().size() == 0 || dir_exists(this->get_root()) == false) {
		return 23;
	}
	// index check
	if (this->get_index().size() == 0 || file_exists(this->get_root() + this->get_index()) == false) {
		return 24;
	}

	if (this->get_redirection().size() > 0 && dir_exists(this->get_redirection()) == false) {
		return 25;
	}

	// methods check
	std::map<short, bool> methods = this->get_methods();
	int method_check = 0;
	for (std::map<short, bool>::iterator it = methods.begin(); it != methods.end(); it++) {
		if (it->second == true)
			method_check = 1;
	}
	if (method_check == 0)
		return 26;
	return EXIT_SUCCESS;
}

bool 								Location::clean_loc_autoindex(std::string line)
{
	if (line.find("on") != std::string::npos)
		return true;
	return false;
}

void				Location::clean_methods(std::string line)
{
	line = remove_end(line, ';');
	size_t pos = 0;
	size_t pos2 = 0;
	line = remove_end(line, ';');
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	while (pos != std::string::npos) {
		pos = line.find_first_not_of(" \r\t\b\f", pos);
		pos2 = line.find_first_of(" \r\t\b\f", pos);
		if  (get_method_num(line.substr(pos, pos2 - pos)) < 0)
			this->set_error_code(13);
		this->set_methods(get_method_num(line.substr(pos, pos2 - pos)), 1);
		pos = pos2;
	}
	if (this->get_methods().empty())
		this->set_error_code(13);
}

void							Location::init_methods()
{
	this->_methods.insert(std::make_pair(0, false));
	this->_methods.insert(std::make_pair(1, false));
	this->_methods.insert(std::make_pair(2, false));
	this->_methods.insert(std::make_pair(3, false));
	this->_methods.insert(std::make_pair(4, false));
	this->_methods.insert(std::make_pair(5, false));
	this->_methods.insert(std::make_pair(6, false));
	this->_methods.insert(std::make_pair(7, false));
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
