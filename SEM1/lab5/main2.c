#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "holylib.h"
#include "sort.h"
#include "structGen.h"
#include "structReader.h"



int main(int argc, char **argv) 
{
    FILE *file;
    file = fopen("logs/binarySearchWorst.txt", "w");
    if (!file)
    {
        fprintf(stderr, "\nError opening file\n\n");
        return 1;
    }
    double log[1501] = {0};
    Voter *masterRegister = NULL;
    for (int n = 2; n <= 1500; n++)
    {
        for (int i = 0; i < 20; i++)
        {
            double start = 0, end = 0;
            masterRegister = voterGen(n);
            binarySearchInsertSort(masterRegister, n, (int (*)(const Voter *, const Voter *))reverseCompareInt);
            start = clock();
            binarySearchInsertSort(masterRegister, n, (int (*)(const Voter *, const Voter *))compareInt);
            end = clock();
            double time = (double)(end - start)/CLOCKS_PER_SEC;
            log[n] += time;
            structFree(masterRegister, n);
        }
    }
    for (int i = 0; i < 1501; i++)
    {
        log[i] = log[i]/20;
        fprintf(file, "%.7lf\n", log[i]);
    }
    fclose(file);
    return 0;
}