#include <stdio.h>

int main(void)
{
    signed char c1 = -1;
    unsigned char c2 = -1;

    if (c1 > 0)
    {
        printf("c1 is positive\n");
    }
    else
    {
        printf("c1 is negative\n");
    }

    if (c2 > 0)
    {
        printf("c2 is positive!\n");
    }
    else
    {
        printf("c2 is negative\n");
    }

    printf("c1 has value %hhd\n", c1);
    printf("c2 has value %hhd\n", c2);
    printf("c1 has value %hhu\n", c1);
    printf("c2 has value %hhu\n", c2);

    return 0;
}