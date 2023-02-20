/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrocha <jrocha@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 14:04:07 by jrocha            #+#    #+#             */
/*   Updated: 2023/02/20 13:05:22 by jrocha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../include/ConfigParser.hpp"

int main (int argc, char** argv)
{
	(void) argv;
	if (argc > 2) {
		// too many arguments
	}
	else if (argc == 2) {
		// error check and config parse
		
	}
	else {
		ConfigParser();
		
		// run default config file	
	}
	
	return EXIT_SUCCESS;
}