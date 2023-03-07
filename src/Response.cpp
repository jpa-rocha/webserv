#include "Response.hpp"


Response::Response(int conn_fd, int server_fd, Config& config, std::string req_uri) : _config(config)
{
    _conn_fd = conn_fd;
    _server_fd = server_fd;
    send_response(_conn_fd, req_uri);
};


Response::~Response()
{

};

void 	Response::send_response(int client_socket, const std::string& path)
{
/* --------------------------------------------------------------------------- */
    //TODO get path function
	if (path == "/")
		_respond_path = _config.get_index();
	else if (path.find("cgi-bin") != std::string::npos)
		_is_cgi = true;
    else
    	_respond_path = path;
    _respond_path = _config.get_root() + clean_response_path(_respond_path);
    std::ifstream file(_respond_path.c_str());
/* --------------------------------------------------------------------------- */
    std::cout << RED << _respond_path << RESET << std::endl;
	if (!file.is_open())
	{
       send_404(_config.get_root(), _response_stream);
	}
    else
    {
        /* 
			TODO
				- which response
				- resopose number
				- response type
				- response body
				- send the response
				- close the file
		*/
		std::stringstream	file_buffer;
		if (_respond_path.compare(_respond_path.length() - 5, 5, ".html") == 0) {

			std::cout << BLUE <<  "----HTML----" << RESET << std::endl;
			file_buffer << file.rdbuf();
			_response_body = file_buffer.str();
			_response_stream << HTTPS_OK << _types.get_content_type(".html") << _response_body;
		}
		else if (_respond_path.compare(_respond_path.length() - 4, 4, ".ico") == 0)
		{
			std::cout << BLUE <<  "----ICO----" << RESET << std::endl;
			file_buffer << file.rdbuf();
			_response_body = file_buffer.str();
			_response_stream << HTTPS_OK << _types.get_content_type(".ico") << _response_body;
		}
		else if (_respond_path.compare(_respond_path.length() - 4, 4, ".css") == 0) {
			std::cout << BLUE <<  "----CSS----" << RESET << std::endl;
			std::string css = readFile("docs/www/utils/style.css");
			_response_stream << HTTPS_OK << _types.get_content_type(".css") << css;
		}
		else if (_respond_path.compare(_respond_path.length() - 4, 4, ".png") == 0) {
			std::cout << BLUE <<  "----PNG----" << RESET << std::endl;
			file_buffer << file.rdbuf();
			_response_body = file_buffer.str();
			_response_stream << HTTPS_OK << _types.get_content_type(".png") << _response_body;
		}
		else if (_is_cgi == true) {
			std::cout << path << std::endl;
			this->handle_cgi(path, _response_body);
            _response_stream << HTTPS_OK << _types.get_content_type(".html") << _response_body;
		}
    }
	
	// Send the response to the client
	_response = _response_stream.str();
    if (send(client_socket, _response.c_str(), _response.length(), 0) < 0  )
        std::cerr << RED << _RES_ERROR << RESET << std::endl;

    file.close();
	// close(client_socket);
    // client_socket = -1;
    
}

int		Response::handle_cgi(const std::string& path, std::string& response_body)
{
    std::ifstream file;
	int fd[2];
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

void	Response::exec_script(int *pipe, std::string path, std::string program)
{
    char *args[2];
	size_t i = 0;
	size_t j = 0;
    args[0] = new char [_config.get_cgi().get_path().find(program.c_str())->second.length() + 1];
    for (i = 0; i < _config.get_cgi().get_path().find(program.c_str())->second.length(); i++) {
        args[0][i] = _config.get_cgi().get_path().find(program.c_str())->second[i];
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
	close(pipe[1]);
    execve(args[0], args, NULL);
    perror("execve failed.");
}

void 	Response::send_404(std::string root, std::ostringstream &response_stream)
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