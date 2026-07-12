#include <stdio.h>

int main()
{
    int T;
    char str[1001];

    scanf("%d", &T);

    while(T--)
    {
        int sum = 0;

        scanf("%s", str);

        for(int i = 0; str[i] != '\0'; i++)
        {
            if(str[i] >= '0' && str[i] <= '9')
            {
                sum = sum + (str[i] - '0');
            }
        }

        printf("%d\n", sum);
    }

    return 0;
}
