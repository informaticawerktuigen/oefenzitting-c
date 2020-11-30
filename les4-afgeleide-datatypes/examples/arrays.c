#include <stdio.h>

int main(void)
{
    int arr[5] = {1, 10, 100, 1000, 10000};
    for (int i = 0; i < 5; i++)
    {
        printf("Element %d of arr has value %d\n", i, arr[i]);
    }
    return 0;
}