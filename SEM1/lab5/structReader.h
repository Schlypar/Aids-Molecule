#ifndef STRUCT_READER_H
#define STRUCT_READER_H

#include <stddef.h>

typedef struct Voter
{
    char *fullName;
    char *idPolling;
    int age;
}Voter;

char *fullNameGiver(char *, int *);

int votersPrint(Voter *, size_t);

void structFree(Voter *, size_t);

Voter *structReader(size_t *);

#endif