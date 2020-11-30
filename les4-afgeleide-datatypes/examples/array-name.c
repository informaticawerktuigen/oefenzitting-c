#include <stdio.h>

int main(void)
{
    int my_array[] = {1, 2, 3};
    printf("my_array: %p\n", my_array);
    printf("&my_array: %p\n", &my_array);
    printf("&my_array[0]: %p\n", &my_array[0]);
    return 0;
}