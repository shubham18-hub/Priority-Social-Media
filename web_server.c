#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

const char* html_response = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"Connection: close\r\n"
"\r\n"
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"    <title>Priority Social Media</title>\n"
"    <style>\n"
"        body { font-family: Arial, sans-serif; margin: 40px; background: #f0f0f0; }\n"
"        .container { max-width: 800px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; }\n"
"        h1 { color: #333; text-align: center; }\n"
"        .status { background: #e8f5e8; padding: 15px; border-radius: 5px; margin: 20px 0; }\n"
"    </style>\n"
"</head>\n"
"<body>\n"
"    <div class='container'>\n"
"        <h1>Priority Social Media Application</h1>\n"
"        <div class='status'>\n"
"            <h3>✅ Application Status: Running</h3>\n"
"            <p>This is a C-based social media application successfully deployed on Render.</p>\n"
"            <p>The application includes:</p>\n"
"            <ul>\n"
"                <li>GTK-based GUI frontend</li>\n"
"                <li>Priority-based social media features</li>\n"
"                <li>Multiple compilation options</li>\n"
"                <li>Cross-platform compatibility</li>\n"
"            </ul>\n"
"        </div>\n"
"        <p><strong>Repository:</strong> <a href='https://github.com/shubham18-hub/Priority-Social-Media'>GitHub</a></p>\n"
"    </div>\n"
"</body>\n"
"</html>";

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Get port from environment variable or use default
    char* port_str = getenv("PORT");
    int port = port_str ? atoi(port_str) : 10000;

    printf("Starting Priority Social Media Web Server on port %d\n", port);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        read(new_socket, buffer, BUFFER_SIZE);
        printf("Request received\n");
        
        send(new_socket, html_response, strlen(html_response), 0);
        printf("Response sent\n");
        
        close(new_socket);
    }

    return 0;
}