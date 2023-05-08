#include "sort.h"
#include "structReader.h"
#include <stdio.h>
#include <string.h>


int compareName(const Voter *V1, const Voter *V2)
{
	return strcmp(V1->fullName, V2->fullName);
}

int reverseCompareName(const Voter *V1, const Voter *V2)
{
	return strcmp(V2->fullName, V1->fullName);
}

int compareId(const Voter *V1, const Voter *V2)
{
	return strcmp(V1->idPolling, V2->idPolling);
}

int reverseCompareId(const Voter *V1, const Voter *V2)
{
	return strcmp(V2->idPolling, V1->idPolling);
}

int compareInt(const Voter *V1, const Voter *V2)
{
	return V1->age - V2->age;
}

int reverseCompareInt(const Voter *V1, const Voter *V2)
{
	return V2->age - V1->age;
}

void swapVoter(Voter *array, int V1, int V2)
{
	Voter tmp = array[V1];
	array[V1] = array[V2];
	array[V2] = tmp;
}

void oddEvenSort(Voter *base, size_t size, int (*compare)(const Voter *, const Voter *))
{
	int sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		for (int i = 1; i < size - 1; i += 2)
		{
			if (compare(&base[i], &base[i + 1]) > 0)
			{
				swapVoter(base, i, i + 1);
				sorted = 0;
			}
		}
		for (int i = 0; i < size - 1; i += 2)
		{
			if (compare(&base[i], &base[i + 1]) > 0)
			{
				swapVoter(base, i, i + 1);
				sorted = 0;
			}
		}
	}
}

int binarySearch(Voter *base, Voter *key, int start, int end, int (*compare)(const Voter *, const Voter *))
{
	if (end <= start)
		return (compare(key, &base[start]) > 0) ? (start + 1) : start;

	int mid = (start + end) / 2;

	if (compare(key, &base[mid]) == 0)
		return mid + 1;

	if (compare(key, &base[mid]) > 0)
		return binarySearch(base, key, mid + 1, end, compare);

	return binarySearch(base, key, start, mid - 1, compare);
}

void binarySearchInsertSort(Voter *base, size_t size, int (*compare)(const Voter *, const Voter *))
{
	int i = 1, j, pos;
	Voter selected;
	for (; i < size; i++)
	{
		j = i - 1;
		selected = base[i];

		pos = binarySearch(base, &selected, 0, j, (compare));

		while (j >= pos)
		{
			base[j + 1] = base[j];
			j--;
		}
		base[j + 1] = selected;
	}
}