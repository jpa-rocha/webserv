/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrocha <jrocha@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 09:38:31 by jrocha            #+#    #+#             */
/*   Updated: 2023/02/20 13:59:57 by jrocha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	std::ifstream in_file;
	
	in_file.open("./webserv.config", std::ios::in);
	if (in_file.fail() == true) {
		// Class Error Handling
		this->exit_with_error(1);
	}
	
	//getline(in_file, line)

	//
	/* std::string tmp;
	int i = 1;
	while (config_variables[i].size() > 0) {
		std::getline(in_file, tmp); // this copies until the occurence '\n'
		if (tmp.find(";") != 4294967295) // we have a line that contains key=>value
		{
			long unsigned int pos = tmp.find(config_variables[i++].data()); //gets the array element position of " " char occurrence
			if (pos != 4294967295) {
				pos = tmp.find(" ");
				
				//std::cout << "pos within if " << pos << std::endl; // testing the new value of pos
				std::string hold = (tmp.data() + pos); // gets all the data from the index point (key + " ") and stores them to hold
				
				// test the output
				std::cout << hold << std::endl;

			//	function_to_
				
			}
		} */
}
	
	
	// At this point, We have the config file stored in in_file

int ConfigParser::exit_with_error(int err_code)
{
	if (err_code == 1) {
		std::cout << RED << "--- Could not find default configuration file at: ./webserver.config ---" << RESET << std::endl;
		exit(1);
	}
	return 1;
}