#include <stdio.h>

int main(void)
{
    int a = 0;
    int *p = &a;
    printf("The value of variable a is: %d\n", a);
    printf("The memory address of variable a is: %p\n", p);
    printf("The value stored at the memory address in p is: %d\n", *p);

    p = 10;
    printf("The value of variable a is: %d\n", a);
    printf("The memory address of variable a is: %p\n", p);
    printf("The value stored at the memory address in p is: %d\n", *p);
    return 0;
}
