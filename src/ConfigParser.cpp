#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	std::ifstream in_file;
	
	Config test;
	//std::cout << test << std::endl;
	this->_config.push_back(test);
	this->_n_servers = 0;
	// Check if config file exists
	in_file.open("./webserv.config", std::ios::in);
	if (in_file.fail() == true)
		this->exit_with_error(1, in_file);
	
	// Check if server context exists
//	std::cout << line << std::endl;
	if (this->check_server_context(in_file) == false)
		this->exit_with_error(this->get_error_code(), in_file);
}

Config ConfigParser::get_config()
{
	return this->_config[0];
}

int ConfigParser::get_error_code()
{
	return this->_error_code;
}

bool ConfigParser::check_server_context(std::ifstream& config_file)
{
	std::string line;
	int context = 0;
	while (getline(config_file, line)) {
		line = this->remove_comments(line);
		size_t pos = line.find_first_not_of(" \r\t\b\f");
		if (pos == std::string::npos)
			continue;
		
		// TODO server should be the top element
		if ((line.find("server") != std::string::npos && line.find("{") != std::string::npos) && line.find("}") != std::string::npos)
			return false;
		else if ((line.find("server") != std::string::npos && line.find("{") != std::string::npos) && context == 0) {
			this->_n_servers += 1;
			context += 1;
		}
		else if (context == 0)
			return false;
		if (context && line.find(LISTEN) != std::string::npos)
			this->clean_listen(line);
		else if (context && line.find(HOST) != std::string::npos)
			this->clean_host(line);
		else if (context && line.find(ERROR_PAGE) != std::string::npos)
			this->clean_error_page(line);
		else if (context && line.find(SERVER_NAME) != std::string::npos)
			this->clean_server_name(line);
		else if (context && line.find(CLIENT_MAX_BODY) != std::string::npos)
			this->clean_client_max_body_size(line);
		else if (context && line.find(AUTOINDEX) != std::string::npos)
			this->clean_autoindex(line);
		//std::cout << GREEN << this->get_config().get_host() << RESET << std::endl;

		/*
		
		go into function and separate location config
		keep track of context
		
		*/
		if (line.find("location") != std::string::npos && line.find("{") != std::string::npos)
			context += 1;
		if (line.find("}") != std::string::npos && context > 0)
			context -= 1;
		
		/*
		
		check line for values for Config
		
		*/	
	}
	if (context == 0)
		return true;
	return false;
}


std::string ConfigParser::remove_comments(std::string line)
{
	line = this->remove_end(line, '#');
	return line;
}

std::string ConfigParser::remove_end(std::string line, char symbol)
{
	if (line.find(symbol) != std::string::npos) {
		line.erase(line.find(symbol));
	}
	return line;
}

std::string ConfigParser::find_int(std::string line)
{
	std::size_t pos = line.find_first_of("1234567890");
	line.erase(0, pos);
	if (pos == std::string::npos)
		return line;
	line = this->remove_end(line, ';');
	return line;
}

int ConfigParser::clean_listen(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		return 3;
	this->get_config().set_port(this->toInt(line.c_str()));

	return EXIT_SUCCESS;
}

int ConfigParser::clean_host(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		return 4;
	this->get_config().set_host(inet_addr(line.c_str()));

	return EXIT_SUCCESS;
}

int ConfigParser::clean_error_page(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		return 5;
	std::cout << PURPLE << line << RESET << std::endl;
	std::size_t pos = line.find_first_of("/");
	std::string path = &line[pos];
	pos = line.find_first_of(" ");
	line.erase(pos);
	this->get_config().set_default_error(this->toInt(line), path);

	return EXIT_SUCCESS;
}

int ConfigParser::clean_server_name(std::string line)
{
	line = this->remove_end(line, ';');	
	size_t pos = line.find_first_not_of(" \r\t\b\f");
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	line.erase(0, pos);
	pos = line.find_first_of(" \r\t\b\f");
	if (pos != std::string::npos)
		line.erase(pos);
	std::cout << line << std::endl;
	this->get_config().set_server_name(line);
	return EXIT_SUCCESS;
}

int ConfigParser::clean_client_max_body_size(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		return 6;
	this->get_config().set_client_max_body_size(this->toInt(line.c_str()));

	return EXIT_SUCCESS;
}

int ConfigParser::clean_autoindex(std::string line)
{
	if (line.find("on") != std::string::npos)
		this->get_config().set_autoindex(true);
	else if (line.find("off") != std::string::npos)
		this->get_config().set_autoindex(false);
	else	
		return 7;
	return EXIT_SUCCESS;
}

int ConfigParser::clean_root(std::string line)
{
	line = this->remove_end(line, ';');	
	size_t pos = line.find_first_not_of(" \r\t\b\f");
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	line.erase(0, pos);
	pos = line.find_first_of(" \r\t\b\f");
	if (pos != std::string::npos)
		line.erase(pos);
	this->get_config().set_root(line);
	return EXIT_SUCCESS;
}

int ConfigParser::clean_index(std::string line)
{
	line = this->remove_end(line, ';');	
	size_t pos = line.find_first_not_of(" \r\t\b\f");
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	line.erase(0, pos);
	pos = line.find_first_of(" \r\t\b\f");
	if (pos != std::string::npos)
		line.erase(pos);
	this->get_config().set_index(line);
	return EXIT_SUCCESS;
}

std::string ConfigParser::get_value()

int    ConfigParser::toInt(std::string str)
{
    int    x;
    std::istringstream ss(str);
    ss >> x;
    return x;
}


int ConfigParser::exit_with_error(int err_code, std::ifstream& in_file)
{
	if (err_code == 1) {
		std::cout << RED << "--- Could not find default configuration file at: ./webserver.config ---" << RESET << std::endl;
		in_file.close();
		exit(1);
	}
	else if (err_code == 2) {
		std::cout << RED << "--- Could not find a valid ** server {} ** context";
		std::cout <<		" in the provided configuration file ---" << RESET << std::endl;
		in_file.close();
		exit(2);
	}
	return EXIT_SUCCESS;
}