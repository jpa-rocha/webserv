#include "../include/Config.hpp"


Config::Config()
{
	this->set_port(8001);
	this->set_host(inet_addr("127.0.0.1"));
	this->set_server_name("test");
	this->set_default_error(404, "/error/404.html");
	this->set_client_max_body_size(1024);
	this->set_autoindex(false);
	this->set_root("./root");
	this->set_index("./root/index.html");

}

// Getters

u_int16_t					Config::get_port()
{
	return this->_port;
}

in_addr_t					Config::get_host()
{
	return this->_host;
}

std::string					Config::get_server_name()
{
	return this->_server_name;
}

std::map<int, std::string>	Config::get_default_error()
{
	return this->_default_error;
}

int 						Config::get_client_max_body_size()
{
	return this->_client_max_body_size;
}

bool						Config::get_autoindex()
{
	return this->_autoindex;
}

std::string					Config::get_root()
{
	return this->_root;
}

std::string					Config::get_index()
{
	return this->_index;
}

std::vector<Location>		Config::get_location()
{
	return this->_location;
}

// Setters

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

void					Config::set_location(std::vector<Location> location)
{
	this->_location = location;
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
	return (os);
/* os << this-> << std::endl;
os << this-> << std::endl;
	os << this-> << std::endl; */
	
}