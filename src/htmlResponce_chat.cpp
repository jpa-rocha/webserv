#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // bind the socket to a port
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    bind(server_socket, (struct sockaddr *)&address, sizeof(address));
    
    // listen for connections
    listen(server_socket, 5);
    
    while (true) {
        // accept a connection
        int client_socket = accept(server_socket, NULL, NULL);
        
        // read the HTTP request
        char buffer[1024] = {0};
        read(client_socket, buffer, 1024);
        std::stringstream ss(buffer);
        std::string request_method, request_path, http_version;
        ss >> request_method >> request_path >> http_version;
        
        // open the file requested by the client
        std::ifstream file(request_path.substr(1));
        if (!file.is_open()) {
            // if the file cannot be opened, send a 404 error
            std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
            write(client_socket, response.c_str(), response.size());
        } else {
            // if the file is opened successfully, send its contents as the response
            std::stringstream file_buffer;
            file_buffer << file.rdbuf();
            std::string file_contents = file_buffer.str();
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + file_contents;
            write(client_socket, response.c_str(), response.size());
        }
        
        // close the client socket
        close(client_socket);
    }
    
    // close the server socket
    close(server_socket);
    
    return 0;
}
