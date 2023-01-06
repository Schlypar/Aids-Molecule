#include <endian.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "sort.h"
#include "readline.h"

#define PROMT "----> "

void proccess(List *list)
{
    if (!list) return;

    Node *start = list->head, *end = list->head;
    while (end)
    {
        if (end == list->tail)
        {
            sort(start, end, (int (*)(Node *, Node *))compare);
            break;
        }
        if (end->next->symbol == ' ') 
        {
            sort(start, end, (int (*)(Node *, Node *))compare);
            if (end->next->next) 
            {
                start = end->next;
                end = start;
            }
            else break;
        }
        end = end->next;
    }
}

int main()
{
    List *list = NULL;
    while (1)
    {
        list = readline(PROMT);
        if (!list)
        {
            printf("\n");
            break;
        }
        proccess(list);
        listPrint(list);
        listDelete(list);
    }
    return 0;
}