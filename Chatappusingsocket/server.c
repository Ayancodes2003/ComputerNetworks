#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for client connections
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    // Accept incoming client connections
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    
    // Server loop to receive and send messages
    while (1) {
        // Receive message from client
        valread = read(new_socket, buffer, BUFFER_SIZE);
        printf("Client: %s\n", buffer);
        
        // Check if client wants to exit
        if (strcmp(buffer, "exit") == 0) {
            printf("Client disconnected\n");
            break;
        }
        
        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        
        // Send message to client
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(new_socket, buffer, strlen(buffer), 0);
        
        // Check if server wants to exit
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Server disconnected\n");
            break;
        }
        
        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
    }
    
    // Close the socket
    close(new_socket);
    close(server_fd);
    
    return 0;
}

