#ifndef SORT_H
#define SORT_H

#include "structReader.h"

int compareName(const Voter *, const Voter *);

int reverseCompareName(const Voter *, const Voter *);

int compareId(const Voter *, const Voter *);

int reverseCompareId(const Voter *, const Voter *);

int compareInt(const Voter *, const Voter *);

int reverseCompareInt(const Voter *, const Voter *);

void swapVoter(Voter *, int, int);

void oddEvenSort(Voter *, size_t , int (*compare)(const Voter *, const Voter *));

int binarySearch(Voter *, Voter *, int , int , int (*compare)(const Voter *, const Voter *));

void binarySearchInsertSort(Voter *, size_t , int (*compare)(const Voter *, const Voter *));

#endif