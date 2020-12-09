#include "linked-list.h"
#include <stdlib.h>
#include <stdio.h>

struct List *list_create()
{
    struct List *list = malloc(sizeof(struct List));
    if (list == NULL)
        return 0;
    list->first = NULL;
    return list;
}

status list_append(struct List *list, int value)
{
    if (list == NULL)
        return UNINITIALIZED_LIST;

    struct ListNode *node = malloc(sizeof(struct ListNode));

    if (node == NULL)
        return OUT_OF_MEMORY;

    node->value = value;
    node->next = NULL;

    if (list->first == NULL)
        list->first = node;
    else
    {
        struct ListNode *current = list->first;

        while (current->next != NULL)
            current = current->next;

        current->next = node;
    }

    return OK;
}

int list_length(struct List *list)
{
    if (list == NULL)
        return UNINITIALIZED_LIST;

    int length = 0;
    struct ListNode *current = list->first;

    while (current != NULL)
    {
        length++;
        current = current->next;
    }

    return length;
}

status list_get(struct List *list, int index, int *value)
{
    if (list == NULL)
        return UNINITIALIZED_LIST;

    if (value == NULL)
        return UNINITIALIZED_RETVAL;

    if (index < 0 || index >= list_length(list))
        return INDEX_OUT_OF_BOUNDS;

    struct ListNode *current = list->first;

    for (int i = 0; i < index; i++)
        current = current->next;

    *value = current->value;
    return OK;
}

status list_print(struct List *list)
{
    if (list == NULL)
        return UNINITIALIZED_LIST;

    int length = list_length(list);
    printf("[");

    for (int i = 0; i < length; i++)
    {
        if (i != 0)
            printf(", ");

        int value;
        list_get(list, i, &value);
        printf("%d", value);
    }

    printf("]\n");
    return OK;
}

status list_remove_item(struct List *list, int index)
{
    if (list == NULL)
        return UNINITIALIZED_LIST;

    if (index < 0 || index >= list_length(list))
        return INDEX_OUT_OF_BOUNDS;

    struct ListNode *removed_node;

    if (index == 0)
    {
        removed_node = list->first;
        list->first = removed_node->next;
    }
    else
    {
        struct ListNode *current = list->first;

        for (int i = 0; i < index - 1; i++)
            current = current->next;

        removed_node = current->next;
        current->next = removed_node->next;
    }

    free(removed_node);
    return OK;
}

status list_delete(struct List *list)
{
    if (list == NULL)
        return UNINITIALIZED_LIST;

    struct ListNode *current = list->first;

    while (current != NULL)
    {
        struct ListNode *previous = current;
        current = current->next;
        free(previous);
    }

    free(list);
    return OK;
}

status list_insert(struct List *list, int index, int value)
{
    if (list == NULL)
        return UNINITIALIZED_LIST;

    if (index < 0 || index > list_length(list))
        return INDEX_OUT_OF_BOUNDS;

    struct ListNode *node = malloc(sizeof(struct ListNode));

    if (node == NULL)
        return OUT_OF_MEMORY;

    node->value = value;
    node->next = NULL;

    if (index == 0)
    {
        node->next = list->first;
        list->first = node;
        return OK;
    }

    struct ListNode *current = list->first;

    while (--index > 0)
    {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
    return OK;
}
