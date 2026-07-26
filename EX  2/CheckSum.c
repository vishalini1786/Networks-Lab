
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to add two binary numbers using 1's complement addition
void addOneComp(char a[], char b[], char res[], int n) {
    int numA = strtol(a, NULL, 2);
    int numB = strtol(b, NULL, 2);

    int sum = numA + numB;
    int limit = (int)pow(2, n);

    // Wrap around carry (subtract limit and add 1)
    while (sum >= limit) {
        sum = sum - limit + 1;
    }

    // Convert decimal sum back into binary string
    for (int i = n - 1; i >= 0; i--) {
        res[i] = (sum % 2 == 1) ? '1' : '0';
        sum = sum / 2;
    }
    res[n] = '\0';
}

// Function to flip 0s to 1s and 1s to 0s for 1's complement
void getOnesComp(char in[], char comp[], int n) {
    for (int i = 0; i < n; i++) {
        comp[i] = (in[i] == '0') ? '1' : '0';
    }
    comp[n] = '\0';
}

int main() {
    char data[200], pad[200] = "";
    int n, ch, pos;

    printf("===============================\n");
    printf("        CHECKSUM PROGRAM       \n");
    printf("===============================\n\n");

    // Input data and block size

    printf("Enter frame data string : ");
    scanf("%s", data);

    printf("Enter block size (n)    : ");
    scanf("%d", &n);

    // Step 1: Prefix zeros if data is not divisible by n

    int len = strlen(data);
    int rem = len % n;
    int pLen = (rem != 0) ? (n - rem) : 0;

    for (int i = 0; i < pLen; i++) {
        pad[i] = '0';
    }
    pad[pLen] = '\0';
    strcat(pad, data);

    int totLen = strlen(pad);
    int numB = totLen / n;

    // Step 2: Extract blocks and print on sender side

    printf("\n========== SENDER SIDE ==========\n");

    if (pLen > 0) {
        printf("Padded Data (%d zeros added) : %s\n\n", pLen, pad);
    }

    char blk[numB][n + 1];

    for (int i = 0; i < numB; i++) {
        strncpy(blk[i], pad + (i * n), n);
        blk[i][n] = '\0';
        printf("Block %d : %s\n", i + 1, blk[i]);
    }

    // Step 3: Step-by-step addition on sender side

    char sum[n + 1];
    strcpy(sum, blk[0]);

    printf("\n--- Step-by-Step Addition ---\n");
    printf("Initial Sum (Block 1) : %s\n", sum);

    for (int i = 1; i < numB; i++) {
        char tmp[n + 1];

        addOneComp(sum, blk[i], tmp, n);
        printf("+ Block %d (%s)       : %s\n", i + 1, blk[i], tmp);

        strcpy(sum, tmp);
    }

    // Calculate Checksum

    char cs[n + 1];
    getOnesComp(sum, cs, n);

    printf("\nSum of Blocks        : %s\n", sum);
    printf("Checksum             : %s\n", cs);

    // Display Transmitted Data with spaces

    printf("\nTransmitted Data     : ");
    for (int i = 0; i < numB; i++) {
        printf("%s ", blk[i]);
    }
    printf("%s\n", cs);

    // Prepare combined string for receiver

    char rec[300] = "";
    for (int i = 0; i < numB; i++) {
        strcat(rec, blk[i]);
    }
    strcat(rec, cs);

    // Step 4: RECEIVER SIDE - Error Induction Menu

    printf("\n========== RECEIVER SIDE ==========\n");
    printf("Induce Error Options:\n");
    printf("0. No Error\n");
    printf("1. Single Bit Error\n");
    printf("2. 2-Bit Burst Error\n");
    printf("Choice: ");
    scanf("%d", &ch);

    int recLen = strlen(rec);

    if (ch == 1) {
        printf("Enter bit pos to flip (1 to %d) : ", recLen);
        scanf("%d", &pos);

        if (pos >= 1 && pos <= recLen) {
            rec[pos - 1] = (rec[pos - 1] == '0') ? '1' : '0';
        }
    } else if (ch == 2) {
        printf("Enter starting bit pos for 2-bit burst (1 to %d) : ", recLen - 1);
        scanf("%d", &pos);

        if (pos >= 1 && pos < recLen) {
            // Flip two adjacent bits
            rec[pos - 1] = (rec[pos - 1] == '0') ? '1' : '0';
            rec[pos]     = (rec[pos]     == '0') ? '1' : '0';
        }
    }

    // Step 5: Process Receiver Blocks

    int rNumB = numB + 1;
    char rBlk[rNumB][n + 1];

    printf("\n--- Receiver Blocks ---\n");
    for (int i = 0; i < rNumB; i++) {
        strncpy(rBlk[i], rec + (i * n), n);
        rBlk[i][n] = '\0';

        if (i < numB) {
            printf("Block %d   : %s\n", i + 1, rBlk[i]);
        } else {
            printf("Checksum  : %s\n", rBlk[i]);
        }
    }

    // Add all received blocks including checksum

    char rSum[n + 1];
    strcpy(rSum, rBlk[0]);

    for (int i = 1; i < rNumB; i++) {
        char tmp[n + 1];

        addOneComp(rSum, rBlk[i], tmp, n);
        strcpy(rSum, tmp);
    }

    char rCs[n + 1];
    getOnesComp(rSum, rCs, n);

    printf("\nSum at Receiver      : %s\n", rSum);
    printf("Receiver Checksum    : %s\n", rCs);

    // Step 6: Verify Error

    int err = 0;
    for (int i = 0; i < n; i++) {
        if (rCs[i] != '0') {
            err = 1;
            break;
        }
    }

    if (err == 0) {
        printf("\nResult: NO ERROR DETECTED! (Frame Accepted)\n");
    } else {
        printf("\nResult: ERROR DETECTED! (Frame Discarded)\n");
    }

    return 0;
}
