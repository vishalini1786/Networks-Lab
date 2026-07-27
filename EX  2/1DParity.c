#include <stdio.h>
#include <string.h>

int main()
{
    char inputString[100];
    int choice, length, errorChoice;

    // Arrays adjusted for an 8-bit total frame size
    int senderDataBits[100][7]; // 7 bits for data
    int senderParity[100];      // 1 bit for parity
    int receivedFrame[8];       // Total 8 bits received
    char finalOutput[100];

    int transmissionFailed = 0;

    printf("1D PARITY STRING TRANSMISSION SYSTEM (8-BIT FRAMES)\n");

    printf("\nEnter the input string: ");
    fgets(inputString, sizeof(inputString), stdin);
    inputString[strcspn(inputString, "\n")] = '\0';
    length = strlen(inputString);

    printf("Choose Parity:\n1. Even Parity\n2. Odd Parity\nEnter your choice: ");
    scanf("%d", &choice);

    /* --- SENDER SIDE --- */
    printf("\n----- SENDER SIDE -----");
    printf("\nInput String: %s", inputString);
    printf("\nTotal Characters: %d\n", length);

    for (int i = 0; i < length; i++)
    {
        char ch = inputString[i];
        int asciiValue = (int)ch & 0x7F; // Masking to guarantee standard 7-bit ASCII
        int countOnes = 0;

        printf("\nFrame %d:", i + 1);
        printf("\n  Character : '%c'", ch);
        printf("\n  ASCII     : %d", asciiValue);
        printf("\n  Data Bits (7 bits): ");

        // Convert ASCII to 7-bit binary (MSB to LSB: index 6 down to 0)
        int tempAscii = asciiValue;
        for (int j = 6; j >= 0; j--)
        {
            senderDataBits[i][j] = tempAscii % 2;
            tempAscii = tempAscii / 2;
        }

        for (int j = 0; j < 7; j++)
        {
            printf("%d ", senderDataBits[i][j]);
            if (senderDataBits[i][j] == 1) countOnes++;
        }
        printf("\n  Count of 1s in Data: %d", countOnes);

        // Generate parity bit based on the 7 data bits
        if (choice == 1) // Even Parity
            senderParity[i] = (countOnes % 2 == 0) ? 0 : 1;
        else // Odd Parity
            senderParity[i] = (countOnes % 2 == 0) ? 1 : 0;

        printf("\n  Parity Bit: %d", senderParity[i]);
        printf("\n  Transmitted Frame (8 bits): ");
        for (int j = 0; j < 7; j++) printf("%d", senderDataBits[i][j]);
        printf("%d\n", senderParity[i]);
    }

    /* --- RECEIVER SIDE --- */
    printf("\n----- RECEIVER SIDE -----\n");

    for (int i = 0; i < length; i++)
    {
        int countOnes = 0;
        int bitMismatch = 0;
        int pos1, pos2;

        // Clone the 7 data bits and 1 parity bit into the 8-bit receiver array
        for (int j = 0; j < 7; j++) {
            receivedFrame[j] = senderDataBits[i][j];
        }
        receivedFrame[7] = senderParity[i];

        printf("----------------------------------------\n");
        printf("Configuring Error for Frame %d (Character: '%c')\n", i + 1, inputString[i]);
        printf("0. No Error\n1. 1-Bit Error\n2. Multi-bit Error (2 Bits)\nEnter your choice for this frame: ");
        scanf("%d", &errorChoice);

        if (errorChoice == 1) {
            printf("Enter bit position to flip (0 to 7): ");
            scanf("%d", &pos1);
            if (pos1 >= 0 && pos1 <= 7) {
                receivedFrame[pos1] = 1 - receivedFrame[pos1];
            } else {
                printf("Invalid position! No error induced.\n");
            }
        }
        else if (errorChoice == 2) {
            printf("Enter first bit position to flip (0 to 7): ");
            scanf("%d", &pos1);
            printf("Enter second bit position to flip (0 to 7): ");
            scanf("%d", &pos2);
            if (pos1 >= 0 && pos1 <= 7 && pos2 >= 0 && pos2 <= 7) {
                receivedFrame[pos1] = 1 - receivedFrame[pos1];
                receivedFrame[pos2] = 1 - receivedFrame[pos2];
            } else {
                printf("Invalid positions! No errors induced.\n");
            }
        }

        printf("\nProcessing Frame %d\n", i + 1);
        printf("  Received Frame: ");
        for (int j = 0; j < 8; j++) {
            printf("%d ", receivedFrame[j]);
            if (receivedFrame[j] == 1) countOnes++;

            // Validate bit configuration across the 8 items
            if (j < 7) {
                if (receivedFrame[j] != senderDataBits[i][j]) bitMismatch = 1;
            } else {
                if (receivedFrame[j] != senderParity[i]) bitMismatch = 1;
            }
        }
        printf("\n  Total Count of 1s (including parity): %d", countOnes);

        int parityError = 0;
        if (choice == 1)
            parityError = (countOnes % 2 != 0) ? 1 : 0;
        else
            parityError = (countOnes % 2 == 0) ? 1 : 0;

        if (parityError || bitMismatch)
        {
            printf("\n  Result        : ERROR DETECTED!\n\n");
            transmissionFailed = 1;
        }
        else
        {
            printf("\n  Result        : No Error Detected.\n\n");

            // Rebuild the 7-bit data block back to an ASCII character
            int asciiRecovered = 0;
            for (int j = 0; j < 7; j++) asciiRecovered = (asciiRecovered * 2) + receivedFrame[j];
            finalOutput[i] = (char)asciiRecovered;
        }
    }

    finalOutput[length] = '\0';

    /* --- FINAL OUTPUT --- */
    printf("----- FINAL RECONSTRUCTED DATA -----\n");
    if (transmissionFailed == 1)
    {
        printf("Result: Error Detected! Whole Frame/Data Discarded.\n");
    }
    else
    {
        printf("Successfully Decoded String: %s\n", finalOutput);
    }

    return 0;
}
