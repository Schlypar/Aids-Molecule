#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "holylib.h"
#include "sort.h"
#include "structReader.h"

int main(int argc, char **argv)
{
	int error;
	size_t lines;
	Voter *voters = structReader(&lines);
	if (!voters)
		return 1;
	error = 0;

	int menu, compar;
	//	(int(const Voter *, const Voter *)) *comparator;
	void *comparator;
	printf("Possible comparators: \n1 - name\n2 - name revesed\n");
	printf("3 - id\n4 - id reversed\n");
	printf("5 - age\n6 - age reversed\n");

	inputInt(&compar);

	switch (compar)
	{
		case 1:
			comparator = &compareName;
			break;
		case 2:
			comparator = &reverseCompareName;
			break;
		case 3:
			comparator = &compareId;
			break;
		case 4:
			comparator = &reverseCompareId;
			break;
		case 5:
			comparator = &compareInt;
			break;
		case 6:
			comparator = &reverseCompareInt;
			break;
		default:
			printf("Такого компаратора не существует\n");
			return 1;
	}

	printf("Possible sorts:\n1 - odd-even sort\n2 - binary search with insertion sort\n3 - qsort\n");

	inputInt(&menu);

	switch (menu)
	{
		case 1:
			oddEvenSort(voters, lines, comparator);
			votersPrint(voters, lines);
			break;
		case 2:
			binarySearchInsertSort(voters, lines, comparator);
			votersPrint(voters, lines);
			break;
		case 3:
			qsort(voters, lines, sizeof(Voter), comparator);
			votersPrint(voters, lines);
			break;
		default:
			printf("Такой сортировки не существует\n");
			break;
	}

	structFree(voters, lines);
	return 0;
}
