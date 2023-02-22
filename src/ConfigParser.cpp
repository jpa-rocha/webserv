#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	std::ifstream in_file;
	
	Config test;
	std::cout << test << std::endl;
	
	this->_n_servers = 0;
	// Check if config file exists
	in_file.open("./webserv.config", std::ios::in);
	if (in_file.fail() == true)
		this->exit_with_error(1, in_file);
	
	// Check if server context exists
	std::string line;
	while (line.size() == 0) 
		getline(in_file, line);
//	std::cout << line << std::endl;
	if (this->check_server_context(in_file, line) == false)
		this->exit_with_error(2, in_file);
	//std::cout << line << std::endl;
}

Config ConfigParser::get_config()
{
	return this->_config[0];
}


bool ConfigParser::check_server_context(std::ifstream& config_file, std::string& line)
{
	int context = 0;
	line = this->remove_comments(line);
	if ((line.find("server") == std::string::npos || line.find("{") == std::string::npos) || 
		((line.find("server") != std::string::npos && line.find("{") != std::string::npos) && line.find("}") != std::string::npos))
		return false;
	this->_n_servers += 1;
	context += 1;
	while (getline(config_file, line)) {
		line = this->remove_comments(line);
		std::cout << line << std::endl;
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
	if (line.find("#") != std::string::npos) {
		line.erase(line.find("#"));
	}
	return line;
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