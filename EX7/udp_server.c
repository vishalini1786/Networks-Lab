#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8081
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

// Reverse the string
void reverse(char str[])
{
    int i, j;
    char temp;

    j = strlen(str) - 1;

    for (i = 0; i < j; i++, j--)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int main()
{
    int sock, choice, i;
    char user[50], pass[50];
    char str1[SIZE], str2[SIZE], buffer[SIZE];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    // Set server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind failed");
        close(sock);
        return 1;
    }

    printf("UDP Server waiting on port %d...\n", PORT);

    // Receive login details
    recvfrom(sock, user, sizeof(user), 0, (struct sockaddr *)&client, &len);
    recvfrom(sock, pass, sizeof(pass), 0, (struct sockaddr *)&client, &len);

    // Check authentication
    if (authenticate(user, pass))
    {
        strcpy(buffer, "SUCCESS");
        sendto(sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&client, len);
        printf("Authentication successful.\n");
    }
    else
    {
        strcpy(buffer, "FAILED");
        sendto(sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&client, len);
        printf("Authentication failed.\n");

        close(sock);
        return 0;
    }

    // Process client requests
    while (1)
    {
        recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &len);
        choice = atoi(buffer);

        // Concatenate two strings
        if (choice == 1)
        {
            recvfrom(sock, str1, sizeof(str1), 0, (struct sockaddr *)&client, &len);
            recvfrom(sock, str2, sizeof(str2), 0, (struct sockaddr *)&client, &len);

            strcat(str1, str2);

            sendto(sock, str1, strlen(str1) + 1, 0, (struct sockaddr *)&client, len);
        }

        // Reverse string
        else if (choice == 2)
        {
            recvfrom(sock, str1, sizeof(str1), 0, (struct sockaddr *)&client, &len);

            reverse(str1);

            sendto(sock, str1, strlen(str1) + 1, 0, (struct sockaddr *)&client, len);
        }

        // Convert uppercase to lowercase
        else if (choice == 3)
        {
            recvfrom(sock, str1, sizeof(str1), 0, (struct sockaddr *)&client, &len);

            for (i = 0; str1[i] != '\0'; i++)
                str1[i] = tolower(str1[i]);

            sendto(sock, str1, strlen(str1) + 1, 0, (struct sockaddr *)&client, len);
        }

        // Convert lowercase to uppercase
        else if (choice == 4)
        {
            recvfrom(sock, str1, sizeof(str1), 0, (struct sockaddr *)&client, &len);

            for (i = 0; str1[i] != '\0'; i++)
                str1[i] = toupper(str1[i]);

            sendto(sock, str1, strlen(str1) + 1, 0, (struct sockaddr *)&client, len);
        }

        // Exit
        else if (choice == 5)
        {
            strcpy(buffer, "Goodbye");

            sendto(sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&client, len);
            break;
        }

        // Invalid choice
        else
        {
            strcpy(buffer, "Invalid choice");

            sendto(sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&client, len);
        }
    }

    // Close socket
    close(sock);

    printf("Server closed.\n");

    return 0;
}
