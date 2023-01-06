#ifndef CASES_H_
#define CASES_H_

int menuInput(int *loop, int *el, double **data, int *reallen);

int menuInsert(int *loop, int *reallen, int *el, double **data);

int menuRemove(int *loop, int *el, int *reallen, double **data);

double *menuTask(int *loop, int *el,int *reallen, double **data, int *newEl);

#endif
