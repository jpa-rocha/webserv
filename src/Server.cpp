#include "../include/Server.hpp"

Server::Server(Config config): _config(config), _error(0)
{
	bzero(&_serv_addr, sizeof(sockaddr_in));
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
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0)
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

std::string	Server::get_type(std::string type)
{
	return this->_types.get_type(type);
}

void Server::send_response(int client_socket, const std::string& path)
{
    std::string			response_body = "";
    std::string			respond_path;
	std::string			response;
	std::ostringstream	response_stream;
	bool				is_cgi;

	std::string root = this->_config.get_root();
    if (path == "/")
		respond_path = this->_config.get_index();
	else if (path.find("cgi-bin") != std::string::npos)
		is_cgi = true;
    else
    	respond_path = path;
	// int flag = 0; //html = 0; css = 1; py = 2; bash = 3
    
    respond_path = root + clean_response_path(respond_path);
    std::ifstream file(respond_path.c_str());
    if (!file.is_open())
       send_404(root, response_stream);
    else
    {
        std::stringstream	file_buffer;
        
		if (respond_path.compare(respond_path.length() - 5, 5, ".html") == 0) {

			std::cout << BLUE <<  "----HTML----" << RESET << std::endl;
			file_buffer << file.rdbuf();
			response_body = file_buffer.str();
			response_stream << HTTPS_OK << 	this->get_type(".html") << response_body;
		}
		else if (respond_path.compare(respond_path.length() - 4, 4, ".ico") == 0)
		{
			std::cout << BLUE <<  "----ICO----" << RESET << std::endl;
			file_buffer << file.rdbuf();
			response_body = file_buffer.str();
			response_stream << HTTPS_OK << 	this->get_type(".ico") << response_body;
		}
		else if (respond_path.compare(respond_path.length() - 4, 4, ".css") == 0) {
			std::cout << BLUE <<  "----CSS----" << RESET << std::endl;
			std::string css = readFile("docs/www/utils/style.css");
			response_stream << HTTPS_OK << 	this->get_type(".css") << css;
		}
		else if (is_cgi == true) {
			std::cout << path << std::endl;
			this->handle_cgi(path, response_body);
            //response_body = ;
            response_stream << HTTPS_OK << 	this->get_type(".html") << response_body;
		}
    }
	
	// Send the response to the client
	response = response_stream.str();
    if (send(client_socket, response.c_str(), response.length(), 0) < 0  )
        std::cerr << RED << _RES_ERROR << RESET << std::endl;

    file.close();
	// close(client_socket);
    // client_socket = -1;
}

int		Server::clean_fd()
{
	int    fd;

	fd = fcntl(this->get_sockfd(), F_GETFL);
	if (fd != -1)
		close(this->get_sockfd());
	return EXIT_SUCCESS;
}

int		Server::handle_cgi(const std::string& path, std::string& response_body)
{
	//(void) response_body;
	std::ifstream file;
	int fd[2];
	/* int std_fd[2]; */
	std::string new_path = path;
	std::string shebang;
	char buff[1000];
	memset(buff, 0, 1000); // TODO we need to initialize buff (otherwise gives cond.jump etc. error)
    if (pipe(fd) < 0)
    {
        std::cout << "Error opening pipe" << std::endl;
        return EXIT_FAILURE;
    }
	
	new_path = remove_end(path, '?');
   	new_path = "." + new_path;
	// TODO check if ext is allowed
	std::cout << new_path << std::endl;
	file.open(new_path.c_str(), std::ios::in);
	if (file.fail() == true) {
		// TODO some error checking - what to return?
		std::cout << "DOES NOT EXIST" << std::endl;
		return EXIT_FAILURE;
	}
	getline(file, shebang);
	// TODO invalid file, no shebang
	if (shebang.find("#!") == std::string::npos)
		return EXIT_FAILURE;
	int pos = shebang.find_last_of("/");
	shebang = &shebang[pos] + 1;
	file.close();
	/* std_fd[0] = dup(STDIN_FILENO);
	std_fd[1] = dup(STDOUT_FILENO); */
    if (fork() == 0)
        exec_script(fd, new_path, shebang);
    else
    {
		waitpid(-1, NULL, 0);
		close(fd[1]);
		while (read(fd[0], buff, sizeof(buff) - 1)) {
			response_body += buff;
		}
		response_body.push_back('\0');
		close(fd[0]);
    }
	return EXIT_SUCCESS;
}

void	Server::exec_script(int *pipe, std::string path, std::string program)
{
    char *args[2];
    //(void)pipe_end;
	size_t i = 0;
	size_t j= 0;
    args[0] = new char [this->get_config().get_cgi().get_path().find(program.c_str())->second.length() + 1];
    for (i = 0; i < this->get_config().get_cgi().get_path().find(program.c_str())->second.length(); i++) {
        args[0][i] = this->get_config().get_cgi().get_path().find(program.c_str())->second[i];
	}
	args[0][i] = '\0';
    args[1] = new char [path.length() + 1];
    for (j = 0; j < path.length(); j++) {
        args[1][j] = path[j];
	}
	args[1][j] = '\0';
    args[2] = NULL;
    dup2(pipe[0], STDIN_FILENO);
	dup2(pipe[1], STDOUT_FILENO);
	//close(pipe[0]);
	close(pipe[1]);
    execve(args[0], args, NULL);
    perror("execve failed.");
    /*
	entrance pipe
	exit pipe file
	
	*/
}
void	Server::send_404(std::string root, std::ostringstream &response_stream)
{
	std::string response_body;

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
		response_stream << response_body;
		error404.close();
	}
}
