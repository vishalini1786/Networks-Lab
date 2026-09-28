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

    while (1)
    {
        printf("\nEnter domain name: ");
        scanf("%s", domain);

        // Exit the client
        if (strcmp(domain, "exit") == 0)
            break;

        // Send domain to DNS server
        sendto(sock, domain, strlen(domain) + 1, 0,
               (struct sockaddr *)&server, len);

        // Receive IP address
        recvfrom(sock, ip, sizeof(ip), 0,
                 (struct sockaddr *)&server, &len);

        printf("IP Address: %s\n", ip);
    }

    // Close socket
    close(sock);

    return 0;
}
