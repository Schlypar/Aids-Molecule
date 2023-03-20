#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "list.h"
#include "sort.h"

int compare(Node *a, Node *b)
{
    if (a->symbol > b->symbol) return 1;

    if (a->symbol < b->symbol) return -1;

    return 0;
}

void swap(Node *a, Node *b)
{
    Node *tmp = malloc(sizeof(Node));
    tmp->symbol = a->symbol;
    a->symbol = b->symbol;
    b->symbol = tmp->symbol;
    free(tmp);
}

void sort(Node *start, Node *end, int (*compare)(Node *, Node *))
{
    Node *ptr = start, *temp = NULL;
    char minChar;
    while (ptr != end->next)
    {
        if (ptr == end->next) break;

        minChar = ptr->symbol;
        temp = ptr->next;

        while (temp != end->next)
        {
            if (compare(temp, ptr) < 0)
            {
                minChar = temp->symbol;
            }
            if (minChar != ptr->symbol) swap(temp, ptr);
            temp = temp->next;
        }
        ptr = ptr->next;
    }
}