#pragma once

#include "ring.h"

#include "base.h"
#include "container.h"

#define START_SIZE 2
#define SIZE_TO_REAL_SIZE 2
#define BASE_LENGTH_OF_WORD 40

#define TYPE_NAME_ARRAY "Array"

typedef struct Array;

typedef int (*Comparator)(Base*, Base*);
typedef void(*Sort)(struct Array*);

typedef struct Array {
	Container container;
	int realsize;
	Base** data;
	Comparator comparator;
} Array;

Array* createArray();

Array* constructArray(int);

void freeArray(Array*);

Array* copyArray(Array*);

int getRealSize(Array*);

void setComparator(Array*, Comparator);

Comparator getComparator(Array*);

char* toStringArray(Array*);

char* printArray(Array*);

char* dumpArray(Array*);

void resizeArray(Array*);

void appendArray(Array*, Base*);

Base* getBase(Array*, int);

void map(Array*, BaseF);

void where(int (*condition)(Base*), Array*, BaseF);

void sort(Array*);

void concatTwoArrays(Array*, Array*);

void resizeArrayMethod(Array*, int);

void setRealSize(Array*, int);

void initialiseResizeArray(Array*, Resize);

void initialiseAppendArray(Array*, Append);

void initialiseConcat(Array*, Concat);

void initialiseGet(Array*, Get);

void initialiseCopyArray(Array*, Copy);