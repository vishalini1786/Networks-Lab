#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main()
{
    int sock;
    char message[SIZE];
    struct sockaddr_in server;

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    // Set server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to chat server.\n");

    while (1)
    {
        // Send message to server
        printf("Client: ");
        fgets(message, SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        send(sock, message, strlen(message) + 1, 0);

        if (strcmp(message, "exit") == 0)
            break;

        // Receive reply from server
        memset(message, 0, SIZE);

        if (recv(sock, message, SIZE, 0) <= 0)
            break;

        printf("Server: %s\n", message);

        if (strcmp(message, "exit") == 0)
            break;
    }

    close(sock);

    return 0;
}
