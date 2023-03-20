#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

char **parser(char *str, int *len);

void sort(char **sentence, int len);

char *sortWordOrder(char *text);


#endif
