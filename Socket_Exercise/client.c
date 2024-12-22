#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT            8000
#define BUF_SIZE        1024

int main() {
    int sock = 0;
    struct  sockaddr_in serv_addr;
    char buffer[BUF_SIZE] = {0};
    char input[BUF_SIZE] = {0};
    
    // Creat socket
    if((sock = socket(AF_INET, SOCK_STREAM,0)) < 0) {
        perror("Creat failed.\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert address to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address.\n");
        exit(EXIT_FAILURE);
    }

    // Connect
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connect failed.\n");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected. Type 'exit' to quit.\n");

    // Chat
    while (1)
    {
        printf("You: ");
        fgets(input, BUF_SIZE, stdin);

        send(sock, input, strlen(input), 0);

        if(strncmp(input, "exit", 4) == 0) {
            printf("Disconnected.\n");
            break;
        }    

        memset(buffer, 0, BUF_SIZE);
        int read_value = read(sock, buffer, BUF_SIZE);
        if (read_value > 0)
        {
            printf("Sever: %s\n", buffer);
        }
    }
    close(sock);
    return 0;    
}