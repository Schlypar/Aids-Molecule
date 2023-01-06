#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "holylib.h"
#include "sort.h"
#include "structReader.h"

int main(int argc, char **argv) {
    int error;
    size_t lines;
    Voter *voters = structReader(&lines);
    if (!voters) {
        return 1;
    }
    error = 0;
    int option;
    printf("\nMein menu:\n\t1 -> 6: odd-even sort\n\t7 -> 12: binary search with insertion sort\n\t13 -> 18: qsort from standart library\n\t0: print\n\n");
    error = inputInt(&option);
    while (1) 
    {   
        printf("\nMein menu:\n\t1 -> 6: odd-even sort\n\t7 -> 12: binary search with insertion sort\n\t13 -> 18: qsort from standart library\n\ts0: print\n\n");
        error = inputInt(&option);
        if (error) break;
        switch (option) {
            case 0:
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 1:
                oddEvenSort(voters, lines, (int (*)(const Voter *, const Voter *))compareName);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 2:
                oddEvenSort(voters, lines, (int (*)(const Voter *, const Voter *))compareId);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 3:
                oddEvenSort(voters, lines, (int (*)(const Voter *, const Voter *))compareInt);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 4:
                oddEvenSort(voters, lines, (int (*)(const Voter *, const Voter *))reverseCompareName);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 5:
                oddEvenSort(voters, lines, (int (*)(const Voter *, const Voter *))reverseCompareId);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 6:
                oddEvenSort(voters, lines, (int (*)(const Voter *, const Voter *))reverseCompareInt);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 7:
                binarySearchInsertSort(voters, lines, (int (*)(const Voter *, const Voter *))compareName);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 8:
                binarySearchInsertSort(voters, lines, (int (*)(const Voter *, const Voter *))compareId);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 9:
                binarySearchInsertSort(voters, lines, (int (*)(const Voter *, const Voter *))compareInt);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 10:
                binarySearchInsertSort(voters, lines, (int (*)(const Voter *, const Voter *))reverseCompareName);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 11:
                binarySearchInsertSort(voters, lines, (int (*)(const Voter *, const Voter *))reverseCompareId);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 12:
                binarySearchInsertSort(voters, lines, (int (*)(const Voter *, const Voter *))reverseCompareInt);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 13:
                qsort(voters, sizeof(Voter), lines, (int (*)(const void *, const void *))compareName);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 14:
                qsort(voters, sizeof(Voter), lines, (int (*)(const void *, const void *))compareId);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 15:
                qsort(voters, sizeof(Voter), lines, (int (*)(const void *, const void *))compareInt);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 16:
                qsort(voters, sizeof(Voter), lines, (int (*)(const void *, const void *))reverseCompareName);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 17:
                qsort(voters, sizeof(Voter), lines, (int (*)(const void *, const void *))reverseCompareId);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            case 18:
                qsort(voters, sizeof(Voter), lines, (int (*)(const void *, const void *))reverseCompareInt);
                error = votersPrint(voters, lines);
                if (error) {
                    structFree(voters, lines);
                    return 1;
                }
                break;
            default:
                printf("Smert\n");
                break;
            }
            
    }
  structFree(voters, lines);
  return 0;
}