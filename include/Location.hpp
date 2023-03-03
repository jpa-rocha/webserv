#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <map>
# include <iostream>
# include "Utils.hpp"

	class Location {
		private:
			// Data to send
			std::string						_root;          	
      		bool							_autoindex;
     		std::map<short, bool> 			_methods;
      		std::string						_index;
      		std::string						_redirection;
     	 	std::string						_alias;       

		public:

			Location();
			// Setters   
			void set_root(std::string root);
			void set_autoindex(bool autoindex);
			void set_methods(short method, bool allowed);
			void set_index(std::string index);
			void set_redirection(std::string redirection);
			void set_alias(std::string alias);

			// Getters
			std::string						get_root();          	
      		bool							get_autoindex();
			std::map<short, bool>			get_methods();
      		std::string						get_index();
      		std::string						get_redirection();
     	 	std::string						get_alias();

			bool							check_method_at(short method);
			int								check_location();

	};
	std::ostream& operator<<(std::ostream& os, Location& location);
#endif