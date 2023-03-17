#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ring.h"

#include "real.h"

Real* createReal()
{
	return (Real*)malloc(sizeof(Real));
}

Real* constructReal(double value)
{
	Real* real = createReal();
	Base* realBase = (Base*)real;

	realBase->typeName = TYPE_NAME_REAL;
	realBase->ring = constructRing(sumReal, multReal);

	setReal(real, value);
	initialiseCopy(realBase, copyReal);
	initialiseFree(realBase, freeReal);
	initialiseToString(realBase, toStringReal);
	initialisePrint(realBase, printReal);
	initialiseDump(realBase, dumpReal);

	return (Base*)real;
}

Real* copyReal(Real* real)
{
	double value = getReal(real);
	Real* copy = constructReal(value);

	return copy;
}

void freeReal(Base* real)
{
	free(real);
}

double getReal(Real* real)
{
	return real->real;
}

char* toStringReal(Real* real)
{
	char* typeName = getType(real);
	return typeName;
}

char* printReal(Real* real)
{
	double realPart = getReal(real);
	char* string = calloc(BASE_LENGTH_OF_REAL_WORD, sizeof(char));
	if (snprintf(string, BASE_LENGTH_OF_REAL_WORD, "%lf", realPart) != EOF)
	{
		return string;
	}
}

char* dumpReal(Real* real)
{
	char* typeName = toStringReal(real);
	char space[] = " ";
	char* number = printBase(real);

	int lengthOfTypeName = strlen(typeName);
	int lengthOfNumber = strlen(number);

	int lengthWithEndAndSpace = lengthOfTypeName + lengthOfNumber + 2;
	char* result = calloc(lengthWithEndAndSpace, 1);

	strcat(result, typeName);
	strcat(result, space);
	strcat(result, number);

	free(typeName);
	free(number);

	return result;
}

void setReal(Real* real, double value)
{
	real->real = value;
}

Real* sumReal(Real* first, Real* second)
{
	return constructReal(getReal(first) + getReal(second));
}

Real* multReal(Real* first, Real* second)
{
	return constructReal(getReal(first) * getReal(second));
}