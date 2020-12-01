#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p;
    {
        int *a = malloc(sizeof(int));
        if (a == NULL)
        {
            return -1;
        }

        *a = 42;
        p = a;
    }
    printf("The value of a is %d\n", *p);
    free(p);
    p = NULL;
    return 0;
}