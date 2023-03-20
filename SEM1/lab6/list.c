#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

List *createList(void)
{
    return (List *) calloc(1, sizeof(List));
}

void listDelete(List *list)
{
    Node *ptr = list->head, *prev;
    while (ptr)
    {
        prev = ptr;
        ptr = ptr->next;
        free(prev);
    }
    free(list);
}

void listPrint(List *list)
{
    Node *ptr = list->head;
    printf("\"");
    while (ptr)
    {
        printf("%c", ptr->symbol);
        ptr = ptr->next;
    }
    printf("\"\n");
}

int listPushBack(List *list, char symbol)
{
    Node *ptr = malloc(sizeof(Node));
    if (!ptr) return 1;

    ptr->symbol = symbol;
    ptr->next = NULL;

    if (!list->head) 
    {
        list->head = ptr;
        list->tail = ptr;
    }
    else
    {
        list->tail->next = ptr;
        list->tail = ptr;
    }
    return 0;
}

int listRemove(List *list, char symbol)
{
    Node *ptr = list->head, *prev = NULL;

    while (ptr && ptr->symbol != symbol)
    {
        prev = ptr;
        ptr = ptr->next;
    }
    if (!ptr) return 1;

    if (ptr == list->head) list->head = ptr->next;

    if (ptr == list->tail) list->tail = prev;

    if (prev) prev->next = ptr->next;

    free(ptr);
    return 0;
}