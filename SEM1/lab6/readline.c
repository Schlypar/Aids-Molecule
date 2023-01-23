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

    if (error == EOF)
    {
        listDelete(list);
        return NULL;
    } 

    while (((symbol = getchar()) != EOF))
    {
        if (symbol == '\n') break;

        error = listPushBack(list, symbol);
        if (error)
        {
            listDelete(list);
            return NULL;
        }
    }

    if (symbol == EOF) 
    {
        free(list);
        return NULL;
    }
    return list;
}
