#include <stdio.h>

int main(void)
{
    char bad_str[] = {'x', 'y', 'z'};
    const char *good_str1 = "abc";
    char good_str2[] = {'d', 'e', 'f', '\0'};
    char good_str3[] = {103, 104, 105, 0};

    printf("Bad: %s\n", bad_str);
    printf("Good 1: %s\n", good_str1);
    printf("Good 2: %s\n", good_str2);
    printf("Good 3: %s\n", good_str3);

    return 0;
}