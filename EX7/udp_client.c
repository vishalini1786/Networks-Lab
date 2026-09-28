#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define SIZE 1024

int main()
{
    int sock, choice;
    char user[50], pass[50];
    char str1[SIZE], str2[SIZE], buffer[SIZE];

    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    // Set server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Enter login details
    printf("Username: ");
    scanf("%s", user);

    printf("Password: ");
    scanf("%s", pass);

    // Send login details
    sendto(sock, user, strlen(user) + 1, 0, (struct sockaddr *)&server, len);
    sendto(sock, pass, strlen(pass) + 1, 0, (struct sockaddr *)&server, len);

    // Receive authentication result
    recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &len);

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
        printf("\n1. String Concatenation\n");
        printf("2. String Reverse\n");
        printf("3. Uppercase to Lowercase\n");
        printf("4. Lowercase to Uppercase\n");
        printf("5. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        // Send choice to server
        sprintf(buffer, "%d", choice);
        sendto(sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&server, len);

        // Concatenate strings
        if (choice == 1)
        {
            printf("Enter first string: ");
            scanf(" %[^\n]", str1);

            printf("Enter second string: ");
            scanf(" %[^\n]", str2);

            sendto(sock, str1, strlen(str1) + 1, 0, (struct sockaddr *)&server, len);
            sendto(sock, str2, strlen(str2) + 1, 0, (struct sockaddr *)&server, len);

            recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &len);

            printf("Result: %s\n", buffer);
        }

        // Reverse string
        else if (choice == 2)
        {
            printf("Enter string: ");
            scanf(" %[^\n]", str1);

            sendto(sock, str1, strlen(str1) + 1, 0, (struct sockaddr *)&server, len);

            recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &len);

            printf("Result: %s\n", buffer);
        }

        // Uppercase to lowercase
        else if (choice == 3)
        {
            printf("Enter string: ");
            scanf(" %[^\n]", str1);

            sendto(sock, str1, strlen(str1) + 1, 0, (struct sockaddr *)&server, len);

            recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &len);

            printf("Result: %s\n", buffer);
        }

        // Lowercase to uppercase
        else if (choice == 4)
        {
            printf("Enter string: ");
            scanf(" %[^\n]", str1);

            sendto(sock, str1, strlen(str1) + 1, 0, (struct sockaddr *)&server, len);

            recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &len);

            printf("Result: %s\n", buffer);
        }

        // Exit
        else if (choice == 5)
        {
            recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &len);

            printf("%s\n", buffer);
            break;
        }

        // Invalid choice
        else
        {
            recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &len);

            printf("%s\n", buffer);
        }
    }

    // Close socket
    close(sock);

    return 0;
}
