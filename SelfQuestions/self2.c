#include <stdio.h>
#include <string.h>

int main()
{
    char data[100];
    char stuffed[200];
    char received[200];
    char recovered[100];

    int i, j = 0, k = 0;

    printf("BYTE STUFFING\n");

    printf("\nSpecial Characters Used");
    printf("\nSOF : F");
    printf("\nEOF : E");
    printf("\nESC : /\n");

    printf("\nEnter Original Message: ");
    scanf("%s", data);

    /* Sender Side */

    stuffed[j++] = 'F';

    for(i = 0; data[i] != '\0'; i++)
    {
        if(data[i] == 'F' || data[i] == 'E' || data[i] == '/')
        {
            stuffed[j++] = '/';
        }

        stuffed[j++] = data[i];
    }

    stuffed[j++] = 'E';
    stuffed[j] = '\0';

    printf("\n----- Sender Side -----");

    printf("\nOriginal Message : %s", data);

    printf("\nStuffed Frame    : %s", stuffed);

    printf("\nTransmitted Frame: %s", stuffed);

    /* Receiver Side */

    printf("\n\n----- Receiver Side -----");

    printf("\nEnter Received Frame: ");
    scanf("%s", received);

    printf("\nReceived Frame   : %s", received);

    /* Error Detection */

    if(strcmp(stuffed, received) != 0)
        printf("\nTransmission Status : ERROR DETECTED");
    else
        printf("\nTransmission Status : NO ERROR");

    /* De-stuffing */

    k = 0;

    for(i = 1; received[i] != '\0' && received[i] != 'E'; i++)
    {
        if(received[i] == '/')
        {
            i++;
        }

        recovered[k++] = received[i];
    }

    recovered[k] = '\0';

    printf("\nRecovered Message: %s", recovered);

    return 0;
}
