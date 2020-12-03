#include <stdio.h>
#include <stdlib.h>

#define LIST_MAX_SIZE 100

struct List
{
    size_t length;
    int data[LIST_MAX_SIZE];
};

struct List list_create(void)
{
    struct List list;
    list.length = 0;
    return list;
}

void list_append(struct List *list, int value)
{
    if (list->length == LIST_MAX_SIZE - 1)
    {
        return;
    }
    list->data[list->length] = value;
    list->length++;
}

void list_print(struct List *list)
{
    printf("[");
    for (int i = 0; i < list->length; i++)
    {
        printf("%d", list->data[i]);
        if (i < list->length - 1)
        {
            printf(",");
        }
    }
    printf("]\n");
}

int main(void)
{
    struct List list = list_create();
    list_print(&list);
    list_append(&list, 0);
    list_print(&list);
    list_append(&list, 1);
    list_print(&list);
    printf("The memory size of the list is %lu\n", sizeof(list));
    return 0;
}