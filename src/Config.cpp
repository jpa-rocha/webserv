#include "../include/Config.hpp"


Config::Config(): _error_code(0)
{
	this->set_port(8000);
	this->set_host(inet_addr("127.0.0.1"));
	this->set_server_name("default");
	this->set_client_max_body_size(1024);
	this->set_autoindex(false);
	this->set_root("");
	this->set_index("");
}

Config::Config(const Config& obj)
{
	*this = obj;
}

Config &Config::operator=(const Config& obj)
{
	if (this != &obj) {
		this->_port = obj._port;
		this->_host = obj._host;
		this->_server_name = obj._server_name;
		this->_default_error = obj._default_error;
		this->_client_max_body_size = obj._client_max_body_size;
		this->_autoindex = obj._autoindex;
		this->_root = obj._root;
		this->_index = obj._index;
		this->_error_code = obj._error_code;
		this->_cgi = obj._cgi;
		this->_location = obj._location;
	}
	return *this;
}

Config::~Config()
{

}

// Getters

u_int16_t					&Config::get_port()
{
	return this->_port;
}

in_addr_t					&Config::get_host()
{
	return this->_host;
}

std::string					&Config::get_server_name()
{
	return this->_server_name;
}

std::map<int, std::string>	&Config::get_default_error()
{
	return this->_default_error;
}

std::string							&Config::get_error_path(int error)
{
	
	return this->get_default_error().at(error);
}

int 						&Config::get_client_max_body_size()
{
	return this->_client_max_body_size;
}

bool						&Config::get_autoindex()
{
	return this->_autoindex;
}

std::string					&Config::get_root()
{
	return this->_root;
}

std::string					&Config::get_index()
{
	return this->_index;
}

std::map<std::string, Location>		&Config::get_location()
{
	return this->_location;
}

CGI									&Config::get_cgi()
{
	return this->_cgi;
}

int 								Config::get_error_code()
{
	return this->_error_code;
}


// Setters

void 					Config::set_error_code(int error_code)
{
	this->_error_code = error_code;
}

void					Config::set_port(u_int16_t port)
{
	this->_port = port;
}

void					Config::set_host(in_addr_t host)
{
	this->_host = host;
}

void					Config::set_server_name(std::string server_name)
{
	this->_server_name = server_name;
}

void					Config::set_default_error(int i, std::string default_error)
{
	std::pair<int, std::string> p = std::make_pair(i, default_error);
	this->_default_error.insert(p);
}

void 					Config::set_client_max_body_size(int clien_max_body_size)
{
	this->_client_max_body_size = clien_max_body_size;
}

void					Config::set_autoindex(bool autoindex)
{
	this->_autoindex = autoindex;
}

void					Config::set_root(std::string root)
{
	this->_root = root;
}

void					Config::set_index(std::string index)
{
	this->_index = index;
}

void					Config::set_location(std::ifstream& config_file, std::string line)
{
	std::string key = get_value(line);
	Location location = Location(config_file, line);
	this->set_error_code(location.get_error_code());
	this->_location.insert(std::make_pair(key, location));
}

void					Config::set_cgi(std::ifstream& config_file, std::string line)
{
	this->_cgi = CGI(config_file, line);
}

//TODO is config valid check
int						Config::check_config()
{
	// root check
	if (this->get_root().size() == 0) {
		this->set_error_code(20);
		// TODO print error
		return 20;
	}
	if (dir_exists(this->get_root()) == false) {
		this->set_error_code(21);
		return 21;
	}

	// index check
	if (this->get_index().size() == 0) {
		this->set_error_code(22);
		// TODO print error
		return 22;
	}
	if (file_exists(this->get_root() + this->get_index()) == false) {
		this->set_error_code(23);
		// TODO print error
		return 23;
	}

	// default error check
	std::map<int, std::string> error = this->get_default_error();
	for (std::map<int, std::string>::iterator it = error.begin(); it != error.end(); it++) {
		if (file_exists(this->get_root() + it->second) == false) {
			this->set_error_code(24);
			// TODO print error
			return 24;
		}
	}

	// Location check
	std::map<std::string, Location> location = this->get_location();
	for (std::map<std::string, Location>::iterator it = location.begin(); it != location.end(); it++) {
		int error_code = it->second.check_location();
		if (error_code != EXIT_SUCCESS) {
			this->set_error_code(error_code);
			return error_code;
		}
	}
	
	// CGI check
	this->set_error_code(this->get_cgi().cgi_check());


	return this->get_error_code();
}

std::ostream& operator<<(std::ostream& os, Config& config)
{
	os << config.get_port() << std::endl;
	os << config.get_host() << std::endl;
	//os << config.get_default_error() << std::endl;
	os << config.get_server_name() << std::endl;
	os << config.get_client_max_body_size() << std::endl;
	os << config.get_root() << std::endl;
	os << config.get_index() << std::endl;
	//os << config.get_location().at("/") << std::endl;
	os << config.get_cgi() << std::endl;
	return (os);
/* os << this-> << std::endl;
os << this-> << std::endl;
	os << this-> << std::endl; */
	
}
