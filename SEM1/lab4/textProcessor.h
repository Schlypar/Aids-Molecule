#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

char *stringConcatenate(char *str1, char *str2);

char **removeElementString(char ***, int *, int);

char **parser(char *str, int *len);

void sort(char **sentence, int len);

char *sortWordOrder(char *text);

#endif
