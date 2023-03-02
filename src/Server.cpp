#include "../include/Server.hpp"

Server::Server(Config config): _config(config), _error(0)
{
	this->_port = this->_config.get_port();
	if (this->init_socket() != 0)
		return;
	if (this->bind_socket() != 0)
		return;
	if (this->listen_socket() != 0)
		return;
}

Server::~Server() {}

int	Server::init_socket()
{
	if ((this->_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << RED << SOCK_ERROR << RESET << std::endl;
        this->_error = 16;
		return 16;
    }
	int optval = 1;
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        std::cerr << RED << SOCK_OPT_ERROR << RESET << std::endl;
		this->_error = 17;
        return 17;
    }
	return EXIT_SUCCESS;
}

int	Server::bind_socket()
{
	this->_serv_addr.sin_family = AF_INET;
    this->_serv_addr.sin_port = htons(_port);
    this->_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(this->_sockfd, (struct sockaddr *)&this->_serv_addr, sizeof(this->_serv_addr)) < 0)
    {
        std::cerr << RED << BIND_ERROR << RESET << std::endl;
		this->_error = 18;
        return 18;
    }
	return EXIT_SUCCESS;
}

int	Server::listen_socket()
{
	if (listen(_sockfd, MAX_CONN) < 0)
    {
         std::cerr << RED << LISTEN_ERROR << RESET << std::endl;
		 this->_error = 19;
         return 19;
    }
	return EXIT_SUCCESS;
}


/* ---------- GETTERS ---------- */
int	Server::getError() const
{
	return _error;
}

int Server::get_sockfd() const
{ 
	return _sockfd;

}

int Server::get_port() const 
{ 
    return _port; 
}

Config &Server::get_config()
{
	return _config;
}

void Server::send_response(int client_socket, const std::string& path)
{
    std::string			response_body;
    std::string			respond_path;
	std::string			response;
	std::ostringstream	response_stream;

	std::string root = this->_config.get_root();
    if (path == "/favicon.ico" || path == "/")
        respond_path = this->_config.get_index();
    else
        respond_path = path;
    
    respond_path = root + respond_path;
    std::ifstream file(respond_path.c_str());
    if (!file.is_open())
    {
        // if the file cannot be opened, send a 404 error
        std::ifstream error404((root + this->_config.get_error_path(404)).c_str());
        if (!error404.is_open())
            std::cerr << RED << _404_ERROR << RESET << std::endl;
        else
        {
            std::stringstream	file_buffer;
            file_buffer << error404.rdbuf();
            response_body = file_buffer.str();
            response_stream << "HTTP/1.1 404 Not Found\r\n\r\n";
            error404.close();
        }
    }
    else
    {
        std::stringstream	file_buffer;
        file_buffer << file.rdbuf();
        response_body = file_buffer.str();
		// Generate the HTTP response headers
    	response_stream << "HTTP/1.1 200 OK\r\n";
		response_stream << 	"Content-Type: text/html\r\n\r\n";
        // Add the content to the response body
    }
	
    response_stream << response_body;
    response_stream << "<style>";
	std::string css = readFile("docs/www/utils/style.css");
	response_stream << css;
	response_stream << "</style>";
	
	// Send the response to the client
	response = response_stream.str();
    //std::cerr << RED << response_body << RESET <<std::endl;
    if ( send(client_socket, response.c_str(), response.length(), 0) < 0  )
        std::cerr << RED << _RES_ERROR << RESET << std::endl;

    // Close the file
    file.close();
	close(client_socket);
    client_socket = -1;
}
