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
    else {
        respond_path = path;
	}
	if (respond_path.find("cgi-bin") == std::string::npos)
    	respond_path = root + respond_path;
	else
    {
		this->handle_cgi(respond_path);
    }
	
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

int		Server::clean_fd()
{
	int    fd;

	fd = fcntl(this->get_sockfd(), F_GETFL);
	if (fd != -1)
		close(this->get_sockfd());
	return EXIT_SUCCESS;
}

int		Server::handle_cgi(std::string& path)
{
	std::ifstream file;
	int fd[2];

    if (pipe(fd) < 0)
    {
        std::cout << "Error opening pipe" << std::endl;
        return EXIT_FAILURE;
    }
	
	path = remove_end(path, '?');
    path = "." + path;
	std::cout << path << std::endl;
	file.open(path.c_str(), std::ios::in);
	if (file.fail() == true) {
		// TODO some error checking - what to return?
		std::cout << "DOES NOT EXIST" << std::endl;
		return EXIT_FAILURE;
	}
    if (!fork())
        exec_script(fd[0], path);
    else
    {

    }
	std::cout << "DOES EXIST" << std::endl;
    close(fd[0]);
	return fd[1];
}

void	Server::exec_script(int pipe_end, std::string path)
{
    char *args[2];
    (void)pipe_end;
	//std::vector<std::string> vector;

	// const char **argv = new const char* [vector.size()+2];   // extra room for program name and sentinel
	// argv [0] = path;         // by convention, argv[0] is program name
	// for (int j = 0;  j < vector.size()+1;  ++j)     // copy args
	// 		argv [j+1] = vector[j] .c_str();

	// argv [vector.size()+1] = NULL;  // end of arguments sentinel is NULL

    args[0] = (char *)malloc(sizeof(char) * 8);
    for (int i = 0; i < 8; i++)
        args[0][i] = "python3"[i];
    args[1] = (char *)malloc(sizeof(char) * path.length());
    for (size_t i = 0; i < path.length(); i++)
        args[1][i] = path[i];
    args[2] = NULL;
   //dup2(0, pipe_end);
   std::cout << args[0] << std::endl;
   std::cout << args[1] << std::endl;
   std::cout << args[2] << std::endl;
    execve(args[0], args, NULL);
    perror("execve failed.");
    /*
	entrance pipe
	exit pipe file
	
	*/
}