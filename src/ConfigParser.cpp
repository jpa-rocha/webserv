#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	std::ifstream in_file;
	
	Config test;
	//std::cout << test << std::endl;
	this->_config.push_back(test);
	this->set_error_code(0);
	this->_n_servers = 0;
	// Check if config file exists
	in_file.open("./webserv.config", std::ios::in);
	if (in_file.fail() == true)
		this->exit_with_error(1, in_file);
	
	// Check if server context exists
//	std::cout << line << std::endl;
	if (this->check_server_context(in_file) == false)
		this->exit_with_error(this->get_error_code(), in_file);
	test = this->get_config();
	std::cout << GREEN << test << RESET << std::endl;
}

Config ConfigParser::get_config()
{
	return this->_config[0];
}

int ConfigParser::get_error_code()
{
	return this->_error_code;
}

void ConfigParser::set_error_code(int error_code)
{
	this->_error_code = error_code;
}

bool ConfigParser::check_server_context(std::ifstream& config_file)
{
	std::string line;
	int context = 0;
	while (getline(config_file, line) && this->get_error_code() == 0) {
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
		/*
		
		go into function and separate location config
		keep track of context
		
		*/
		if (line.find("location") != std::string::npos && line.find("{") != std::string::npos) {
			context += 1;
			if (line.find("cgi-bin") != std::string::npos)
				this->clean_cgi(config_file, line);
			else
				this->clean_location(config_file, line);
		}
		if (line.find("}") != std::string::npos && context > 0)
			context -= 1;
		
		/*
		
		check line for values for Config
		
		*/	
	}
	if (context == 0 && this->get_error_code() == 0)
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

void ConfigParser::clean_listen(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		this->set_error_code(3);
	this->get_config().set_port(this->to_int(line.c_str()));
}

void ConfigParser::clean_host(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		this->set_error_code(4);
	this->get_config().set_host(inet_addr(line.c_str()));
}

void ConfigParser::clean_error_page(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		this->set_error_code(5);
	std::size_t pos = line.find_first_of("/");
	std::string path = &line[pos];
	pos = line.find_first_of(" ");
	line.erase(pos);
	this->get_config().set_default_error(this->to_int(line), path);
}

void ConfigParser::clean_server_name(std::string line)
{
	line = this->get_value(line);
	if (line.size() == 0)
		this->set_error_code(6);
	this->get_config().set_server_name(line);
}

void ConfigParser::clean_client_max_body_size(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		this->set_error_code(7);
	this->get_config().set_client_max_body_size(this->to_int(line.c_str()));
}

void ConfigParser::clean_autoindex(std::string line)
{
	if (line.find("on") != std::string::npos)
		this->get_config().set_autoindex(true);
	else if (line.find("off") != std::string::npos)
		this->get_config().set_autoindex(false);
	else	
		this->set_error_code(8);
}

void ConfigParser::clean_root(std::string line)
{
	line = this->get_value(line);
	if (line.size() == 0)
		this->set_error_code(9);
	this->get_config().set_root(line);
}

void ConfigParser::clean_index(std::string line)
{
	line = this->get_value(line);
	if (line.size() == 0)
		this->set_error_code(10);
	this->get_config().set_index(line);
}

void ConfigParser::clean_location(std::ifstream& config_file, std::string line)
{
	Location location;
	std::string key = this->get_value(line);
	int	exit_context = 0;
	while (exit_context == 0 && getline(config_file, line)) {
		if (line.find("}") != std::string::npos)
			exit_context = 1;
		if (line.find(ROOT) != std::string::npos)
			location.set_root(this->get_value(line));
		if (line.find(AUTOINDEX) != std::string::npos)
			location.set_autoindex(this->clean_loc_autoindex(line));
		if (line.find(ALLOW_METHODS) != std::string::npos)
		// TODO clean methods
			//location.set_methods();
		if (line.find(INDEX) != std::string::npos)
			location.set_index(this->get_value(line));
		if (line.find(ALIAS) != std::string::npos)
			location.set_alias(this->get_value(line));
		if (line.find(RETURN) != std::string::npos)
			location.set_redirection(this->get_value(line));
	}	
	if (exit_context == 1 || (location.get_index().empty() || location.get_root().empty()))
		this->set_error_code(11);
	// location here/ {
}

void ConfigParser::clean_cgi(std::ifstream& config_file, std::string line)
{
	int	exit_context = 0;
	
	while (!exit_context && getline(config_file, line)) {
		if (line.find("}") != std::string::npos)
			exit_context = 1;
		if (line.find(ROOT) != std::string::npos)
			this->get_config().get_cgi().set_root(this->get_value(line));
		else if (line.find(CGI_PATH) != std::string::npos)
			this->clean_cgi_path(line);
		/* else if (line.find(CGI_EXT) != std::string::npos)
			this->clean_cgi_ext(line); */
	}
	if (!exit_context || (this->get_config().get_cgi().get_path().empty() || this->get_config().get_cgi().get_ext().empty() || this->get_config().get_cgi().get_root().empty()))
		this->set_error_code(12);
}

bool 								ConfigParser::clean_loc_autoindex(std::string line)
{
	if (line.find("on") != std::string::npos)
		return true;
	return false;
}

void				ConfigParser::clean_methods(std::string line)
{
	(void) line;
	// this->set_error_code(13);
}

void									ConfigParser::clean_cgi_path(std::string line)
{
	std::string temp;
	std::string temp2;
	size_t pos = 0;
	size_t pos2 = 0;
	size_t pos3 = 0;
	line = this->remove_end(line, ';');
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	std::cout << &line[pos] << std::endl;
	pos = line.find_first_of(" \r\t\b\f", pos);
	std::cout << &line[pos] << std::endl;
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	std::cout << &line[pos] << std::endl;
	while (pos != std::string::npos) {
		pos = line.find_first_not_of(" \r\t\b\f", pos);
		std::cout << &line[pos] << std::endl;
		pos3 = line.find_first_of(" \r\t\b\f", pos);
		std::cout << &line[pos] << std::endl;
		pos2 = line.find_last_of("/", pos3);
		temp = line.substr(pos2 + 1, pos3 - pos2 - 1);
		temp2 = line.substr(pos, pos2 + 1 - pos + temp.size());
		this->get_config().get_cgi().set_path(temp, temp2);
		pos = pos3;
	}
	std::cout << this->get_config().get_cgi().get_path().size() << std::endl;
	if (this->get_config().get_cgi().get_path().empty())
		this->set_error_code(14);
}

//std::vector<std::string>			ConfigParser::clean_cgi_ext(std::string line);


std::string ConfigParser::get_value(std::string line)
{
	line = this->remove_end(line, ';');	
	size_t pos = line.find_first_not_of(" \r\t\b\f");
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	line.erase(0, pos);
	pos = line.find_first_of(" \r\t\b\f");
	if (pos != std::string::npos)
		line.erase(pos);
	return line;
}

int    ConfigParser::to_int(std::string str)
{
    int    x;
    std::istringstream ss(str);
    ss >> x;
    return x;
}


int ConfigParser::exit_with_error(int err_code, std::ifstream& in_file)
{
	if (err_code == 1)
		std::cout << RED << NO_DEFAULT_CONFIG << RESET << std::endl;
	else if (err_code == 2)
		std::cout << RED << NO_VALID_SERVER << RESET << std::endl;
	else if (err_code == 3)
		std::cout << RED << NO_VALID_PORT << RESET << std::endl;
	else if (err_code == 4)
		std::cout << RED << NO_VALID_HOST << RESET << std::endl;
	else if (err_code == 5)
		std::cout << RED << NO_VALID_ERROR_PAGE << RESET << std::endl;
	else if (err_code == 6)
		std::cout << RED << NO_VALID_SERVER_NAME << RESET << std::endl;
	else if (err_code == 7)
		std::cout << RED << NO_VALID_CLIENT_MAX_BODY_SIZE << RESET << std::endl;
	else if (err_code == 8)
		std::cout << RED << NO_VALID_AUTOINDEX << RESET << std::endl;
	else if (err_code == 9)
		std::cout << RED << NO_VALID_ROOT << RESET << std::endl;
	else if (err_code == 10)
		std::cout << RED << NO_VALID_INDEX << RESET << std::endl;
	else if (err_code == 11)
		std::cout << RED << NO_VALID_LOCATION << RESET << std::endl;
	else if (err_code == 12)
		std::cout << RED << NO_VALID_CGI << RESET << std::endl;
	else if (err_code == 13)
		std::cout << RED << NO_VALID_METHODS<< RESET << std::endl;
	else if (err_code == 14)
		std::cout << RED << NO_VALID_CGI_PATH << RESET << std::endl;
	in_file.close();
	exit(err_code);
}