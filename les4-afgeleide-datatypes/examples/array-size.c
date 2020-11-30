#include <stdio.h>
#include <stdint.h>

int main(void)
{
    int arr[] = {1, 10, 100, 1000, 10000};
    size_t total_memory_size = sizeof(arr);
    size_t element_memory_size = sizeof(arr[0]);
    size_t amount_of_elements = total_memory_size / element_memory_size;
    printf("Total size of the array in memory: %lu\n", total_memory_size);
    printf("Total size of an array element in memory: %lu\n", element_memory_size);
    printf("Amount of elements in array: %lu\n", amount_of_elements);
    return 0;
}