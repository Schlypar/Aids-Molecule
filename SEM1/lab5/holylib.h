#ifndef HOLY_LIB_H
#define HOLY_LIB_H

int inputInt(int *input);

int inputFloat(float *input) ;

int inputDouble(double *input);

int inputLongDouble(long double *input);

void swap(double *a, double *b);

void swapChar(char *a, char *b);

int insertElementInt(int **data, int *len, int index, int value);

int insertElementFloat(float **data, int *len, int index, float value);

int insertElementDouble(double **data, int *len, int index, double value);

int removeElementInt(int **data, int *len, int index);

int removeElementFloat(float **data, int *len, int index);

int removeElementDouble(double **data, int *len, int index);

int *arrayGen(int *data, int *len, int start, int end);

void printArrayInt(int *data, int len);

void printArrayDouble(double *data, int len);

int printString(char *text);

int *inputArray(int *len);

#endif
