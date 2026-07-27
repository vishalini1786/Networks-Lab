#include <stdio.h>
#include <string.h>

// Core CRC Division logic
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
    char inputString[100];
    char generator[50];
    char finalOutput[100];
    
    char frameBinary[9]; 
    char paddedData[150];
    char crc[50];
    char transmittedCodeword[150];
    char remainder[50];

    int choice, position, length;
    int transmissionFailed = 0;

    printf("===== CRC STRING ERROR DETECTION (8-BIT DATA) =====\n\n");

    printf("Enter the input string: ");
    fgets(inputString, sizeof(inputString), stdin);
    inputString[strcspn(inputString, "\n")] = '\0'; 
    length = strlen(inputString);

    printf("Enter Generator (Binary e.g., 1011): ");
    scanf("%s", generator);

    int dataBitsCount = 8;                   // Modified to full 8-bit data format
    int generatorLength = strlen(generator);
    int crcBitsCount = generatorLength - 1;  
    int totalCodewordBits = dataBitsCount + crcBitsCount;

    /* ================= SENDER SIDE ================= */
    printf("\n----- SENDER SIDE -----\n");
    printf("Input String     : %s\n", inputString);
    printf("Total Characters : %d\n", length);
    printf("Number of Data Bits per Frame : %d\n", dataBitsCount);
    printf("Number of CRC Bits per Frame  : %d\n", crcBitsCount);
    printf("Total Bits per Codeword       : %d\n", totalCodewordBits);

    char savedCodewords[100][150];

    for (int i = 0; i < length; i++)
    {
        char ch = inputString[i];
        int asciiValue = (int)ch;

        // Convert character ASCII to full 8-bit binary string (index 7 down to 0)
        int tempAscii = asciiValue;
        for (int j = 7; j >= 0; j--)
        {
            frameBinary[j] = (tempAscii % 2) + '0';
            tempAscii = tempAscii / 2;
        }
        frameBinary[8] = '\0';

        // Pad data with zeros
        strcpy(paddedData, frameBinary);
        for (int j = 0; j < crcBitsCount; j++)
            paddedData[8 + j] = '0';
        paddedData[8 + crcBitsCount] = '\0';

        // Compute CRC remainder bits
        crcDivision(paddedData, generator, crc);

        // Form final transmission Codeword (8-bit Data + CRC)
        strcpy(transmittedCodeword, frameBinary);
        strcat(transmittedCodeword, crc);
        
        // Save codeword for receiver validation phase
        strcpy(savedCodewords[i], transmittedCodeword);

        printf("\nFrame %d (Character '%c'):\n", i + 1, ch);
        printf("  %d Data Bits    : %s\n", dataBitsCount, frameBinary);
        printf("  Padded Framework : %s\n", paddedData);
        printf("  %d CRC Bits     : %s\n", crcBitsCount, crc);
        printf("  Final Codeword   : %s\n", transmittedCodeword);
    }

    /* ================= RECEIVER SIDE ================= */
    printf("\n----- RECEIVER SIDE -----\n");

    for (int i = 0; i < length; i++)
    {
        char receivedData[150];
        strcpy(receivedData, savedCodewords[i]);

        printf("----------------------------------------\n");
        printf("Configuring Error for Frame %d (Character: '%c')\n", i + 1, inputString[i]);
        printf("0. No Error\n");
        printf("1. Single Bit Error\n");
        printf("2. Two Bit Error\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            printf("Enter Bit Position to flip (1-%d): ", totalCodewordBits);
            scanf("%d", &position);

            if (position >= 1 && position <= totalCodewordBits)
            {
                receivedData[position - 1] = (receivedData[position - 1] == '0') ? '1' : '0';
            }
        }
        else if (choice == 2)
        {
            printf("Enter Starting Position to flip 2 bits (1-%d): ", totalCodewordBits - 1);
            scanf("%d", &position);

            if (position >= 1 && position < totalCodewordBits)
            {
                receivedData[position - 1] = (receivedData[position - 1] == '0') ? '1' : '0';
                receivedData[position] = (receivedData[position] == '0') ? '1' : '0';
            }
        }

        printf("\nProcessing Frame %d\n", i + 1);
        printf("  Received Data : %s (%d bits)\n", receivedData, totalCodewordBits);

        // Run validation division
        crcDivision(receivedData, generator, remainder);
        printf("  Remainder     : %s (%d CRC bits checked)\n", remainder, crcBitsCount);

        int frameError = 0;
        for (int j = 0; j < crcBitsCount; j++)
        {
            if (remainder[j] != '0')
            {
                frameError = 1;
                break;
            }
        }

        if (frameError)
        {
            printf("  Result        : ERROR DETECTED\n");
            transmissionFailed = 1;
        }
        else
        {
            printf("  Result        : NO ERROR DETECTED\n");
            
            // Rebuild character using all 8 bits from the start of the codeword
            int asciiRecovered = 0;
            for (int j = 0; j < 8; j++)
            {
                asciiRecovered = (asciiRecovered * 2) + (receivedData[j] - '0');
            }
            finalOutput[i] = (char)asciiRecovered;
        }
    }

    finalOutput[length] = '\0';

    /* ================= FINAL OUTPUT ================= */
    printf("\n----- FINAL RECONSTRUCTED DATA -----\n");
    if (transmissionFailed)
    {
        printf("Result: Error Detected! Whole Frame/Data Discarded.\n");
    }
    else
    {
        printf("Successfully Decoded String: %s\n", finalOutput);
    }

    return 0;
}
