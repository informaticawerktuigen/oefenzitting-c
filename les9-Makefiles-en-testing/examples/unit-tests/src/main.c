#include "../lib/linked-list.h"

int main(void)
{
    struct List *list = list_create();

    list_print(list);
    for (int i = 0; i < 10; i++)
    {
        list_insert(list, 0, i);
        list_print(list);
    }
    return 0;
}