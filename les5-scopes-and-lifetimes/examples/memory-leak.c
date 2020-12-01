#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p;
    for (int i = 0; i < 10; i++)
    {
        p = malloc(sizeof(int));
        *p = i;
    }
    free(p);
    return 0;
}