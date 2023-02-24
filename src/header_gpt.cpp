#include <map>
#include <string>

class HttpHeader {
public:
    HttpHeader() {}

    // Set a header field
    void set_field(const std::string& name, const std::string& value) {
        fields_[name] = value;
    }

    // Get a header field
    std::string get_field(const std::string& name) const {
        auto it = fields_.find(name);
        if (it != fields_.end()) {
            return it->second;
        } else {
            return "";
        }
    }

    // Remove a header field
    void remove_field(const std::string& name) {
        fields_.erase(name);
    }

    // Parse an HTTP header from a string
    static HttpHeader parse(const std::string& header) {
        HttpHeader http_header;
        size_t start = 0, end = 0;
        while ((end = header.find("\r\n", start)) != std::string::npos) {
            std::string line = header.substr(start, end - start);
            size_t separator = line.find(": ");
            if (separator != std::string::npos) {
                std::string name = line.substr(0, separator);
                std::string value = line.substr(separator + 2);
                http_header.set_field(name, value);
            }
            start = end + 2;
        }
        return http_header;
    }

    // Serialize the HTTP header to a string
    std::string to_string() const {
        std::string header;
        for (auto& field : fields_) {
            header += field.first + ": " + field.second + "\r\n";
        }
        header += "\r\n";
        return header;
    }

private:
    std::map<std::string, std::string> fields_;
};


/* __________________________________________________________________________ */

#include <iostream>
#include <unistd.h>
#include <string>
//#include "http_Header.h" // assume you have defined the HttpHeader class in this header file

// Parses an HTTP header from a file descriptor and returns an HttpHeader object.
HttpHeader parse_http_header(int fd) {
    std::string header_str;
    char buffer[1024];
    ssize_t bytes_read;
    bool found_end_of_header = false;
    
    // Read bytes from the file descriptor until the end of the header is found.
    while (!found_end_of_header && (bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        header_str.append(buffer, bytes_read);
        size_t pos = header_str.find("\r\n\r\n");
        if (pos != std::string::npos) {
            found_end_of_header = true;
            header_str.resize(pos); // Remove the trailing \r\n\r\n
        }
    }
    
    // Parse the string as an HTTP header.
    HttpHeader header = HttpHeader::parse(header_str);
    
    return header;
}

int main() {
    // Assume you have opened a file descriptor to an HTTP request
    int fd = open("http_request.txt", O_RDONLY);

    // Parse the HTTP header from the file descriptor
    HttpHeader header = parse_http_header(fd);

    // Do something with the header
    std::string content_type = header.get_field("Content-Type");
    std::string content_length = header.get_field("Content-Length");
    std::cout << "Content-Type: " << content_type << std::endl;
    std::cout << "Content-Length: " << content_length << std::endl;
    
    // Close the file descriptor
    close(fd);

    return 0;
}
