#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

// Check username and password
int authenticate(char user[], char pass[])
{
    FILE *fp;
    char u[50], p[50];

    fp = fopen("users.txt", "r");

    if (fp == NULL)
        return 0;

    while (fscanf(fp, "%s %s", u, p) != EOF)
    {
        if (strcmp(user, u) == 0 && strcmp(pass, p) == 0)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// Calculate factorial
int factorial(int n)
{
    int result = 1, i;

    for (i = 1; i <= n; i++)
        result = result * i;

    return result;
}

int main()
{
    int serverSocket, clientSocket;
    int choice, number, result;
    char user[50], pass[50], buffer[SIZE];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

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

    // Bind socket to port
    if (bind(serverSocket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind failed");
        close(serverSocket);
        return 1;
    }

    // Listen for clients
    if (listen(serverSocket, 5) < 0)
    {
        perror("Listen failed");
        close(serverSocket);
        return 1;
    }

    printf("Server waiting for client...\n");

    // Accept client
    clientSocket = accept(serverSocket, (struct sockaddr *)&client, &len);

    if (clientSocket < 0)
    {
        perror("Accept failed");
        close(serverSocket);
        return 1;
    }

    printf("Client connected.\n");

    // Receive login details
    recv(clientSocket, user, sizeof(user), 0);
    recv(clientSocket, pass, sizeof(pass), 0);

    // Check authentication
    if (authenticate(user, pass))
    {
        strcpy(buffer, "SUCCESS");
        send(clientSocket, buffer, strlen(buffer) + 1, 0);
        printf("Login successful.\n");
    }
    else
    {
        strcpy(buffer, "FAILED");
        send(clientSocket, buffer, strlen(buffer) + 1, 0);
        printf("Login failed.\n");

        close(clientSocket);
        close(serverSocket);
        return 0;
    }

    // Process client requests
    while (1)
    {
        recv(clientSocket, buffer, sizeof(buffer), 0);
        choice = atoi(buffer);

        // Echo
        if (choice == 1)
        {
            recv(clientSocket, buffer, sizeof(buffer), 0);
            printf("Message: %s\n", buffer);

            send(clientSocket, buffer, strlen(buffer) + 1, 0);
        }

        // Factorial
        else if (choice == 2)
        {
            recv(clientSocket, buffer, sizeof(buffer), 0);
            number = atoi(buffer);

            if (number < 0 || number > 12)
                strcpy(buffer, "Enter number between 0 and 12");
            else
            {
                result = factorial(number);
                sprintf(buffer, "Factorial = %d", result);
            }

            send(clientSocket, buffer, strlen(buffer) + 1, 0);
        }

        // Exit
        else if (choice == 3)
        {
            strcpy(buffer, "Goodbye");

            send(clientSocket, buffer, strlen(buffer) + 1, 0);
            break;
        }

        // Invalid choice
        else
        {
            strcpy(buffer, "Invalid choice");
            send(clientSocket, buffer, strlen(buffer) + 1, 0);
        }
    }

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    printf("Server closed.\n");

    return 0;
}
