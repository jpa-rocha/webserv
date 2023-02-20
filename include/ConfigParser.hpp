/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrocha <jrocha@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 14:09:47 by jrocha            #+#    #+#             */
/*   Updated: 2023/02/20 13:40:52 by jrocha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <stdlib.h>
# include <string_view>
# include "Config.hpp"
# include "Utils.hpp"

static std::string config_variables[] = {"listen", "host", "server_name", "cgi_ext",
										"error_page", "allow_methods", "client_max_body_size",
										"root", "index", "autoindex", "return", "cgi_path"};

class ConfigParser {
	private:
		Config config;
	public:
		ConfigParser();
		ConfigParser(std::string config);
		

		// Getter
		Config getConfig();


		// Error Handling
		int exit_with_error(int err_code);
};
		


#endif