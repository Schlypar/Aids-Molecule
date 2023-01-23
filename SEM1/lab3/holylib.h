#ifndef HOLY_LIB_H
#define HOLY_LIB_H

int inputInt(int *input);

int inputDouble(double *input);

int insertElementDouble(double **data, int *len, int index, double value);

int removeElementDouble(double **data, int *len, int index);

void printArrayDouble(double *data, int len);

#endif
