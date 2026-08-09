#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEN 200

/* Check whether a number is a power of 2 */
int is_power_of_two(int num)
{
    if (num <= 0)
        return 0;

    while (num % 2 == 0)
        num = num / 2;

    return (num == 1);
}

int main()
{
    char input_data[MAX_LEN];

    int codeword[MAX_LEN];
    int rec_codeword[MAX_LEN];

    int bit_type_is_parity[MAX_LEN];
    int bit_index_label[MAX_LEN];

    int k, r, n;

    int i, j, p;

    int data_idx;
    int parity_count;
    int data_count;

    int error_position;
    int old_value;

    int syndrome_decimal;
    int syndrome_bits[32];

    int ext_idx;
    char extracted_data[MAX_LEN];


    printf("==============================================================\n");
    printf("           HAMMING CODE - SINGLE BIT ERROR CORRECTION\n");
    printf("==============================================================\n");


    /* ---------------------------------------------------------
       1. INPUT DATAWORD
       --------------------------------------------------------- */

    printf("\nEnter dataword in binary: ");
    fgets(input_data, sizeof(input_data), stdin);

    input_data[strcspn(input_data, "\n")] = '\0';

    k = strlen(input_data);

    if (k == 0)
    {
        printf("\nEmpty input entered.\n");
        return 1;
    }


    /* Check whether input contains only 0 and 1 */

    for (i = 0; i < k; i++)
    {
        if (input_data[i] != '0' && input_data[i] != '1')
        {
            printf("\nInvalid dataword. Enter only 0 and 1.\n");
            return 1;
        }
    }


    /* ---------------------------------------------------------
       2. CALCULATE NUMBER OF PARITY BITS
       Formula: 2^r >= k + r + 1
       --------------------------------------------------------- */

    r = 0;

    while (1)
    {
        int power = 1;

        for (p = 0; p < r; p++)
        {
            power = power * 2;
        }

        if (power >= k + r + 1)
        {
            break;
        }

        r++;
    }

    n = k + r;


    printf("\n---- 1. CODE PARAMETERS & PARITY CALCULATION ----\n");

    printf("Dataword Length (k)       : %d bits\n", k);

    printf("Parity Bits (r)           : %d bits\n", r);

    printf("Total Codeword Length (n) : %d bits\n", n);

    printf("\nCondition:\n");

    printf("2^%d >= %d + %d + 1\n",
           r, k, r);

    printf("%d >= %d\n",
           (1 << r), k + r + 1);


    /* ---------------------------------------------------------
       3. CREATE BIT POSITION MAP
       Position 1 is the rightmost position.
       Positions 1,2,4,8,... are parity positions.
       --------------------------------------------------------- */

    data_idx = k;

    parity_count = 1;

    data_count = 1;


    for (i = 1; i <= n; i++)
    {
        if (is_power_of_two(i))
        {
            bit_type_is_parity[i] = 1;

            bit_index_label[i] = parity_count++;

            codeword[i] = 0;
        }
        else
        {
            bit_type_is_parity[i] = 0;

            bit_index_label[i] = data_count++;

            data_idx--;

            codeword[i] = input_data[data_idx] - '0';
        }
    }


    printf("\n---- 2. BIT POSITION MAP ----\n");

    printf("\nPosition  : ");

    for (i = n; i >= 1; i--)
    {
        printf("%4d", i);
    }

    printf("   (Right to Left)\n");


    printf("Bit Type  : ");

    for (i = n; i >= 1; i--)
    {
        if (bit_type_is_parity[i])
        {
            printf("  P%d", bit_index_label[i]);
        }
        else
        {
            printf("  D%d", bit_index_label[i]);
        }
    }

    printf("\n");


    printf("Data Value: ");

    for (i = n; i >= 1; i--)
    {
        if (bit_type_is_parity[i])
        {
            printf("   _");
        }
        else
        {
            printf("%4d", codeword[i]);
        }
    }

    printf("\n");


    /* ---------------------------------------------------------
       4. SENDER SIDE - CALCULATE PARITY BITS
       --------------------------------------------------------- */

    printf("\n---- 3. SENDER SIDE - EVEN PARITY CALCULATION ----\n");


    for (i = 0; i < r; i++)
    {
        int p_pos = 1;

        for (p = 0; p < i; p++)
        {
            p_pos = p_pos * 2;
        }


        int ones_count = 0;


        printf("\nP%d checks positions: ",
               p_pos);


        for (j = 1; j <= n; j++)
        {
            if ((j / p_pos) % 2 == 1)
            {
                printf("%d ", j);
            }
        }


        printf("\nData bits checked: [");


        int first = 1;


        for (j = 1; j <= n; j++)
        {
            if ((j / p_pos) % 2 == 1 &&
                j != p_pos)
            {
                if (!first)
                {
                    printf(", ");
                }

                printf("%d",
                       codeword[j]);

                if (codeword[j] == 1)
                {
                    ones_count++;
                }

                first = 0;
            }
        }


        printf("]");


        if (ones_count % 2 == 1)
        {
            codeword[p_pos] = 1;
        }
        else
        {
            codeword[p_pos] = 0;
        }


        printf("\nNumber of 1s            : %d",
               ones_count);

        printf("\nParity P%d               : %d",
               p_pos,
               codeword[p_pos]);
    }


    /* ---------------------------------------------------------
       5. DISPLAY GENERATED HAMMING CODE
       --------------------------------------------------------- */

    printf("\n\n---- 4. GENERATED HAMMING CODE ----\n");

    printf("\nPosition : ");

    for (i = n; i >= 1; i--)
    {
        printf("%4d", i);
    }

    printf("\n");


    printf("Bit Type : ");

    for (i = n; i >= 1; i--)
    {
        if (bit_type_is_parity[i])
        {
            printf("  P%d", bit_index_label[i]);
        }
        else
        {
            printf("  D%d", bit_index_label[i]);
        }
    }

    printf("\n");


    printf("Codeword : ");

    for (i = n; i >= 1; i--)
    {
        printf("%4d", codeword[i]);
    }

    printf("\n");


    /* ---------------------------------------------------------
       6. RANDOM SINGLE-BIT ERROR
       --------------------------------------------------------- */

    for (i = 1; i <= n; i++)
    {
        rec_codeword[i] = codeword[i];
    }


    srand(time(NULL));

    error_position = rand() % n + 1;

    old_value = rec_codeword[error_position];

    rec_codeword[error_position] =
        (old_value == 0) ? 1 : 0;


    printf("\n---- 5. TRANSMISSION ERROR SIMULATION ----\n");

    printf("\nA single bit has been randomly selected for error.\n");

    printf("Random Error Position : %d\n",
           error_position);

    printf("Original Bit          : %d\n",
           old_value);

    printf("Received Bit          : %d\n",
           rec_codeword[error_position]);


    printf("\nTransmitted Codeword : ");

    for (i = n; i >= 1; i--)
    {
        printf("%d ", codeword[i]);
    }


    printf("\nReceived Codeword    : ");

    for (i = n; i >= 1; i--)
    {
        printf("%d ", rec_codeword[i]);
    }

    printf("\n");


    /* ---------------------------------------------------------
       7. RECEIVER SIDE - SYNDROME CALCULATION
       --------------------------------------------------------- */

    printf("\n---- 6. RECEIVER SIDE - SYNDROME CALCULATION ----\n");


    syndrome_decimal = 0;


    for (i = 0; i < r; i++)
    {
        int p_pos = 1;

        int ones_count = 0;


        for (p = 0; p < i; p++)
        {
            p_pos = p_pos * 2;
        }


        printf("\nS%d checks positions: ",
               p_pos);


        for (j = 1; j <= n; j++)
        {
            if ((j / p_pos) % 2 == 1)
            {
                printf("%d ", j);
            }
        }


        printf("\nReceived bits: [");


        int first = 1;


        for (j = 1; j <= n; j++)
        {
            if ((j / p_pos) % 2 == 1)
            {
                if (!first)
                {
                    printf(", ");
                }

                printf("%d",
                       rec_codeword[j]);

                if (rec_codeword[j] == 1)
                {
                    ones_count++;
                }

                first = 0;
            }
        }


        printf("]");


        if (ones_count % 2 == 1)
        {
            syndrome_bits[i] = 1;

            syndrome_decimal =
                syndrome_decimal + p_pos;
        }
        else
        {
            syndrome_bits[i] = 0;
        }


        printf("\nS%d = %d",
               p_pos,
               syndrome_bits[i]);
    }


    /* Syndrome vector */

    printf("\n\nSyndrome Vector : ");

    for (i = r - 1; i >= 0; i--)
    {
        printf("%d",
               syndrome_bits[i]);
    }


    printf("\nSyndrome Decimal Value : %d",
           syndrome_decimal);


    /* ---------------------------------------------------------
       8. ERROR DETECTION AND CORRECTION
       --------------------------------------------------------- */

    printf("\n\n---- 7. ERROR DETECTION & CORRECTION ----\n");


    if (syndrome_decimal == 0)
    {
        printf("\nNo error detected.\n");
    }
    else
    {
        printf("\nError detected at position : %d",
               syndrome_decimal);


        old_value =
            rec_codeword[syndrome_decimal];


        rec_codeword[syndrome_decimal] =
            (old_value == 0) ? 1 : 0;


        printf("\nErroneous bit              : %d",
               old_value);

        printf("\nCorrected bit              : %d",
               rec_codeword[syndrome_decimal]);
    }


    /* ---------------------------------------------------------
       9. CORRECTED CODEWORD
       --------------------------------------------------------- */

    printf("\n\n---- 8. CORRECTED CODEWORD ----\n");

    printf("\nCorrected Codeword : ");

    for (i = n; i >= 1; i--)
    {
        printf("%d ",
               rec_codeword[i]);
    }

    printf("\n");


    /* ---------------------------------------------------------
       10. EXTRACT ORIGINAL DATAWORD
       --------------------------------------------------------- */

    ext_idx = 0;


    for (i = n; i >= 1; i--)
    {
        if (!bit_type_is_parity[i])
        {
            extracted_data[ext_idx++] =
                rec_codeword[i] + '0';
        }
    }


    extracted_data[ext_idx] = '\0';


    printf("\n---- 9. DATA RECOVERY ----\n");

    printf("\nOriginal Dataword  : %s",
           input_data);

    printf("\nRecovered Dataword : %s",
           extracted_data);


    if (strcmp(input_data, extracted_data) == 0)
    {
        printf("\n\nSUCCESS: Original data recovered correctly!\n");
    }
    else
    {
        printf("\n\nERROR: Data recovery failed!\n");
    }


    printf("\n==============================================================\n");

    return 0;
}
