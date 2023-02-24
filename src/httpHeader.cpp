#include "../include/httpHeader.hpp"

/* httpHeader::httpHeader(): _method(), _uri(), _version(), _header()
{}  */

httpHeader::httpHeader(std::string header)
{
	int i = 0;
	size_t start = 0, end = 0;
	end = header.find(" ");
	std::string tmp_method = header.substr(start, end - start);
	start = end + 1;
	end = header.find(" ", start);
	this->_uri = header.substr(start, end - start);
	start = end + 1;
	end = header.find("\r\n", start);
	this->_version = header.substr(start, end - start);

	std::string methods[9] = {"GET", "POST", "DELETE", "PUT", \
		"HEAD", "OPTIONS", "TRACE", "CONNECT", "NONE"};
	for (int i = 0; i < 9; i++)
	{
		if (tmp_method == methods[i])
			break;
	}

	switch (i)
	{
		case 0:
			this->_method = GET;
			break;
		case 1:
			this->_method = POST;
			break;
		case 3:
			this->_method = DELETE;
			break;
		case 4:
			this->_method = PUT;
			break;
		case 5:
			this->_method = HEAD;
			break;
		case 6:
			this->_method = OPTIONS;
			break;
		case 7:
			this->_method = TRACE;
			break;
		case 8:
			this->_method = CONNECT;
			break;
		case 9:
			this->_method = NONE;
			break;
		default:
			std::cerr << "Error method not compatible!" << std::endl; 
			break;
	}

	start = end + 1;
	while ((end = header.find("\r\n", start)) != std::string::npos)
	{
		std::string line = header.substr(start, end - start);
		size_t separator = line.find(": ");
		if (separator != std::string::npos) {
			std::string name = line.substr(0, separator);
			std::string value = line.substr(separator + 2);
			this->setHeader(name, value);
		}
		start = end + 2;
	}

}

httpHeader::~httpHeader()
{}

httpHeader::httpHeader(const httpHeader& copy)
{
	*this = copy;
}

httpHeader &httpHeader::operator=(const httpHeader& rhs)
{
	if (this != &rhs)
	{
		this->_header = rhs._header;
		this->_method = rhs._method;
		this->_uri = rhs._uri;
		this->_version = rhs._version;
	}
	return *this;
}

const httpMethods &httpHeader::getMethod() const
{
	return(this->_method);
}

const std::string &httpHeader::getUri() const
{
	return(this->_uri);
}

const std::string &httpHeader::getVersion() const
{
	return(this->_version);
}

void httpHeader::setHeader(std::string name, std::string value)
{
	//TODO
	// check for errors - if key values are the same;
	// white spaces, chars toLower
	_header[ name ] = value;
}

void httpHeader::setMethod(httpMethods method)
{
	//TODO
	//check if method is valid
	
	_method = method;
}

void httpHeader::setVersion(std::string version)
{
	//TODO - check is version is valid
	
	_version = version;
}

void httpHeader::printHeader()
{
	std::cout << GREEN <<"Method: " << YELLOW << _method << RESET << std::endl;
    std::cout << GREEN << "URI: " << YELLOW << _uri << RESET << std::endl;
    std::cout << GREEN << "HTTP Version: " << YELLOW << _version << RESET << std::endl;

	std::cout << "map Contains:\n";
	for ( std::map<std::string, std::string>::iterator itr = _header.begin(); itr != _header.end(); ++itr)
	{
		std::cout << GREEN << itr->first << RESET << ": " << YELLOW << itr->second.c_str() << RESET << std::endl;
	}
}
