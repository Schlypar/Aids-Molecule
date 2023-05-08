#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "dynamicArray.h"
#include "sort.h"

Array* createArray()
{
	return (Array*) malloc(sizeof(Array));
}

Array* constructArray(int size)
{
	Array* array = createArray(size);
	array->data = (Base**) malloc(size * sizeof(Base*));

	Base* arrayBase = (Base*) array;

	arrayBase->typeName = TYPE_NAME_ARRAY;
	setRealSize(array, 0);
	initialiseSize(array, size);
	initialiseFree(array, freeArray);
	initialiseCopyArray(array, copyArray);

	initialiseToString(array, toStringArray);
	initialisePrint(array, printArray);
	initialiseDump(array, dumpArray);

	initialiseResizeArray(array, resizeArrayMethod);
	initialiseAppendArray(array, appendArray);
	initialiseConcat(array, concatTwoArrays);
	initialiseGet(array, getBase);

	return array;
}

Array* copyArray(Array* array)
{
	int realSizeOne = getRealSize(array);
	Array* arrayCopy = createArray(realSizeOne);

	for (int i = 0; i < realSizeOne; i++)
	{
		Base* current = getBase(array, i);
		appendArray(arrayCopy, current);
	}

	return arrayCopy;
}

void freeArray(Array* array)
{
	int m_size = getRealSize(array);
	for (int i = 0; i < m_size; i++)
	{
		Base* current = getBase(array, i);
		freeBase(current);
	}
	free(array);
}

int getRealSize(Array* array)
{
	return array->realsize;
}

Comparator getComparator(Array* array)
{
	return array->comparator;
}

void setComparator(Array* array, Comparator comparator)
{
	array->comparator = comparator;
}

void resizeArray(Array* array)
{
	int realSize = getRealSize(array);
	Container* container = (Container*) array;
	container->resize(array, realSize * SIZE_TO_REAL_SIZE);
}

void appendArray(Array* array, Base* base)
{
	int realSize = getRealSize(array);
	int m_size = getSize((Container*) array);

	if (realSize >= m_size)
		resizeArray(array);
	if (array->data[realSize])
	{
		array->data[realSize] = base;
		setRealSize(array, realSize + 1);
	}
}

Base* getBase(Array* array, int index)
{
	int realSize = getRealSize(array);
	if (index >= realSize)
		return NULL;

	return array->data[index];
}

void map(Array* array, BaseF BaseF)
{
	int realSize = getRealSize(array);
	for (int i = 0; i < realSize; i++)
	{
		Base* current = getBase(array, i);
		char* typeName = getType(current);
		if (baseIsThisType(typeName, TYPE_NAME_ARRAY))
		{
			free(typeName);
			map(current, BaseF);
		}
		else
			free(typeName), BaseF(current);
	}
}

void where(int (*condition)(Base*), Array* array, BaseF BaseF)
{
	int realSize = getRealSize(array);
	for (int i = 0; i < realSize; i++)
	{
		Base* current = getBase(array, i);
		char* typeName = getType(current);
		if (strcmp(typeName, TYPE_NAME_ARRAY) == STRING_EQUALS)
		{
			free(typeName);
			where(condition, current, BaseF);
		}
		else if (condition(current) == TRUE)
		{
			free(typeName);
			BaseF(current);
		}
	}
}

void concatTwoArrays(Array* arrayOne, Array* arrayTwo)
{
	int realSizeOne = getRealSize(arrayOne);
	int realSizeTwo = getRealSize(arrayTwo);
	int newRealSizeOne = realSizeOne + realSizeTwo;
	int sizeOne = getSize((Container*) arrayOne);

	if (newRealSizeOne >= sizeOne)
		resizeArray(arrayOne, newRealSizeOne * SIZE_TO_REAL_SIZE);

	for (int i = 0; i < realSizeTwo; i++)
	{
		Base* current = getBase(arrayTwo, i);
		Base* copy = copyBase(current);
		appendArray(arrayOne, copy);
	}
}

void sort(Array* array)
{
	if (!array->comparator)
		return;

	Comparator comparator = getComparator(array);
	binaryInsertionSort(array, comparator);
}

void initialiseCopyArray(Array* array, Copy copy)
{
	Base* arrayBase = (Base*) array;
	arrayBase->copy = copy;
}

void setRealSize(Array* array, int realSize)
{
	array->realsize = realSize;
}

void initialiseResizeArray(Array* array, Resize resize)
{
	Container* container = (Container*) array;
	container->resize = resize;
}

void initialiseAppendArray(Array* array, Append append)
{
	Container* container = (Container*) array;
	container->append = append;
}

void initialiseConcat(Array* array, Concat concat)
{
	Container* container = (Container*) array;
	container->concat = concat;
}

void initialiseGet(Array* array, Get get)
{
	Container* container = (Container*) array;
	container->get = get;
}

void resizeArrayMethod(Array* array, int m_size)
{
	Container* container = (Container*) array;
	initialiseSize(container, m_size);

	array->data = (Base**) realloc(array->data, m_size * sizeof(Base*));
}

char* toStringArray(Array* array)
{
	int realSize = getRealSize(array);

	char* typeName = getType(array);

	int lengthWithSpacesAndEnd = strlen(typeName) + 2 * realSize * BASE_LENGTH_OF_WORD;

	char* result = calloc(lengthWithSpacesAndEnd, 1);

	char space[] = " ";
	char comaSpace[] = ", ";
	char leftBracket[] = "[";
	char rightBracket[] = "]";

	strcat(result, typeName);
	strcat(result, space);
	strcat(result, leftBracket);

	int sizeWithoutLast = realSize - 1;

	if (sizeWithoutLast == 0)
	{
		int indexOfFirst = sizeWithoutLast;
		Base* last = getBase(array, indexOfFirst);
		char* item = toStringBase(last);
		strcat(result, item);
		strcat(result, rightBracket);
		free(item);

		return result;
	}

	for (int i = 0; i < sizeWithoutLast; i++)
	{
		Base* current = getBase(array, i);
		char* item = toStringBase(current);
		strcat(result, item);
		strcat(result, comaSpace);
		free(item);
	}

	int indexOfLast = sizeWithoutLast;
	Base* last = getBase(array, indexOfLast);

	char* item = toStringBase(last);

	strcat(result, item);
	strcat(result, rightBracket);

	free(item);
	free(typeName);

	return result;
}

char* printArray(Array* array)
{
	int realSize = getRealSize(array);

	char* result = calloc(realSize * BASE_LENGTH_OF_WORD, 1);

	char comaSpace[] = ", ";
	char leftBracket[] = "[";
	char rightBracket[] = "]";

	strcat(result, leftBracket);

	int sizeWithoutLast = realSize - 1;

	if (sizeWithoutLast == 0)
	{
		int indexOfFirst = sizeWithoutLast;
		Base* last = getBase(array, indexOfFirst);
		char* item = printBase(last);
		strcat(result, item);
		strcat(result, rightBracket);

		free(item);

		return result;
	}

	for (int i = 0; i < sizeWithoutLast; i++)
	{
		Base* current = getBase(array, i);
		char* item = printBase(current);
		strcat(result, item);
		strcat(result, comaSpace);
		free(item);
	}

	int indexOfLast = sizeWithoutLast;
	Base* last = getBase(array, indexOfLast);

	char* item = printBase(last);

	strcat(result, item);
	strcat(result, rightBracket);

	free(item);

	return result;
}

char* dumpArray(Array* array)
{
	int realSize = getRealSize(array);

	char* typeName = getType(array);

	// fillers
	char space[] = " ";
	char comaSpace[] = ", ";
	char leftBracket[] = "[";
	char rightBracket[] = "]";
	int fillerLength = 2;

	int overallLengthWithFillers = strlen(typeName) + realSize * BASE_LENGTH_OF_WORD + realSize * fillerLength;

	char* result = calloc(overallLengthWithFillers * 2, 1);

	strcat(result, typeName);
	strcat(result, space);
	strcat(result, leftBracket);

	int sizeWithoutLast = realSize - 1;

	if (sizeWithoutLast == 0)
	{
		int indexOfFirst = sizeWithoutLast;
		Base* last = getBase(array, indexOfFirst);
		char* item = dumpBase(last);
		strcat(result, item);
		strcat(result, rightBracket);

		free(item);

		return result;
	}

	for (int i = 0; i < sizeWithoutLast; i++)
	{
		Base* current = getBase(array, i);
		char* item = dumpBase(current);
		strcat(result, item);
		strcat(result, comaSpace);
		free(item);
	}

	int indexOfLast = sizeWithoutLast;
	Base* last = getBase(array, indexOfLast);

	char* item = dumpBase(last);

	strcat(result, item);
	strcat(result, rightBracket);

	free(item);

	return result;
}
