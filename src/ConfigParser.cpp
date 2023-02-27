#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	std::ifstream in_file;
	
	this->set_error_code(0);
	this->set_n_servers(0);
	//this->_config = new std::vector<Config>;
	
	// Check if config file exists
	in_file.open("./webserv.config", std::ios::in);
	if (in_file.fail() == true)
		this->exit_with_error(1, in_file);
	
	// Check if server context exists
//	std::cout << line << std::endl;
	if (this->check_server_context(in_file) == false)
		this->exit_with_error(this->get_error_code(), in_file);
}

ConfigParser::~ConfigParser()
{
	//  for (size_t i = 0; i < this->_config.size(); i++)
	//  	delete this->_config[i];
}

Config& ConfigParser::get_config(int i)
{
	return (this->_config.at(i));
}

int ConfigParser::get_error_code()
{
	return this->_error_code;
}

int ConfigParser::get_n_servers()
{
	return this->_n_servers;
}

void ConfigParser::set_error_code(int error_code)
{
	this->_error_code = error_code;
}

void ConfigParser::set_n_servers(int i)
{
	this->_n_servers = i;
}

bool ConfigParser::check_server_context(std::ifstream& config_file)
{
	std::string line;
	int context = 0;

	while (getline(config_file, line) && this->get_error_code() == 0) {
		line = this->remove_comments(line);
		if (line.find_first_not_of(" \r\t\b\f") == std::string::npos)
			continue;
		
		// // TODO server should be the top element
		if ((line.find("server") != std::string::npos && this->check_def_format("server", line) && line.find("{") != std::string::npos) && line.find("}") != std::string::npos)
			return false;
		else if ((line.find("server") != std::string::npos && this->check_def_format("server", line) && line.find("{") != std::string::npos) && context == 0) {
			this->set_n_servers(this->get_n_servers() + 1);
			context += 1;
			this->_config.push_back(Config());
		}
		else if (context == 0)
			return false;
		if ((context && line.find(LISTEN) != std::string::npos) && this->check_def_format(LISTEN, line))
			this->clean_listen(line);
		else if ((context && line.find(HOST) != std::string::npos) && this->check_def_format(HOST, line))
			this->clean_host(line);
		else if ((context && line.find(ERROR_PAGE) != std::string::npos) && this->check_def_format(ERROR_PAGE, line))
			this->clean_error_page(line);
		else if ((context && line.find(SERVER_NAME) != std::string::npos) && this->check_def_format(SERVER_NAME, line))
			this->clean_server_name(line);
		else if ((context && line.find(CLIENT_MAX_BODY) != std::string::npos) && this->check_def_format(CLIENT_MAX_BODY, line))
			this->clean_client_max_body_size(line);
		else if ((context && line.find(AUTOINDEX) != std::string::npos) && this->check_def_format(AUTOINDEX, line))
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
	//std::cout << _n_servers << std::endl;
	//std::cout << this->get_config(0).get_cgi() << std::endl;
	// std::cout << "output" << std::endl;
	// std::cout << this->get_config(0).get_cgi().get_path().at("python3") << std::endl;
	// std::cout << this->get_config(0).get_cgi().get_path().at("bash") << std::endl;
	// std::cout << this->get_config(0).get_cgi().get_ext().at(0) << std::endl;
	// std::cout << this->get_config(0).get_cgi().get_ext().at(1) << std::endl;
	// std::cout << this->get_config(0).get_cgi().get_root() << std::endl;
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
	this->get_config(this->get_n_servers() - 1).set_port(this->to_int(line.c_str()));
}

void ConfigParser::clean_host(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		this->set_error_code(4);
	this->get_config(this->get_n_servers() - 1).set_host(inet_addr(line.c_str()));
}

// TODO break if there is no value
void ConfigParser::clean_error_page(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		this->set_error_code(5);
	std::size_t pos = line.find_first_of("/");
	std::string path = &line[pos];
	pos = line.find_first_of(" ");
	line.erase(pos);
	this->get_config(this->get_n_servers() - 1).set_default_error(this->to_int(line), path);
}

void ConfigParser::clean_server_name(std::string line)
{
	line = this->get_value(line);
	if (line.size() == 0)
		this->set_error_code(6);
	this->get_config(this->get_n_servers() - 1).set_server_name(line);
}

void ConfigParser::clean_client_max_body_size(std::string line)
{
	line = this->find_int(line);
	if (line.size() == 0)
		this->set_error_code(7);
	this->get_config(this->get_n_servers() - 1).set_client_max_body_size(this->to_int(line.c_str()));
}

void ConfigParser::clean_autoindex(std::string line)
{
	if (line.find("on") != std::string::npos)
		this->get_config(this->get_n_servers() - 1).set_autoindex(true);
	else if (line.find("off") != std::string::npos)
		this->get_config(this->get_n_servers() - 1).set_autoindex(false);
	else	
		this->set_error_code(8);
}

void ConfigParser::clean_root(std::string line)
{
	line = this->get_value(line);
	if (line.size() == 0)
		this->set_error_code(9);
	this->get_config(this->get_n_servers() - 1).set_root(line);
}

void ConfigParser::clean_index(std::string line)
{
	line = this->get_value(line);
	if (line.size() == 0)
		this->set_error_code(10);
	this->get_config(this->get_n_servers() - 1).set_index(line);
}

void ConfigParser::clean_location(std::ifstream& config_file, std::string line)
{
	Location location;
	std::string key = this->get_value(line);
	int	exit_context = 0;
	while (exit_context == 0 && getline(config_file, line)) {
		line = this->remove_comments(line);
		if (line.find("}") != std::string::npos)
			exit_context = 1;
		if ((line.find(ROOT) != std::string::npos) && this->check_def_format(ROOT, line))
			location.set_root(this->get_value(line));
		if ((line.find(AUTOINDEX) != std::string::npos) && this->check_def_format(AUTOINDEX, line))
			location.set_autoindex(this->clean_loc_autoindex(line));
		if ((line.find(ALLOW_METHODS) != std::string::npos) && this->check_def_format(ALLOW_METHODS, line))
			this->clean_methods(line, location);
		if ((line.find(INDEX) != std::string::npos) && this->check_def_format(INDEX, line))
			location.set_index(this->get_value(line));
		if ((line.find(ALIAS) != std::string::npos)  && this->check_def_format(ALIAS, line))
			location.set_alias(this->get_value(line));
		if ((line.find(RETURN) != std::string::npos)  && this->check_def_format(RETURN, line))
			location.set_redirection(this->get_value(line));
	}	
	if (exit_context != 1 || (location.get_index().empty() || location.get_root().empty()))
		this->set_error_code(11);
	this->get_config(this->get_n_servers() - 1).set_location(key, location);
	// location here/ {
}

void ConfigParser::clean_cgi(std::ifstream& config_file, std::string line)
{
	int	exit_context = 0;
	CGI	cgi;

	while (!exit_context && getline(config_file, line)) {
		if (line.find("}") != std::string::npos)
			exit_context = 1;
		if ((line.find(ROOT) != std::string::npos) && this->check_def_format(ROOT, line))
			cgi.set_root(this->get_value(line));
		else if ((line.find(CGI_PATH) != std::string::npos) && this->check_def_format(CGI_PATH, line))
			this->clean_cgi_path(line, cgi);
		else if ((line.find(CGI_EXT) != std::string::npos) && this->check_def_format(CGI_EXT, line))
			this->clean_cgi_ext(line, cgi);
	}
	if (!exit_context || (cgi.get_path().empty() || cgi.get_ext().empty() || cgi.get_root().empty()))
		this->set_error_code(12);
	this->_config[this->get_n_servers() - 1].set_cgi(cgi);
}

bool 								ConfigParser::clean_loc_autoindex(std::string line)
{
	if (line.find("on") != std::string::npos)
		return true;
	return false;
}

void				ConfigParser::clean_methods(std::string line, Location& location)
{
	line = this->remove_end(line, ';');
	size_t pos = 0;
	size_t pos2 = 0;
	line = this->remove_end(line, ';');
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	while (pos != std::string::npos) {
		pos = line.find_first_not_of(" \r\t\b\f", pos);
		pos2 = line.find_first_of(" \r\t\b\f", pos);
		if  (this->get_method_num(line.substr(pos, pos2 - pos)) < 0)
			this->set_error_code(13);
		location.set_methods(this->get_method_num(line.substr(pos, pos2 - pos)), 1);
		pos = pos2;
	}
	if (location.get_methods().empty())
		this->set_error_code(13);
	// this->set_error_code(13);
}

void									ConfigParser::clean_cgi_path(std::string line, CGI &cgi)
{
	std::string temp;
	std::string temp2;
	size_t pos = 0;
	size_t pos2 = 0;
	size_t pos3 = 0;
	line = this->remove_end(line, ';');
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	while (pos != std::string::npos) {
		pos3 = line.find_first_of(" \r\t\b\f", pos);
		pos2 = line.find_last_of("/", pos3);
		temp =  line.substr(pos2 + 1, pos3 - pos2 - 1);
		temp2 = line.substr(pos, pos2 + 1 - pos + (temp).size());
		cgi.set_path(temp, temp2);
		pos = line.find_first_not_of(" \r\t\b\f", pos3);
	}
	if (cgi.get_path().empty())
		this->set_error_code(14);
}

void			ConfigParser::clean_cgi_ext(std::string line, CGI &cgi)
{
	size_t pos = 0;
	size_t pos2 = 0;
	line = this->remove_end(line, ';');
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	while (pos != std::string::npos) {
		pos = line.find_first_not_of(" \r\t\b\f", pos);
		pos2 = line.find_first_of(" \r\t\b\f", pos);
		cgi.set_ext(line.substr(pos, pos2 - pos));
		pos = pos2;
	}
	if (cgi.get_path().empty())
		this->set_error_code(15);
}


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

bool	ConfigParser::check_def_format(std::string def, std::string line)
{
	size_t pos = line.find(def);
	if ((pos == 0 || pos == line.find_first_not_of(" \r\t\b\f")) 
		&& line.find_first_of(" \r\t\b\f", pos) - pos == def.size())
		return true;
	return false;
}

int			ConfigParser::get_method_num(std::string method)
{
	if (method == "GET")
		return 0;
	else if (method == "POST")
		return 1;
	else if (method == "PUT")
		return 2;
	else if (method == "HEAD")
		return 3;
	else if (method == "OPTIONS")
		return 4;
	else if (method == "TRACE")
		return 5;
	else if (method == "CONNECT")
		return 6;
	else
		return -1;
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
	else if (err_code == 15)
		std::cout << RED << NO_VALID_CGI_EXT << RESET << std::endl;
	in_file.close();
	return(err_code);
}
