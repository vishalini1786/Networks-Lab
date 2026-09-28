#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main()
{
    int sock, choice, number;
    char user[50], pass[50], buffer[SIZE];

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
        close(sock);
        return 1;
    }

    printf("Connected to server.\n");

    // Enter login details
    printf("Username: ");
    scanf("%s", user);

    printf("Password: ");
    scanf("%s", pass);

    // Send login details
    send(sock, user, strlen(user) + 1, 0);
    send(sock, pass, strlen(pass) + 1, 0);

    // Receive authentication result
    recv(sock, buffer, sizeof(buffer), 0);

    if (strcmp(buffer, "FAILED") == 0)
    {
        printf("Authentication failed.\n");
        close(sock);
        return 0;
    }

    printf("Authentication successful.\n");

    // Display menu
    while (1)
    {
        printf("\n1. Echo\n");
        printf("2. Factorial\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        // Send choice to server
        sprintf(buffer, "%d", choice);
        send(sock, buffer, strlen(buffer) + 1, 0);

        // Echo
        if (choice == 1)
        {
            getchar();

            printf("Enter message: ");
            fgets(buffer, SIZE, stdin);

            buffer[strcspn(buffer, "\n")] = '\0';

            send(sock, buffer, strlen(buffer) + 1, 0);

            recv(sock, buffer, sizeof(buffer), 0);

            printf("Echo: %s\n", buffer);
        }

        // Factorial
        else if (choice == 2)
        {
            printf("Enter number: ");
            scanf("%d", &number);

            sprintf(buffer, "%d", number);
            send(sock, buffer, strlen(buffer) + 1, 0);

            recv(sock, buffer, sizeof(buffer), 0);

            printf("%s\n", buffer);
        }

        // Exit
        else if (choice == 3)
        {
            recv(sock, buffer, sizeof(buffer), 0);

            printf("%s\n", buffer);
            break;
        }

        // Invalid choice
        else
        {
            recv(sock, buffer, sizeof(buffer), 0);

            printf("%s\n", buffer);
        }
    }

    // Close socket
    close(sock);

    return 0;
}
