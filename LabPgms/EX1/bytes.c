#include <stdio.h>
#include <string.h>

#define FLAG 126  // Binary: 01111110
#define ESC  163  // Binary: 10100011

// Function to print an 8-bit binary block
void printAsBinary(int value) {
    for (int i = 7; i >= 0; i--) {
        int bit = (value >> i) & 1;
        printf("%d", bit);
    }
    printf(" ");
}

int main() {
    char text[100];    //  text input
    int data[100];     //  data as integer values
    int frame[200];    //  transmitter stuffed frame
    int result[100];   //  receiver destuffed data

    printf("Enter text (Use 'F' for FLAG and 'E' for ESC): ");
    scanf("%s", text);
    int dataLen = strlen(text);

    // Convert text characters into integer values
    for (int i = 0; i < dataLen; i++) {
        if (text[i] == 'F') {
            data[i] = FLAG;
        } else if (text[i] == 'E') {
            data[i] = ESC;
        } else {
            data[i] = text[i];
        }
    }
=
    // TRANSMITTER SIDE

    printf("\n=== TRANSMITTER SIDE ===\n");
    printf("Original Text: %s\n\n", text);

    printf("--- Text to ASCII to Binary Conversion ---\n");
    for (int i = 0; i < dataLen; i++) {
        printf("Character '%c' -> ASCII Decimal: %3d -> Binary: ", text[i], data[i]);
        printAsBinary(data[i]);
        printf("\n");
    }

    int fIdx = 0;

    // Append starting FLAG
    frame[fIdx++] = FLAG;

    // Perform Byte Stuffing
    for (int i = 0; i < dataLen; i++) {
        if (data[i] == FLAG) {
            frame[fIdx++] = ESC;  // Stuff ESC
            frame[fIdx++] = FLAG; // Actual character
        }
        else if (data[i] == ESC) {
            frame[fIdx++] = ESC;  // Stuff ESC
            frame[fIdx++] = ESC;  // Actual character
        }
        else {
            frame[fIdx++] = data[i];
        }
    }

    // Append trailing FLAG
    frame[fIdx++] = FLAG;

    printf("\n--- Final Framed Packet Output ---\n");

    printf("Framed Characters   : ");
    for (int i = 0; i < fIdx; i++) {
        if (frame[i] == FLAG) {
            printf("F        ");
        } else if (frame[i] == ESC) {
            printf("E        ");
        } else {
            printf("%c        ", frame[i]);
        }
    }

    printf("\nFramed Packet ASCII : ");
    for (int i = 0; i < fIdx; i++) {
        printf("%-8d ", frame[i]); // Left-aligned with fixed spacing
    }

    printf("\nFramed Packet Binary: ");
    for (int i = 0; i < fIdx; i++) {
        printAsBinary(frame[i]);
    }
    printf("\n");

    // RECEIVER SIDE

    printf("\n=== RECEIVER SIDE ===\n");

    int rIdx = 0;

    // Loop ignores the outer starting and trailing flags
    for (int i = 1; i < fIdx - 1; i++) {
        if (frame[i] == ESC) {
            i++; // Skip the stuffed ESC byte marker
            result[rIdx++] = frame[i]; // Keep actual data byte
        }
        else {
            result[rIdx++] = frame[i];
        }
    }

    // FINAL RESULT

    printf("\n=== FINAL RESULT ===\n");

    printf("Correct Destuffed Ans: ");
    for (int i = 0; i < rIdx; i++) {
        if (result[i] == FLAG) {
            printf("F");
        } else if (result[i] == ESC) {
            printf("E");
        } else {
            printf("%c", result[i]);
        }
    }
    printf("\n");

    printf("Final Clean ASCII    : ");
    for (int i = 0; i < rIdx; i++) {
        printf("%d ", result[i]);
    }

    printf("\nFinal Clean Binary   : ");
    for (int i = 0; i < rIdx; i++) {
        printAsBinary(result[i]);
    }
    printf("\n");

    return 0;
}
