#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 3
struct ArrayListElement
{
    int value[ARRAY_SIZE];
    struct ArrayListElement *next;
};

struct ArrayList
{
    size_t length;
    struct ArrayListElement *first;
};