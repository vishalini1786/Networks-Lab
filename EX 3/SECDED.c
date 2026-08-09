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


/* Display a codeword */
void display_codeword(int codeword[], int length)
{
    int i;

    for (i = length; i >= 1; i--)
    {
        printf("%d ", codeword[i]);
    }

    printf("\n");
}


/* ---------------------------------------------------------
   Calculate Hamming parity bits
   --------------------------------------------------------- */

void calculate_hamming_parity(int codeword[], int n, int r)
{
    int i, j;
    int p_pos;
    int ones_count;

    for (i = 0; i < r; i++)
    {
        p_pos = 1;

        for (j = 0; j < i; j++)
        {
            p_pos = p_pos * 2;
        }

        ones_count = 0;

        for (j = 1; j <= n; j++)
        {
            if ((j / p_pos) % 2 == 1)
            {
                if (j != p_pos && codeword[j] == 1)
                {
                    ones_count++;
                }
            }
        }

        if (ones_count % 2 == 1)
        {
            codeword[p_pos] = 1;
        }
        else
        {
            codeword[p_pos] = 0;
        }
    }
}


/* ---------------------------------------------------------
   Calculate Hamming syndrome
   --------------------------------------------------------- */

int calculate_syndrome(int codeword[], int n, int r)
{
    int i, j;
    int p_pos;
    int ones_count;
    int syndrome;

    syndrome = 0;

    for (i = 0; i < r; i++)
    {
        p_pos = 1;

        for (j = 0; j < i; j++)
        {
            p_pos = p_pos * 2;
        }

        ones_count = 0;

        for (j = 1; j <= n; j++)
        {
            if ((j / p_pos) % 2 == 1)
            {
                if (codeword[j] == 1)
                {
                    ones_count++;
                }
            }
        }

        if (ones_count % 2 == 1)
        {
            syndrome = syndrome + p_pos;
        }
    }

    return syndrome;
}


/* ---------------------------------------------------------
   Calculate overall parity bit

   Extended Hamming code uses EVEN parity.
   --------------------------------------------------------- */

int calculate_overall_parity(int codeword[], int n)
{
    int i;
    int ones_count;

    ones_count = 0;

    for (i = 1; i <= n; i++)
    {
        if (codeword[i] == 1)
        {
            ones_count++;
        }
    }

    if (ones_count % 2 == 1)
    {
        return 1;
    }

    return 0;
}


/* ---------------------------------------------------------
   Extract original data bits
   --------------------------------------------------------- */

void extract_data(
    int codeword[],
    int n,
    char extracted[],
    int k)
{
    int i;
    int index;

    index = 0;

    for (i = n; i >= 1; i--)
    {
        if (!is_power_of_two(i))
        {
            extracted[index] = codeword[i] + '0';
            index++;
        }
    }

    extracted[index] = '\0';
}


/* ---------------------------------------------------------
   Process one frame
   --------------------------------------------------------- */

void process_frame(
    int received[],
    int hamming_length,
    int r,
    int overall_position,
    int frame_type)
{
    int syndrome;
    int overall_check;

    int i;

    printf("\n======================================================");

    if (frame_type == 0)
    {
        printf("\n                 FRAME 1 - NO ERROR");
    }
    else if (frame_type == 1)
    {
        printf("\n            FRAME 2 - SINGLE BIT ERROR");
    }
    else
    {
        printf("\n            FRAME 3 - DOUBLE BIT ERROR");
    }

    printf("\n======================================================");


    printf("\n\nReceived Frame: ");

    for (i = overall_position; i >= 1; i--)
    {
        printf("%d ", received[i]);
    }

    printf("\n");


    /*
       Calculate Hamming syndrome
    */

    syndrome =
        calculate_syndrome(
            received,
            hamming_length,
            r);


    /*
       Calculate overall parity check.

       Include the additional overall parity bit.
    */

    overall_check = 0;

    for (i = 1; i <= overall_position; i++)
    {
        if (received[i] == 1)
        {
            overall_check++;
        }
    }

    overall_check = overall_check % 2;


    printf("\nHamming Syndrome : %d",
           syndrome);

    printf("\nOverall Parity Check : %d",
           overall_check);


    /*
       SECDED decision
    */

    if (syndrome == 0 && overall_check == 0)
    {
        printf("\n\nSTATUS : NO ERROR DETECTED");
    }

    else if (syndrome != 0 && overall_check == 1)
    {
        /*
           Single-bit error in Hamming portion
        */

        printf("\n\nSTATUS : SINGLE-BIT ERROR DETECTED");

        printf("\nError Position : %d",
               syndrome);


        printf("\nCorrection : Bit %d flipped",
               syndrome);


        if (received[syndrome] == 0)
        {
            received[syndrome] = 1;
        }
        else
        {
            received[syndrome] = 0;
        }


        printf("\nCorrected Frame: ");

        for (i = overall_position; i >= 1; i--)
        {
            printf("%d ", received[i]);
        }
    }

    else if (syndrome == 0 && overall_check == 1)
    {
        /*
           Error only in overall parity bit
        */

        printf("\n\nSTATUS : SINGLE-BIT ERROR DETECTED");

        printf("\nError is in the overall parity bit.");

        printf("\nCorrection : Overall parity bit corrected");

        if (received[overall_position] == 0)
        {
            received[overall_position] = 1;
        }
        else
        {
            received[overall_position] = 0;
        }

        printf("\nCorrected Frame: ");

        for (i = overall_position; i >= 1; i--)
        {
            printf("%d ", received[i]);
        }
    }

    else if (syndrome != 0 && overall_check == 0)
    {
        /*
           This is the important SECDED case.

           Non-zero syndrome + even overall parity
           means an even number of errors.

           For this question, we simulate
           exactly TWO errors.
        */

        printf("\n\nSTATUS : DOUBLE-BIT ERROR DETECTED");

        printf("\nSyndrome indicates an error pattern.");

        printf("\nCorrection : NOT PERFORMED");

        printf("\nReason : SECDED detects double-bit errors");
        printf(" but does not correct them.");
    }


    printf("\n");
}


int main()
{
    char input_data[MAX_LEN];

    int codeword[MAX_LEN];
    int frame_single[MAX_LEN];
    int frame_double[MAX_LEN];

    int k;
    int r;
    int n;
    int total_length;

    int i;
    int p;

    int data_index;

    int overall_parity;

    int single_error_position;

    int double_error_position1;
    int double_error_position2;

    int old_value;

    int syndrome;

    char recovered[MAX_LEN];


    /*
       ------------------------------------------------------
       TITLE
       ------------------------------------------------------
    */

    printf("==============================================================\n");
    printf("       SECDED - SINGLE ERROR CORRECTION,\n");
    printf("       DOUBLE ERROR DETECTION USING HAMMING CODE\n");
    printf("==============================================================\n");


    /*
       ------------------------------------------------------
       INPUT
       ------------------------------------------------------
    */

    printf("\nEnter dataword in binary: ");

    fgets(
        input_data,
        sizeof(input_data),
        stdin);


    input_data[
        strcspn(input_data, "\n")
    ] = '\0';


    k = strlen(input_data);


    if (k == 0)
    {
        printf("\nEmpty input entered.\n");
        return 1;
    }


    /*
       Validate binary input
    */

    for (i = 0; i < k; i++)
    {
        if (input_data[i] != '0' &&
            input_data[i] != '1')
        {
            printf("\nInvalid input.");
            printf("\nEnter only 0 and 1.\n");

            return 1;
        }
    }


    /*
       ------------------------------------------------------
       CALCULATE NUMBER OF HAMMING PARITY BITS
       ------------------------------------------------------
    */

    r = 0;

    while (1)
    {
        int power;

        power = 1;

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


    /*
       Hamming length excluding overall parity
    */

    n = k + r;


    /*
       Overall parity adds one more bit
    */

    total_length = n + 1;


    printf("\n---- 1. CODE PARAMETERS ----");

    printf("\nDataword Length        : %d", k);

    printf("\nHamming Parity Bits    : %d", r);

    printf("\nHamming Code Length    : %d", n);

    printf("\nOverall Parity Bits    : 1");

    printf("\nTotal SECDED Length    : %d",
           total_length);


    /*
       ------------------------------------------------------
       CREATE HAMMING CODE
       ------------------------------------------------------
    */

    data_index = k;


    for (i = 1; i <= n; i++)
    {
        if (is_power_of_two(i))
        {
            codeword[i] = 0;
        }
        else
        {
            data_index--;

            codeword[i] =
                input_data[data_index] - '0';
        }
    }


    /*
       Calculate Hamming parity bits
    */

    calculate_hamming_parity(
        codeword,
        n,
        r);


    /*
       Calculate overall parity
    */

    overall_parity =
        calculate_overall_parity(
            codeword,
            n);


    codeword[total_length] =
        overall_parity;


    /*
       ------------------------------------------------------
       DISPLAY GENERATED SECDED CODE
       ------------------------------------------------------
    */

    printf("\n\n---- 2. GENERATED SECDED CODE ----");

    printf("\n\nHamming Code : ");

    for (i = n; i >= 1; i--)
    {
        printf("%d ", codeword[i]);
    }

    printf("\nOverall Parity : %d",
           overall_parity);


    printf("\nSECDED Codeword : ");

    for (i = total_length; i >= 1; i--)
    {
        printf("%d ", codeword[i]);
    }


    /*
       ------------------------------------------------------
       CREATE FRAME COPIES
       ------------------------------------------------------
    */

    for (i = 1; i <= total_length; i++)
    {
        frame_single[i] = codeword[i];

        frame_double[i] = codeword[i];
    }


    /*
       Random number generation
    */

    srand(time(NULL));


    /*
       ------------------------------------------------------
       FRAME 1 - NO ERROR
       ------------------------------------------------------
    */

    printf("\n\n\n======================================================");
    printf("\n                  TRANSMISSION");
    printf("\n======================================================");

    printf("\n\nThree frames will be tested:");

    printf("\nFrame 1 : No error");

    printf("\nFrame 2 : Single-bit error");

    printf("\nFrame 3 : Double-bit error");


    process_frame(
        codeword,
        n,
        r,
        total_length,
        0);


    /*
       ------------------------------------------------------
       FRAME 2 - SINGLE ERROR
       ------------------------------------------------------
    */

    single_error_position =
        rand() % total_length + 1;


    old_value =
        frame_single[single_error_position];


    if (frame_single[single_error_position] == 0)
    {
        frame_single[single_error_position] = 1;
    }
    else
    {
        frame_single[single_error_position] = 0;
    }


    printf("\n\n------------------------------------------------------");

    printf("\nFRAME 2 ERROR SIMULATION");

    printf("\n------------------------------------------------------");

    printf("\nRandom Single Error Position : %d",
           single_error_position);

    printf("\nOriginal Bit                 : %d",
           old_value);

    printf("\nChanged Bit                  : %d",
           frame_single[single_error_position]);


    process_frame(
        frame_single,
        n,
        r,
        total_length,
        1);


    /*
       ------------------------------------------------------
       FRAME 3 - DOUBLE ERROR
       ------------------------------------------------------
    */

    double_error_position1 =
        rand() % total_length + 1;


    double_error_position2 =
        rand() % total_length + 1;


    while (double_error_position2 ==
           double_error_position1)
    {
        double_error_position2 =
            rand() % total_length + 1;
    }


    old_value =
        frame_double[double_error_position1];


    if (frame_double[double_error_position1] == 0)
    {
        frame_double[double_error_position1] = 1;
    }
    else
    {
        frame_double[double_error_position1] = 0;
    }


    if (frame_double[double_error_position2] == 0)
    {
        frame_double[double_error_position2] = 1;
    }
    else
    {
        frame_double[double_error_position2] = 0;
    }


    printf("\n\n------------------------------------------------------");

    printf("\nFRAME 3 ERROR SIMULATION");

    printf("\n------------------------------------------------------");

    printf("\nRandom Error Position 1 : %d",
           double_error_position1);

    printf("\nRandom Error Position 2 : %d",
           double_error_position2);


    process_frame(
        frame_double,
        n,
        r,
        total_length,
        2);


    /*
       ------------------------------------------------------
       DATA RECOVERY FROM SINGLE ERROR FRAME
       ------------------------------------------------------
    */

    syndrome =
        calculate_syndrome(
            frame_single,
            n,
            r);


    /*
       The frame_single was already corrected
       inside process_frame().
    */

    extract_data(
        frame_single,
        n,
        recovered,
        k);


    printf("\n\n======================================================");

    printf("\n              SINGLE ERROR RECOVERY");

    printf("\n======================================================");


    printf("\nOriginal Dataword  : %s",
           input_data);

    printf("\nRecovered Dataword : %s",
           recovered);


    if (strcmp(input_data, recovered) == 0)
    {
        printf("\n\nSTATUS : ORIGINAL DATA SUCCESSFULLY RECOVERED");
    }
    else
    {
        printf("\n\nSTATUS : DATA RECOVERY FAILED");
    }


    /*
       ------------------------------------------------------
       PERFORMANCE COMPARISON
       ------------------------------------------------------
    */

    printf("\n\n======================================================");

    printf("\n       SECDED vs STANDARD HAMMING CODE");

    printf("\n======================================================");


    printf("\n\nFeature\t\t\tStandard Hamming\tSECDED");

    printf("\n------------------------------------------------------");

    printf("\nSingle-bit correction\tYES\t\t\tYES");

    printf("\nDouble-bit detection\tNO\t\t\tYES");

    printf("\nDouble-bit correction\tNO\t\t\tNO");

    printf("\nExtra overall parity\tNO\t\t\tYES");

    printf("\nError status\t\tLimited\t\t\tMore reliable");


    printf("\n\nStandard Hamming Code:");

    printf("\n- Corrects single-bit errors.");

    printf("\n- Cannot reliably distinguish");
    printf(" some multi-bit error patterns.");


    printf("\n\nSECDED:");

    printf("\n- Corrects single-bit errors.");

    printf("\n- Detects double-bit errors.");

    printf("\n- Does not attempt to correct");
    printf(" double-bit errors.");

    printf("\n- Uses one additional overall parity bit.");


    printf("\n\n======================================================\n");

    return 0;
}
