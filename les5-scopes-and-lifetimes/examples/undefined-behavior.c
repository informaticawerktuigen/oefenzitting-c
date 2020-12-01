#include <stdio.h>

int main(void)
{
    int *p;
    {
        int a = 42;
        p = &a;
    }
    printf("The value of a is %d\n", *p);
    return 0;
}