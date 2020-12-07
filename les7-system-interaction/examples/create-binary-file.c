#include <stdio.h>
#include <stdint.h>

int main(void)
{
    const char *filename = "binary_file";
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        fprintf(stderr, "%s couldn't be created or opened\n", filename);
        return -1;
    }

    for (uint8_t i = 0; i < UINT8_MAX; i++)
    {
        printf("Writing byte 0x%x to file\n", i);
        if (fwrite(&i, sizeof(uint8_t), 1, file) != 1)
        {
            return -1;
        }
    }
    fclose(file);
    return 0;
}