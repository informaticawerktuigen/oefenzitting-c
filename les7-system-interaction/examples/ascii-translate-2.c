#include <stdio.h>

int main(void)
{
    int character;
    while ((character = fgetc(stdin)) != EOF)
    {
        printf("%c (ASCII: %d)\n", character, character);
    }
    return 0;
}