#ifndef LIST_H
#define LIST_H

typedef struct Node
{
    char symbol;
    struct Node *next;
}Node;

typedef struct List
{
    Node *head;
    Node *tail;
}List;

List *createList(void);

void listDelete(List *);

void listPrint(List *);

int listPushBack(List *, char);

int listRemove(List *, char);

#endif