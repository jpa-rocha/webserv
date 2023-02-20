/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrocha <jrocha@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 11:00:01 by jrocha            #+#    #+#             */
/*   Updated: 2023/02/20 11:54:21 by jrocha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <vector>

	class Location {
		private:
			// Data to send
			std::string						_root;          	
      		bool							_autoindex;
     		std::vector<short> 				_methods;
      		std::string						_index;
      		std::string						_redirection;
     	 	std::string						_alias;

			// CGI
			std::string						_cgi_root;
      		std::vector<std::string>		_cgi_path;
			std::vector<std::string>		_cgi_ext;               
	};
	
#endif