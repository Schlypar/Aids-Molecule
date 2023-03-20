#ifndef SORT_H
#define SORT_H

#include "list.h"

int compare(Node *, Node *);

void swap(Node *, Node *);

void sort(Node *, Node *, int (*compare)(Node *, Node *));

#endif