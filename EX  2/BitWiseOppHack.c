#include <stdio.h>

void calculate_the_maximum(int n, int k)
{
    int maxAnd = 0;
    int maxOr = 0;
    int maxXor = 0;

    int i, j;

    for(i = 1; i <= n; i++)
    {
        for(j = i + 1; j <= n; j++)
        {
            int andValue = i & j;
            int orValue = i | j;
            int xorValue = i ^ j;

            if(andValue < k && andValue > maxAnd)
                maxAnd = andValue;

            if(orValue < k && orValue > maxOr)
                maxOr = orValue;

            if(xorValue < k && xorValue > maxXor)
                maxXor = xorValue;
        }
    }

    printf("%d\n", maxAnd);
    printf("%d\n", maxOr);
    printf("%d\n", maxXor);
}

int main()
{
    int n, k;

    scanf("%d %d", &n, &k);

    calculate_the_maximum(n, k);

    return 0;
}
