#ifndef MY_STRING_H
#define MY_STRING_H

#include <stddef.h>

size_t strlen (const char* str);

size_t strspn (const char *, const char *);

size_t strcspn (const char *, const char *);

void *memcpy (void *dest, void *src, size_t num);

char *strcpy (char* dest, const char* src);

char *strdup (const char *src);

char *strtok (char *str, const char *delim);

#endif