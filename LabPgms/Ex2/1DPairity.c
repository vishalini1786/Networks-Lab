#include <stdio.h>

int main()
{
    int choice, n, i;
    int data[20], received[20];
    int count = 0, parity;

    printf("1D PARITY\n");

    printf("\nChoose Parity");
    printf("\n1. Even Parity");
    printf("\n2. Odd Parity");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    printf("\nEnter the number of data bits: ");
    scanf("%d", &n);

    printf("Enter the data bits:\n");
    for(i = 0; i < n; i++)
    {
        scanf("%d", &data[i]);

        if(data[i] == 1)
            count++;
    }

    /* Sender Side */

    printf("\n----- Sender Side -----");

    printf("\nData Bits : ");
    for(i = 0; i < n; i++)
        printf("%d", data[i]);

    printf("\nNumber of 1's = %d", count);

    if(choice == 1)
    {
        if(count % 2 == 0)
            parity = 0;
        else
            parity = 1;

        printf("\nEven Parity Selected");
    }
    else
    {
        if(count % 2 == 0)
            parity = 1;
        else
            parity = 0;

        printf("\nOdd Parity Selected");
    }

    printf("\nParity Bit = %d", parity);

    printf("\nTransmitted Data = ");
    for(i = 0; i < n; i++)
        printf("%d", data[i]);
    printf("%d", parity);

    /* Receiver Side */

    count = 0;

    printf("\n\n----- Receiver Side -----");

    printf("\nEnter the received data (including parity bit):\n");

    for(i = 0; i <= n; i++)
    {
        scanf("%d", &received[i]);

        if(received[i] == 1)
            count++;
    }

    printf("\nReceived Data = ");
    for(i = 0; i <= n; i++)
        printf("%d", received[i]);

    printf("\nNumber of 1's = %d", count);

    if(choice == 1)
    {
        if(count % 2 == 0)
            printf("\nNo Error Detected");
        else
            printf("\nError Detected");
    }
    else
    {
        if(count % 2 != 0)
            printf("\nNo Error Detected");
        else
            printf("\nError Detected");
    }

    return 0;
}
