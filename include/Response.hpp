#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Utils.hpp"
# include "MIME.hpp"
# include <sstream>
# include <sys/wait.h>
# include <cstring>
# include "Config.hpp"
# include "httpHeader.hpp"

class httpHeader;

class Response 
{

    private:
		std::string                 _httpVersion;
		std::string                 _response_number;
		std::string					_buff;
        int                         _conn_fd;
        int                         _server_fd;
		std::string					_req_uri;
		bool		                _is_cgi;
        MIME                        _types;
		std::string			        _response_body;
		std::string			        _respond_path;
		std::string			        _response;
		Config      	   			_config;
    
        Response();

    public:
		httpHeader	 				_request;
		Response(int conn_fd, int server_fd, Config& config);
        Response(Response const &cpy);
        Response &operator=(Response const &rhs);
        ~Response();

        void 	send_response();

        int		handle_cgi(const std::string& path, std::string& response_body, std::ostringstream &response_stream);
        void	exec_script(int *pipe, std::string path, std::string program);

        void 	send_404(std::string root, std::ostringstream &response_stream);

		void	new_request(httpHeader &request);

		void	responseToGET(std::ifstream &file, const std::string& path, std::ostringstream &response_stream);

		bool	response_complete() const;
};

#endif