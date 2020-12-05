#include <stdio.h>
#define BUFFER_SIZE 255

int main(void)
{
    char string_buffer[BUFFER_SIZE];
    while (fgets(string_buffer, BUFFER_SIZE, stdin) != NULL)
    {
        printf("I've read: %s\n", string_buffer);
    }
    return 0;
}