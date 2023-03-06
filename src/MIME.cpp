#include "../include/MIME.hpp"

MIME::MIME()
{
	this->_mime_types[".html"] = "text/html";
    this->_mime_types[".htm"] = "text/html";
    this->_mime_types[".css"] = "text/css";
    this->_mime_types[".ico"] = "image/x-icon";
    this->_mime_types[".avi"] = "video/x-msvideo";
    this->_mime_types[".bmp"] = "image/bmp";
    this->_mime_types[".doc"] = "application/msword";
    this->_mime_types[".gif"] = "image/gif";
    this->_mime_types[".gz"] = "application/x-gzip";
    this->_mime_types[".ico"] = "image/x-icon";
    this->_mime_types[".jpg"] = "image/jpeg";
    this->_mime_types[".jpeg"] = "image/jpeg";
    this->_mime_types[".png"] = "image/png";
    this->_mime_types[".txt"] = "text/plain";
    this->_mime_types[".mp3"] = "audio/mp3";
    this->_mime_types[".pdf"] = "application/pdf";
    this->_mime_types["default"] = "text/html";
}

std::string MIME::get_type(std::string type)
{
	if (this->_mime_types.count(type))
		return this->_mime_types[type];
	return this->_mime_types["default"];
}