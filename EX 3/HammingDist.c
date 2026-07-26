#include <stdio.h>
#include <math.h>

int main() {
    int parityType, dataSize;

    printf("===============================\n");
    printf("  HAMMING CODE (RIGHT TO LEFT) \n");
    printf("===============================\n\n");
    printf("1. Even Parity\n2. Odd Parity\nChoice: ");
    scanf("%d", &parityType);

    printf("Enter number of data bits: ");
    scanf("%d", &dataSize);

    int dataBits[dataSize + 1];

    // Read binary data bits (Left to Right input)
    printf("Enter %d bits: ", dataSize);
    for (int i = 1; i <= dataSize; i++) {
        scanf("%d", &dataBits[i]);
    }

    // Step 1: Find parity bits needed (r)
    int parityCount = 0;
    while (pow(2, parityCount) < (dataSize + parityCount + 1)) {
        parityCount++;
    }

    int totalBits = dataSize + parityCount;
    int codeWord[totalBits + 1];
    int receivedCode[totalBits + 1];

    // Step 2: Place data bits (Filling from Right to Left: Position 1 is at Far Right)
    // dataBits[1] goes to leftmost data slot, dataBits[dataSize] to rightmost data slot
    int dataIndex = dataSize;
    int parityPower = 0;

    for (int pos = 1; pos <= totalBits; pos++) {
        if (pos == (int)pow(2, parityPower)) {
            codeWord[pos] = 0; // Parity placeholder at pos 1, 2, 4, 8...
            parityPower++;
        } else {
            codeWord[pos] = dataBits[dataIndex--];
        }
    }

    // Step 3: Calculate parity bits on sender side
    printf("\n========== SENDER SIDE ==========\n");

    for (int i = 0; i < parityCount; i++) {
        int parityPos = (int)pow(2, i);
        int sumBits = 0;

        for (int j = parityPos; j <= totalBits; j += 2 * parityPos) {
            for (int k = j; k < j + parityPos && k <= totalBits; k++) {
                sumBits += codeWord[k];
            }
        }

        if (parityType == 1) {
            codeWord[parityPos] = sumBits % 2;
        } else {
            codeWord[parityPos] = (sumBits + 1) % 2;
        }

        printf("Parity Bit P%d (at Pos %d) = %d\n", parityPos, parityPos, codeWord[parityPos]);
    }

    // Print generated Hamming code word (From Position totalBits down to Position 1)
    printf("\nGenerated Code Word (Left to Right = Pos %d to 1): ", totalBits);
    for (int pos = totalBits; pos >= 1; pos--) {
        printf("%d", codeWord[pos]);
    }
    printf("\n");

    // Copy transmitted code to receiver array
    for (int pos = 1; pos <= totalBits; pos++) {
        receivedCode[pos] = codeWord[pos];
    }

    // Step 4: RECEIVER SIDE - Error Induction Choice Menu
    printf("\n========== RECEIVER SIDE ==========\n");
    printf("Induce Error Options:\n");
    printf("0. No Error\n");
    printf("1. Single Bit Error\n");
    printf("2. 2-Bit Burst Error\n");
    
    int choice, errorPos;
    printf("Choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter bit position from right (1 to %d): ", totalBits);
        scanf("%d", &errorPos);

        if (errorPos >= 1 && errorPos <= totalBits) {
            receivedCode[errorPos] = (receivedCode[errorPos] == 0) ? 1 : 0;
        }
    } else if (choice == 2) {
        printf("Enter starting bit position from right for 2-bit burst (1 to %d): ", totalBits - 1);
        scanf("%d", &errorPos);

        if (errorPos >= 1 && errorPos < totalBits) {
            receivedCode[errorPos]     = (receivedCode[errorPos]     == 0) ? 1 : 0;
            receivedCode[errorPos + 1] = (receivedCode[errorPos + 1] == 0) ? 1 : 0;
        }
    }

    printf("Received Code Word : ");
    for (int pos = totalBits; pos >= 1; pos--) {
        printf("%d", receivedCode[pos]);
    }
    printf("\n\n--- Receiver Parity Checks ---\n");

    // Step 5: Check parity bits on receiver side and calculate Syndrome Value
    int syndromeBits[parityCount];
    int calculatedError = 0;

    for (int i = 0; i < parityCount; i++) {
        int parityPos = (int)pow(2, i);
        int sumBits = 0;

        for (int j = parityPos; j <= totalBits; j += 2 * parityPos) {
            for (int k = j; k < j + parityPos && k <= totalBits; k++) {
                sumBits += receivedCode[k];
            }
        }

        int parityFailed = (parityType == 1) ? (sumBits % 2 != 0) : (sumBits % 2 == 0);
        syndromeBits[i] = parityFailed;

        if (parityFailed) {
            calculatedError += parityPos;
        }

        printf("P%d Check Result = %d\n", parityPos, syndromeBits[i]);
    }

    // Step 6: Display Syndrome Value
    printf("\nSyndrome Value (Binary)  : ");
    for (int i = parityCount - 1; i >= 0; i--) {
        printf("%d", syndromeBits[i]);
    }
    printf("\nSyndrome Value (Decimal) : %d\n", calculatedError);

    // Step 7: Correct Single Bit Error or Discard
    if (calculatedError == 0) {
        printf("\nResult: NO ERROR DETECTED! (Frame Accepted)\n");

        printf("Original Data Recovered  : ");
        parityPower = 0;

        for (int pos = totalBits; pos >= 1; pos--) {
            if (pos == (int)pow(2, parityPower)) {
                parityPower++;
            } else {
                printf("%d", receivedCode[pos]);
            }
        }
        printf("\n\n");
    } else {
        if (choice == 2) {
            printf("\nResult: MULTI-BIT BURST ERROR DETECTED! (Frame Discarded - Cannot Correct)\n\n");
        } else {
            printf("\nError Position Identified : %d (from right)\n", calculatedError);

            // Correct bit
            receivedCode[calculatedError] = (receivedCode[calculatedError] == 0) ? 1 : 0;

            printf("Corrected Code Word       : ");
            for (int pos = totalBits; pos >= 1; pos--) {
                printf("%d", receivedCode[pos]);
            }
            printf("\n");

            // Extract recovered data
            printf("Original Data Recovered   : ");
            
            // Re-read non-parity positions from Left to Right
            for (int pos = totalBits; pos >= 1; pos--) {
                // Check if pos is a power of 2
                if ((pos & (pos - 1)) != 0) {
                    printf("%d", receivedCode[pos]);
                }
            }
            printf("\n\n");
        }
    }

    return 0;
}
