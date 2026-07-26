#include <stdio.h>
#include <string.h>

int main() {
    char data[200];
    int parityType, choice, numErrors, pos;

    printf("===============================\n");
    printf("      1D PARITY CHECK CODE     \n");
    printf("===============================\n\n");

    // Select Parity Mode
    printf("1. Even Parity\n");
    printf("2. Odd Parity\n");
    printf("Choice: ");
    scanf("%d", &parityType);

    // Read input binary data
    printf("Enter binary data string : ");
    scanf("%s", data);

    int len = strlen(data);
    int onesCount = 0;

    // Count number of 1s in input data
    for (int i = 0; i < len; i++) {
        if (data[i] == '1') {
            onesCount++;
        }
    }

    // STEP 1: SENDER SIDE - Calculate Parity Bit
    char parityBit;

    if (parityType == 1) {
        // Even Parity: Make total 1s EVEN
        parityBit = (onesCount % 2 == 0) ? '0' : '1';
    } else {
        // Odd Parity: Make total 1s ODD
        parityBit = (onesCount % 2 != 0) ? '0' : '1';
    }

    printf("\n========== SENDER SIDE ==========\n");
    printf("Count of 1s in Data  : %d\n", onesCount);
    printf("Generated Parity Bit : %c\n", parityBit);

    // Build Transmitted Frame (Data + Parity Bit)
    char txFrame[200];
    strcpy(txFrame, data);
    txFrame[len] = parityBit;
    txFrame[len + 1] = '\0';

    printf("Transmitted Frame    : %s (Data: %s | Parity: %c)\n", txFrame, data, parityBit);

    // STEP 2: RECEIVER SIDE - Error Simulation Menu
    char rxFrame[200];
    strcpy(rxFrame, txFrame);

    int totalLen = strlen(rxFrame);

    printf("\n========== RECEIVER SIDE ==========\n");
    printf("Induce Error Options:\n");
    printf("0. No Error\n");
    printf("1. Single Bit Error\n");
    printf("2. Multi-Bit Error\n");
    printf("Choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter bit position to flip (1 to %d) : ", totalLen);
        scanf("%d", &pos);

        if (pos >= 1 && pos <= totalLen) {
            rxFrame[pos - 1] = (rxFrame[pos - 1] == '0') ? '1' : '0';
        }
    } else if (choice == 2) {
        printf("How many bits do you want to flip? (1 to %d) : ", totalLen);
        scanf("%d", &numErrors);

        for (int i = 1; i <= numErrors; i++) {
            printf("Enter position for error %d (1 to %d) : ", i, totalLen);
            scanf("%d", &pos);

            if (pos >= 1 && pos <= totalLen) {
                rxFrame[pos - 1] = (rxFrame[pos - 1] == '0') ? '1' : '0';
            }
        }
    }

    printf("\nReceived Frame       : %s\n", rxFrame);

    // STEP 3: RECEIVER SIDE - Check Parity
    int rxOnesCount = 0;
    for (int i = 0; i < totalLen; i++) {
        if (rxFrame[i] == '1') {
            rxOnesCount++;
        }
    }

    printf("Count of 1s in Receiver Frame : %d\n", rxOnesCount);

    int isError = 0;
    if (parityType == 1) {
        // Even Parity: If total 1s is ODD, error detected!
        if (rxOnesCount % 2 != 0) {
            isError = 1;
        }
    } else {
        // Odd Parity: If total 1s is EVEN, error detected!
        if (rxOnesCount % 2 == 0) {
            isError = 1;
        }
    }

    // STEP 4: DISPLAY STATUS
    if (isError == 0) {
        printf("\nResult: NO ERROR DETECTED! (Frame Accepted)\n");

        // Extract original data (all bits except last parity bit)
        char rxData[200];
        strncpy(rxData, rxFrame, len);
        rxData[len] = '\0';

        printf("Recovered Data      : %s\n\n", rxData);
    } else {
        printf("\nResult: ERROR DETECTED! (Frame Discarded)\n\n");
    }

    return 0;
}
