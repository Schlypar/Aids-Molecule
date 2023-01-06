#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "readline.h"

List *readline(const char *PROMT)
{
    if (PROMT) printf("%s", PROMT);

    List *list = createList();

    int error = 0, symbol;
    char prev = '\0';

    error = fscanf(stdin, "[^n]");
    if (error == EOF)
    {
        listDelete(list);
        return NULL;
    } 
    printf("\"");

    while (((symbol = getchar()) != EOF))
    {
        if (symbol == '\n') break;

        printf("%c", symbol);

        if (symbol == '\t') symbol = ' ';

        if ((prev == ' ') && (symbol == ' ')) continue;

        prev = symbol;
        error = listPushBack(list, symbol);
        if (error)
        {
            listDelete(list);
            return NULL;
        }
    }
    printf("\"\n");

    if (list->head->symbol == ' ') 
    {
        Node *temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
    if (list->tail-> symbol == ' ')
    {
        Node *ptr = list->head, *prev;
        while (ptr != list->tail)
        {
            prev = ptr;
            ptr = ptr->next;
        }
        list->tail = prev;
        free(ptr);
        list->tail->next = NULL;
    }
    return list;
}