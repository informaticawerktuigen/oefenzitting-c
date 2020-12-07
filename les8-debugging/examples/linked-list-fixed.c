#include <stdio.h>
#include <stdlib.h>

struct ListElement
{
    int value;
    struct ListElement *next;
};

struct List
{
    struct ListElement *first;
};

struct List *list_create(void)
{
    struct List *list = malloc(sizeof(struct List));
    if (list == NULL)
    {
        return NULL;
    }
    list->first = NULL;
    return list;
}

int list_append(struct List *list, int value)
{
    if (list == NULL)
    {
        return -1;
    }
    struct ListElement *element = malloc(sizeof(struct ListElement));
    element->value = value;
    element->next = NULL;
    if (list->first == NULL)
    {
        list->first = element;
        return 0;
    }

    struct ListElement *last = list->first;
    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = element;
    return 0;
}

int list_length(struct List *list)
{
    if (list == NULL)
    {
        return -1;
    }

    int length = 0;

    struct ListElement *current = list->first;
    while (current != NULL)
    {
        length++;
        current = current->next;
    }

    return length;
}

int list_print(struct List *list)
{
    if (list == NULL)
    {
        return -1;
    }

    printf("[");

    struct ListElement *current = list->first;
    int length = list_length(list);
    for (int i = 0; i < length; i++)
    {
        printf("%d", current->value);
        current = current->next;
        if (i < length - 1)
        {

            printf(",");
        }
    }
    printf("]\n");
    return 0;
}

int list_destroy(struct List *list)
{
    if (list == NULL)
    {
        return -1;
    }

    struct ListElement *current = list->first;
    while (current != NULL)
    {
        struct ListElement *to_free = current;
        current = current->next;
        free(to_free);
    }

    free(list);

    return 0;
}

int main(void)
{
    struct List *list = list_create();
    list_print(list);
    list_append(list, 42);
    list_print(list);
    list_append(list, 3);
    list_print(list);
    list_destroy(list);
    return 0;
}