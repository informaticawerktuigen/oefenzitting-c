#include <stdio.h>

int array_sum(int *arr, size_t length)
{
    int sum = 0;
    for (size_t i = 0; i < length; i++)
    {
        sum += arr[i];
    }
    return sum;
}

int main(void)
{
    int arr[] = {1, 2, 3};
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);
    int sum = array_sum(arr, arr_len);
    printf("Sum of array elements: %d\n", sum);
    return 0;
}