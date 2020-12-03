#include <stdio.h>

int factorial(int n)
{
    printf("n=%d (allocated)\n", n);
    if (n == 0)
    {
        printf("n=%d (deallocating)\n", n);
        return 1;
    }
    int res = factorial(n - 1);
    printf("n=%d (deallocating)\n", n);
    return n * res;
}

int main(void)
{
    factorial(10);
    return 0;
}