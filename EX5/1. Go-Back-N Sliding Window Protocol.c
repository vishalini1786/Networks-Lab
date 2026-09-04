#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int frames, window;
    int i, j;
    int ack;
    int totalTransmissions = 0;
    int lost[100];

    printf("============================================\n");
    printf("       GO-BACK-N SLIDING WINDOW PROTOCOL\n");
    printf("============================================\n");

    printf("\nEnter number of frames: ");
    scanf("%d", &frames);

    printf("Enter window size: ");
    scanf("%d", &window);

    srand(time(NULL));

    for (i = 0; i < frames; i++)
    {
        lost[i] = rand() % 4;

        if (lost[i] == 0)
            lost[i] = 1;
        else
            lost[i] = 0;
    }

    printf("\nFrame Status:\n");
    printf("--------------------------------------------\n");

    for (i = 0; i < frames; i++)
    {
        if (lost[i] == 1)
            printf("Frame %d : LOST/CORRUPTED\n", i);
        else
            printf("Frame %d : Successfully received\n", i);
    }

    printf("\n============================================\n");
    printf("             TRANSMISSION PROCESS\n");
    printf("============================================\n");

    i = 0;

    while (i < frames)
    {
        printf("\nSending window: ");

        for (j = i; j < i + window && j < frames; j++)
        {
            printf("%d ", j);
        }

        printf("\n");

        for (j = i; j < i + window && j < frames; j++)
        {
            totalTransmissions++;

            printf("Sending Frame %d -> ", j);

            if (lost[j] == 1)
            {
                printf("LOST/CORRUPTED\n");

                printf("ACK for Frame %d not received.\n", j);
                printf("Go-Back-N: Retransmitting from Frame %d.\n", j);

                break;
            }
            else
            {
                printf("Received\n");
                printf("ACK %d received.\n", j);
            }
        }

        if (j < i + window && j < frames)
        {
            for (ack = j; ack < i + window && ack < frames; ack++)
            {
                totalTransmissions++;

                printf("Retransmitting Frame %d -> ", ack);

                if (lost[ack] == 1)
                {
                    printf("LOST/CORRUPTED again\n");
                    ack--;
                    break;
                }
                else
                {
                    printf("Received\n");
                    printf("ACK %d received.\n", ack);
                }
            }

            if (ack >= frames)
                i = frames;
            else
                i = ack;
        }
        else
        {
            i = j;
        }
    }

    printf("\n============================================\n");
    printf("              FINAL RESULT\n");
    printf("============================================\n");

    printf("Total Frames       : %d\n", frames);
    printf("Window Size        : %d\n", window);
    printf("Total Transmissions: %d\n", totalTransmissions);

    printf("\nAll frames successfully delivered.\n");

    return 0;
}
