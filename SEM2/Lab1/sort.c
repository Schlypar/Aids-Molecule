#include <stdlib.h>
#include <stdio.h>

#include "dynamicArray.h"

void swap(Array* array, int first, int second)
{
    Base* tmp = getBase(array, first);
    array->data[first] = array->data[second];
    array->data[second] = tmp;
}

int binarySearch(Array* array, Base* key, int start, int end, Comparator comparator)
{
    if (end <= start) return (comparator(key, array->data[start]) > 0) ? (start + 1) : start;

    int mid = (start + end) / 2;

    if (comparator(key, array->data[mid]) == 0) return mid + 1;

    if (comparator(key, array->data[mid]) > 0) return binarySearch(array, key, mid + 1, end, comparator);

    return binarySearch(array, key, start, mid - 1, comparator);
}

void binaryInsertionSort(Array* array, Comparator comparator)
{
    int size = getRealSize(array);
    int j, pos;
    Base* selected;
    for (int i = 1; i < size; i++)
    {
        j = i - 1;
        selected = getBase(array, i);

        char* typeName = getType(selected);
        if (strcmp(typeName, TYPE_NAME_ARRAY) == STRING_EQUALS)
        {
            free(typeName);
            binaryInsertionSort(selected, comparator);
            continue;
        }
        free(typeName);

        pos = binarySearch(array, selected, 0, j, (comparator));

        while (j >= pos)
        {
            array->data[j + 1] = array->data[j];
            j--;
        }
        array->data[j + 1] = selected;
    }
}

void oddEvenSort(Array* base, Comparator comparator)
{
    int size = getRealSize(base);
    int sorted = FALSE;
    while (!sorted)
    {
        sorted = TRUE;
        for (int i = 1; i < size - 1; i += 2)
        {
            if (comparator(base->data[i], base->data[i + 1]) > 0)
            {
                swap(base, i, i + 1);
                sorted = FALSE;
            }
        }
        for (int i = 0; i < size - 1; i += 2)
        {
            if (comparator(base->data[i], base->data[i + 1]) > 0)
            {
                swap(base, i, i + 1);
                sorted = 0;
            }
        }
    }
}