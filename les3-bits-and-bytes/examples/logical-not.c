#include <stdio.h>

int main(void)
{
    for (signed char c = -10; c < 10; c++)
    {
        printf("%hhd ", !c);
    }
    return 0;
}