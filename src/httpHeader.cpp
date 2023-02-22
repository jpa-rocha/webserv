#include "../include/httpHeader.hpp"

httpHeader::httpHeader(): _method(), _uri(), _version(), _header()
{
	
} 

httpHeader::~httpHeader()
{

}

httpHeader::httpHeader(int requestFd)
{
	//check if request in valid
	
	// requestFd will be read & keys will be grouped?
	// std::ifstream 
	// shall we read the fd in another func and pass here
	FILE *file = fdopen(requestFd, "r+");
	std::fstream *f(file);


	std::string line;
	getline(file, line);
		// first line : 
		// 1) find the first string
		// 2) second URI 
		// 3) third string version 


	/* while(std::getline(requestFd, line))
	{
		
		
	} */

}

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

void httpHeader::setMethod(std::string method)
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
	std::cout << "Method: " << _method << std::endl;
    std::cout << "URI: " << _uri << std::endl;
    std::cout << "HTTP Version: " << _version << std::endl;

	std::cout << "map Contains:\n";
	for ( std::map<std::string, std::string>::iterator itr = _header.begin(); itr != _header.end(); ++itr)
	{
		std::cout << "Key =>" << itr->first << ", Value => " << itr->second.c_str() << std::endl;
	}
}
