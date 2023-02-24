#ifndef UTILS_HPP
# define UTILS_HPP

/*

	Colors

*/

# define RESET   "\033[0m"
# define BLACK   "\033[30m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define PURPLE "\033[35m"
# define CYAN    "\033[36m"
# define WHITE   "\033[37m"

/*

	Error Messages

*/

// error_code 1
# define NO_DEFAULT_CONFIG "ERROR: --- Could not find default configuration file at: ./webserver.config ---"

// error_code 2
# define NO_VALID_SERVER "ERROR: --- Could not find a valid ** server {} ** context in the provided configuration file ---"

// error_code 3
# define NO_VALID_PORT "ERROR: --- Could not find a valid ** port ** configuration ---"

// error_code 4
# define NO_VALID_HOST "ERROR: --- Could not find a valid ** host ** configuration ---"

// error_code 5
# define NO_VALID_ERROR_PAGE "ERROR: --- Could not find a valid ** error_page ** configuration ---"

// error_code 6
# define NO_VALID_SERVER_NAME "ERROR: --- An empty ** server_name ** was provided ---"

// error_code 7
# define NO_VALID_CLIENT_MAX_BODY_SIZE "ERROR: --- Could not find a valid ** client_max_body_size ** configuration ---"

// error_code 8
# define NO_VALID_AUTOINDEX "ERROR: --- Could not find a valid ** autoindex ** configuration ---"

// error_code 9
# define NO_VALID_ROOT "ERROR: --- Could not find a valid ** root ** configuration ---"

// error_code 10
# define NO_VALID_INDEX "ERROR: --- Could not find a valid ** index ** configuration ---"

// error_code 11
# define NO_VALID_LOCATION "ERROR: --- An invalid ** location ** context was provided ---"

// error_code 12
# define NO_VALID_CGI "ERROR: --- An invalid ** CGI ** context was provided ---"

// error_code 13
# define NO_VALID_METHODS "ERROR: --- Could not find a valid ** allowed_methods ** configuration ---"

// error_code 14
# define NO_VALID_CGI_PATH "ERROR: --- Could not find a valid ** index ** configuration ---"

#endif