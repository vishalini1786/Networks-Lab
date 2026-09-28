#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main()
{
    int serverSocket, clientSocket;
    char message[SIZE];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    pid_t pid;

    // Create TCP socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    // Set server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind failed");
        close(serverSocket);
        return 1;
    }

    // Listen for clients
    listen(serverSocket, 5);

    printf("Chat server started...\n");

    while (1)
    {
        // Accept client
        clientSocket = accept(serverSocket, (struct sockaddr *)&client, &len);

        if (clientSocket < 0)
        {
            perror("Accept failed");
            continue;
        }

        printf("Client connected.\n");

        // Create child process for client
        pid = fork();

        if (pid == 0)
        {
            close(serverSocket);

            while (1)
            {
                memset(message, 0, SIZE);

                // Receive message from client
                if (recv(clientSocket, message, SIZE, 0) <= 0)
                    break;

                printf("Client: %s\n", message);

                // Send reply to client
                printf("Server: ");
                fgets(message, SIZE, stdin);
                message[strcspn(message, "\n")] = '\0';

                send(clientSocket, message, strlen(message) + 1, 0);

                if (strcmp(message, "exit") == 0)
                    break;
            }

            close(clientSocket);
            exit(0);
        }
        else
        {
            close(clientSocket);
        }
    }

    close(serverSocket);

    return 0;
}
