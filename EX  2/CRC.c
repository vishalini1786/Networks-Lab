#include <stdio.h>
#include <string.h>

void crcDivision(char data[], char generator[], char remainder[])
{
    char temp[200];
    int dataLength = strlen(data);
    int generatorLength = strlen(generator);

    strcpy(temp, data);

    for (int i = 0; i <= dataLength - generatorLength; i++)
    {
        if (temp[i] == '1')
        {
            for (int j = 0; j < generatorLength; j++)
            {
                if (temp[i + j] == generator[j])
                    temp[i + j] = '0';
                else
                    temp[i + j] = '1';
            }
        }
    }

    for (int i = 0; i < generatorLength - 1; i++)
        remainder[i] = temp[dataLength - generatorLength + 1 + i];

    remainder[generatorLength - 1] = '\0';
}

int main()
{
    char data[100], generator[50];
    char paddedData[150], crc[50];
    char receivedData[150], remainder[50];

    int choice, position;

    printf("===== CRC ERROR DETECTION =====\n\n");

    printf("Enter Binary Data : ");
    scanf("%s", data);

    printf("Enter Generator   : ");
    scanf("%s", generator);

    int dataLength = strlen(data);
    int generatorLength = strlen(generator);
    int zeros = generatorLength - 1;

    strcpy(paddedData, data);

    for (int i = 0; i < zeros; i++)
        paddedData[dataLength + i] = '0';

    paddedData[dataLength + zeros] = '\0';

    crcDivision(paddedData, generator, crc);

    printf("\nSender Side\n");
    printf("Data with Zeros : %s\n", paddedData);
    printf("CRC            : %s\n", crc);

    strcpy(receivedData, data);
    strcat(receivedData, crc);

    printf("Codeword       : %s\n", receivedData);

    printf("\nReceiver Side\n");
    printf("0. No Error\n");
    printf("1. Single Bit Error\n");
    printf("2. Two Bit Error\n");

    printf("Enter Choice : ");
    scanf("%d", &choice);

    int length = strlen(receivedData);

    if (choice == 1)
    {
        printf("Enter Bit Position (1-%d): ", length);
        scanf("%d", &position);

        if (position >= 1 && position <= length)
        {
            if (receivedData[position - 1] == '0')
                receivedData[position - 1] = '1';
            else
                receivedData[position - 1] = '0';
        }
    }
    else if (choice == 2)
    {
        printf("Enter Starting Position (1-%d): ", length - 1);
        scanf("%d", &position);

        if (position >= 1 && position < length)
        {
            if (receivedData[position - 1] == '0')
                receivedData[position - 1] = '1';
            else
                receivedData[position - 1] = '0';

            if (receivedData[position] == '0')
                receivedData[position] = '1';
            else
                receivedData[position] = '0';
        }
    }

    printf("\nReceived Data : %s\n", receivedData);

    crcDivision(receivedData, generator, remainder);

    printf("Remainder     : %s\n", remainder);

    int error = 0;

    for (int i = 0; i < zeros; i++)
    {
        if (remainder[i] != '0')
        {
            error = 1;
            break;
        }
    }

    if (error)
        printf("\nERROR DETECTED\n");
    else
        printf("\nNO ERROR DETECTED\n");

    return 0;
}
