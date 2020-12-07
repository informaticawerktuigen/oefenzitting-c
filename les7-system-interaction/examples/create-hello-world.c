#include <stdio.h>

int main(void)
{
    const char *filename = "hello_world";
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "%s couldn't be created or opened\n", filename);
    }
    fprintf(file, "Hello, world!");
    fclose(file);
    return 0;
}