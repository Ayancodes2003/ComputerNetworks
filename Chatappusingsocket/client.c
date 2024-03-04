#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }
    
    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    // Client loop to send and receive messages
    while (1) {
        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        
        // Send message to server
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);
        
        // Check if client wants to exit
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Client disconnected\n");
            break;
        }
        
        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        
        // Receive message from server
        valread = read(sock, buffer, BUFFER_SIZE);
        printf("Server: %s\n", buffer);
        
        // Check if server wants to exit
        if (strcmp(buffer, "exit") == 0) {
            printf("Server disconnected\n");
            break;
        }
       
    }
    
    // Close the socket
    close(sock);
    
    return 0;
}

