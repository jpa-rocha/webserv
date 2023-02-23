#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <vector>
# include <iostream>

	class Location {
		private:
			// Data to send
			std::string						_root;          	
      		bool							_autoindex;
     		std::vector<short> 				_methods;
      		std::string						_index;
      		std::string						_redirection;
     	 	std::string						_alias;       

		public:

			// Setters   
			void set_root(std::string root);
			void set_autoindex(bool autoindex);
			void set_methods(std::vector<short> methods);
			void set_index(std::string index);
			void set_redirection(std::string redirection);
			void set_alias(std::string alias);

			// Getters
			std::string						get_root();          	
      		bool							get_autoindex();
     		std::vector<short> 				get_methods();
      		std::string						get_index();
      		std::string						get_redirection();
     	 	std::string						get_alias();      

	};
	
#endif