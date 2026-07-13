#include <stdio.h>
#include <string.h>

int main()
{
    char data[100], stuffed[150];
    int i, j = 0, count = 0;

    printf("Enter Binary Data: ");
    scanf("%s", data);

    // Bit Stuffing
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

    stuffed[j] = '\0';

    printf("\nOriginal Data : %s", data);
    printf("\nStuffed Data  : %s", stuffed);

    printf("\n\nTransmitted Frame : ");
    printf("01111110%s01111110\n", stuffed);

    // Receiver Side
    char destuffed[100];
    count = 0;
    j = 0;

    for(i = 0; stuffed[i] != '\0'; i++)
    {
        destuffed[j++] = stuffed[i];

        if(stuffed[i] == '1')
        {
            count++;

            if(count == 5)
            {
                i++;      // Skip stuffed 0
                count = 0;
            }
        }
        else
        {
            count = 0;
        }
    }

    destuffed[j] = '\0';

    printf("\nReceiver Output : %s", destuffed);

    return 0;
}
