#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t array_length = sizeof(array) / sizeof(array[0]);

    for (unsigned i = 0; i < array_length; i++)
    {
        printf("My current value is: %d\n", array[i]);

        if (i - 1 >= 0)
        {
            printf("The value to my left is: %d\n", array[i - 1]);
        }

        if (i + 1 < array_length)
        {
            printf("The value to my right is %d\n", array[i + 1]);
        }
        printf("\n");
    }
    return 0;
}