#include <stdio.h>
#include <string.h>

int main()
{
    char data[100];
    char stuffed[200];
    char received[200];
    char recovered[100];

    int i, j = 0, k = 0;
    int count = 0;

    printf("HDLC FRAMING USING BIT STUFFING\n");

    printf("\nEnter Binary Data: ");
    scanf("%s", data);

    /* Sender Side */

    strcpy(stuffed, "01111110");
    j = strlen(stuffed);

    count = 0;

    for(i = 0; data[i] != '\0'; i++)
    {
        stuffed[j++] = data[i];

        if(data[i] == '1')
        {
            count++;

            if(count == 5)
            {
                stuffed[j++] = '0';
                count = 0;
            }
        }
        else
        {
            count = 0;
        }
    }

    strcpy(&stuffed[j], "01111110");

    printf("\n----- Sender Side -----");

    printf("\nOriginal Data    : %s", data);
    printf("\nStuffed Frame    : %s", stuffed);
    printf("\nTransmitted Frame: %s", stuffed);

    /* Receiver Side */

    printf("\n\n----- Receiver Side -----");

    printf("\nEnter Received Frame: ");
    scanf("%s", received);

    printf("\nReceived Frame   : %s", received);

    if(strcmp(stuffed, received) == 0)
        printf("\nTransmission Status : NO ERROR");
    else
        printf("\nTransmission Status : ERROR DETECTED");

    /* Bit De-stuffing */

    count = 0;
    k = 0;

    for(i = 8; i < strlen(received) - 8; i++)
    {
        if(received[i] == '1')
        {
            count++;
            recovered[k++] = received[i];

            if(count == 5)
            {
                i++;        // Skip stuffed 0
                count = 0;
            }
        }
        else
        {
            count = 0;
            recovered[k++] = received[i];
        }
    }

    recovered[k] = '\0';

    printf("\nRecovered Data   : %s", recovered);

    return 0;
}
