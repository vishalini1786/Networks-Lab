#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8053
#define SIZE 1024

int main()
{
    int sock;
    char domain[SIZE];
    char ip[SIZE];

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

    printf("DNS Server started on port %d...\n", PORT);

    while (1)
    {
        // Receive domain name from client
        recvfrom(sock, domain, sizeof(domain), 0,
                 (struct sockaddr *)&client, &len);

        printf("Domain requested: %s\n", domain);

        // Search domain in DNS table
        if (strcmp(domain, "google.com") == 0)
            strcpy(ip, "142.250.195.14");

        else if (strcmp(domain, "facebook.com") == 0)
            strcpy(ip, "157.240.241.35");

        else if (strcmp(domain, "youtube.com") == 0)
            strcpy(ip, "142.250.196.46");

        else if (strcmp(domain, "amazon.com") == 0)
            strcpy(ip, "205.251.242.103");

        else if (strcmp(domain, "localhost") == 0)
            strcpy(ip, "127.0.0.1");

        else
            strcpy(ip, "Domain not found");

        // Send IP address to client
        sendto(sock, ip, strlen(ip) + 1, 0,
               (struct sockaddr *)&client, len);
    }

    close(sock);
    return 0;
}
