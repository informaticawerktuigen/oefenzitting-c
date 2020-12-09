#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../lib/linked-list.h"

void list_create_test()
{
    struct List *list = list_create();
    assert(list != NULL);
}

void list_append_test()
{
    //We don't use list_length since it might not yet be implemented
    struct List *list = list_create();
    assert(list != NULL);

    fprintf(stdout, " - Testing empty list\n");
    assert(list_append(list, 1) == OK);

    struct ListNode *node = list->first;
    assert(node->value == 1);
    assert(node->next == NULL);

    fprintf(stdout, " - Testing nonempty list\n");
    assert(list_append(list, 2) == OK);

    node = node->next;
    assert(list->first->next == node);
    assert(node->next == NULL);
    assert(node->value == 2);

    assert(list_append(list, 3) == OK);

    node = node->next;
    assert(list->first->next->next == node);
    assert(node->next == NULL);
    assert(node->value == 3);

    fprintf(stdout, " - Testing uninitialized list\n");
    assert(list_append(NULL, 0) == UNINITIALIZED_LIST);
}

void list_length_test()
{
    struct List *list = list_create();
    assert(list != NULL);

    fprintf(stdout, " - Testing length after appends\n");
    for (int i = 0; i < 5; i++)
    {
        assert(list_length(list) == i);
        list_append(list, i);
    }

    fprintf(stdout, " - Testing uninitialized list\n");
    assert(list_length(NULL) == UNINITIALIZED_LIST);
}

void list_get_test()
{
    struct List *list = list_create();
    assert(list != NULL);

    fprintf(stdout, " - Testing after appends\n");
    int retval;
    for (int i = 0; i < 5; i++)
    {
        list_append(list, i);
        assert(list_get(list, i, &retval) == OK);
        assert(retval == i);
    }

    fprintf(stdout, " - Testing uninitialized list\n");
    assert(list_get(NULL, 0, &retval) == UNINITIALIZED_LIST);
    fprintf(stdout, " - Testing uninitialized return value\n");
    assert(list_get(list, 0, NULL) == UNINITIALIZED_RETVAL);
    fprintf(stdout, " - Testing negative index\n");
    assert(list_get(list, -1, &retval) == INDEX_OUT_OF_BOUNDS);
    fprintf(stdout, " - Testing index out of bounds\n");
    assert(list_get(list, 5, &retval) == INDEX_OUT_OF_BOUNDS);
}

void list_remove_item_test()
{
    struct List *list = list_create();
    assert(list != NULL);

    fprintf(stdout, " - Testing on empty list\n");
    assert(list_remove_item(list, 0) == INDEX_OUT_OF_BOUNDS);

    for (int i = 0; i < 5; i++)
    {
        list_append(list, i);
    }

    fprintf(stdout, " - Testing removal of first element\n");
    assert(list_remove_item(list, 0) == OK);
    assert(list_length(list) == 4);
    assert(list->first->value == 1);

    fprintf(stdout, " - Testing removal of middle element\n");
    assert(list_remove_item(list, 1) == OK);
    assert(list_length(list) == 3);
    assert(list->first->value == 1);
    assert(list->first->next->value == 3);

    fprintf(stdout, " - Testing removal of last element\n");
    assert(list_remove_item(list, 2) == OK);
    assert(list_length(list) == 2);
    assert(list->first->value == 1);
    assert(list->first->next->value == 3);

    fprintf(stdout, " - Testing index out of bounds\n");
    assert(list_remove_item(list, 2) == INDEX_OUT_OF_BOUNDS);
    assert(list_remove_item(list, -1) == INDEX_OUT_OF_BOUNDS);

    fprintf(stdout, " - Testing uninitialized list\n");
    assert(list_remove_item(NULL, 0) == UNINITIALIZED_LIST);
    assert(list_length(list) == 2);

    //We sadly have no way of checking if the removed node was actually freed
}

void list_delete_test()
{
    struct List *list = list_create();
    assert(list != NULL);

    fprintf(stdout, " - Testing empty list\n");
    assert(list_delete(list) == OK);

    list = list_create();
    assert(list != NULL);

    for (int i = 0; i < 5; i++)
    {
        list_append(list, i);
    }

    assert(list_delete(list) == OK);

    fprintf(stdout, " - Testing uninitialized list\n");
    assert(list_delete(NULL) == UNINITIALIZED_LIST);

    //We sadly have no way of checking if all memory was actually freed
}

void list_insert_test()
{
    struct List *list = list_create();
    assert(list != NULL);

    fprintf(stdout, " - Testing out of bounds accesses\n");
    assert(list_insert(list, 1, 0) == INDEX_OUT_OF_BOUNDS);
    assert(list_length(list) == 0);

    assert(list_insert(list, -1, 0) == INDEX_OUT_OF_BOUNDS);
    assert(list_length(list) == 0);

    //list: []
    fprintf(stdout, " - Testing empty list insert\n");
    assert(list_insert(list, 0, 0) == OK);
    assert(list_length(list) == 1);
    assert(list->first->value == 0);

    //list: [0]
    fprintf(stdout, " - Testing front insert\n");
    assert(list_insert(list, 0, 1) == OK);
    assert(list_length(list) == 2);
    assert(list->first->value == 1);

    //list: [1, 0]
    fprintf(stdout, " - Testing back insert\n");
    assert(list_insert(list, 2, 2) == OK);
    assert(list_length(list) == 3);
    assert(list->first->value == 1);
    assert(list->first->next->value == 0);
    assert(list->first->next->next->value == 2);

    //list: [1, 0, 2]
    fprintf(stdout, " - Testing middle insert\n");
    assert(list_insert(list, 2, 3) == OK);
    assert(list_length(list) == 4);
    assert(list->first->value == 1);
    assert(list->first->next->value == 0);
    assert(list->first->next->next->value == 3);
    assert(list->first->next->next->next->value == 2);
}

typedef void (*unit_test_func)();
unit_test_func unit_tests[] = {
    list_create_test,
    list_append_test,
    list_length_test,
    list_get_test,
    list_remove_item_test,
    list_delete_test,
    list_insert_test,
};

const char *unit_test_names[] = {
    "list_create",
    "list_append",
    "list_length",
    "list_get",
    "list_remove_item",
    "list_delete",
    "list_insert",
};

int main(void)
{
    fprintf(stdout, "Starting unit tests...\n\n");
    int success = 0;
    size_t amount_of_tests = sizeof(unit_tests) / sizeof(unit_test_func);

    for (int i = 0; i < amount_of_tests; i++)
    {
        int pid = fork();
        if (pid == 0)
        {

            fprintf(stdout, "Starting %s_test\n", unit_test_names[i]);
            unit_tests[i]();

            exit(0);
        }
        int status;
        wait(&status);
        if (status == 0)
        {
            fprintf(stdout, "[OK] %s\n\n", unit_test_names[i]);
            success++;
        }
        else
        {

            fprintf(stderr, "[ERROR] %s\n\n", unit_test_names[i]);
        }
    }

    fprintf(stderr, "Unit tests complete (%d/%zu succesful).\n", success, amount_of_tests);
    if (success != amount_of_tests)
    {
        return -1;
    }
    return 0;
}
