#include <stdio.h>

void swap(int a, int b)
{
    int c = a;
    a = b;
    b = c;
}

int main(void)
{
    int x = 0;
    int y = 42;

    printf("Before swap: x = %d, y = %d\n", x, y);
    swap(x, y);
    printf(" After swap: x = %d, y = %d\n", x, y);
    return 0;
}