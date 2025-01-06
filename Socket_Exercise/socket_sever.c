#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT            8000
#define BUF_SIZE        1024

int main() {
    int serv_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE] = {0};
    char *message = "Message received";

    // Creat socket
    if((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Creat failed.\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    // Convert address to binary
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
    {
        perror("Invalid address.\n");
        exit(EXIT_FAILURE);
    }

    // Bind
    if (bind(serv_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("Bind failed.\n");
        close(serv_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(serv_fd, 3) < 0)
    {
        perror("Listen failed.\n");
        close(serv_fd);
        exit(EXIT_FAILURE);
    }

    printf("Listening port %d", PORT);

    // Accept
    if ((new_socket = accept(serv_fd, (struct sockaddr *)&address,(socklen_t *)&addrlen)) < 0) {
        perror("Accept failed.\n");
        close(serv_fd);
        exit(EXIT_FAILURE);
    }

        if(strncmp(buffer, "exit", 4) == 0) {
            printf("Disconneted.\n");
            break;
    printf("Connected. Type 'exit' to quit.\n");

    // Chat
    while (1)
    {
        memset(buffer, 0, BUF_SIZE);
        int read_value = read(new_socket, buffer, BUF_SIZE);
        if(read_value < 0){
            perror("Read failed.\n");
            break;
        }

        printf("Client: %s", buffer);
        }

        send(new_socket, message, strlen(message), 0);
    }
    close(new_socket);
    close(serv_fd);
    return 0;
}