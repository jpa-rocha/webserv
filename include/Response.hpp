#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Utils.hpp"
# include "MIME.hpp"
# include <sstream>
# include <sys/wait.h>
# include <cstring>
# include "Config.hpp"


class Response 
{

    private:
		std::string                 _httpVersion;
		std::string                 _response_number;
        int                         _conn_fd;
        int                         _server_fd;
		std::string					_req_uri;
		bool		                _is_cgi;
        MIME                        _types;
		std::string			        _response_body;
		std::string			        _respond_path;
		std::string			        _response;
		std::ostringstream	        _response_stream;
		Config&      	   			_config;
    
        Response();
        Response(Response const &cpy);
        Response &operator=(Response const &rhs);

    public:
		Response(int conn_fd, int server_fd, Config& config);
        ~Response();

        void 	send_response(int client_socket, const std::string& path);

        int		handle_cgi(const std::string& path, std::string& response_body);
        void	exec_script(int *pipe, std::string path, std::string program);

        void 	send_404(std::string root, std::ostringstream &response_stream);

		void	set_uri(std::string uri);
};

#endif