#include <stdio.h>

int main(void)
{
    printf("%hhd\n", 1 && 66 && -33);
    printf("%hhd\n", 0 && -54 && 44);
    printf("%hhd\n", -12 || 0);
    printf("%hhd\n", !-10 || 0);
    return 0;
}