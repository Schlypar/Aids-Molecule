#include "ring.h"
#include <stdlib.h>
#include <string.h>


#include "complex.h"
#include "dynamicArray.h"
#include "real.h"


Ring* createRing()
{
	return malloc(sizeof(Ring));
}

Ring* constructRing(Sum sum, Mult mult)
{
	Ring* ring = createRing();

	initialiseSum(ring, sum);
	initialiseMult(ring, mult);

	return ring;
}

Ring* getRing(Base* base)
{
	return base->ring;
}

Base* sumBase(Base* first, Base* second)
{
	char* typeNameFirst = getType(first);
	char* typeNameSecond = getType(second);

	if ((strcmp(typeNameFirst, typeNameSecond) == STRING_EQUALS) && (strcmp(typeNameFirst, TYPE_NAME_ARRAY) != STRING_EQUALS))
	{
		Ring* ring = getRing(first);
		free(typeNameFirst);
		free(typeNameSecond);
		return ring->sum(first, second);
	}

	if ((strcmp(typeNameFirst, TYPE_NAME_ARRAY) == STRING_EQUALS)
		|| (strcmp(typeNameSecond, TYPE_NAME_ARRAY) == STRING_EQUALS))
	{
		free(typeNameFirst);
		free(typeNameSecond);
		return first;
	}

	if ((strcmp(typeNameFirst, TYPE_NAME_REAL) == STRING_EQUALS)
		&& (strcmp(typeNameSecond, TYPE_NAME_COMPLEX) == STRING_EQUALS))
	{
		Ring* ring = getRing(second);
		free(typeNameFirst);
		free(typeNameSecond);
		double real = getReal(first);
		Complex* complex = constructComplex(real, 0);
		return ring->sum(complex, second);
	}
	if ((strcmp(typeNameFirst, TYPE_NAME_COMPLEX) == STRING_EQUALS)
		&& (strcmp(typeNameSecond, TYPE_NAME_REAL) == STRING_EQUALS))
	{
		Ring* ring = getRing(first);
		free(typeNameFirst);
		free(typeNameSecond);
		double real = getReal(second);
		Complex* complex = constructComplex(real, 0);
		return ring->sum(complex, first);
	}
}

Base* multBase(Base* first, Base* second)
{
	char* typeNameFirst = getType(first);
	char* typeNameSecond = getType(second);

	if ((strcmp(typeNameFirst, typeNameSecond) == STRING_EQUALS) && (strcmp(typeNameFirst, TYPE_NAME_ARRAY) != STRING_EQUALS))
	{
		Ring* ring = getRing(first);
		free(typeNameFirst);
		free(typeNameSecond);
		return ring->mult(first, second);
	}

	if ((strcmp(typeNameFirst, TYPE_NAME_ARRAY) == STRING_EQUALS)
		|| (strcmp(typeNameSecond, TYPE_NAME_ARRAY) == STRING_EQUALS))
	{
		free(typeNameFirst);
		free(typeNameSecond);
		return first;
	}

	if ((strcmp(typeNameFirst, TYPE_NAME_REAL) == STRING_EQUALS)
		&& (strcmp(typeNameSecond, TYPE_NAME_COMPLEX) == STRING_EQUALS))
	{
		Ring* ring = getRing(second);
		free(typeNameFirst);
		free(typeNameSecond);
		double real = getReal(first);
		Complex* complex = constructComplex(real, 0);
		return ring->mult(complex, second);
	}
	if ((strcmp(typeNameFirst, TYPE_NAME_COMPLEX) == STRING_EQUALS)
		&& (strcmp(typeNameSecond, TYPE_NAME_REAL) == STRING_EQUALS))
	{
		Ring* ring = getRing(first);
		free(typeNameFirst);
		free(typeNameSecond);
		double real = getReal(second);
		Complex* complex = constructComplex(real, 0);
		return ring->mult(complex, first);
	}
}

Base* zeroBase(char* typeName)
{
	if (baseIsThisType(typeName, TYPE_NAME_REAL) == TRUE)
	{
		free(typeName);
		return constructReal(0);
	}

	if (baseIsThisType(typeName, TYPE_NAME_COMPLEX) == TRUE)
	{
		free(typeName);
		return constructComplex(0, 0);
	}

	if (baseIsThisType(typeName, TYPE_NAME_ARRAY) == TRUE)
	{
		free(typeName);
		return constructArray(0);
	}
}

Base* oneBase(char* typeName)
{
	if (baseIsThisType(typeName, TYPE_NAME_REAL) == TRUE)
	{
		free(typeName);
		return constructReal(1);
	}

	if (baseIsThisType(typeName, TYPE_NAME_COMPLEX) == TRUE)
	{
		free(typeName);
		return constructComplex(1, 1);
	}

	if (baseIsThisType(typeName, TYPE_NAME_ARRAY) == TRUE)
	{
		free(typeName);
		return constructArray(1);
	}
}

void initialiseSum(Ring* ring, Sum sum)
{
	ring->sum = sum;
}

void initialiseMult(Ring* ring, Mult mult)
{
	ring->mult = mult;
}

void initialiseZero(Ring* ring, Zero zero)
{
	ring->zero = zero;
}

void initialiseOne(Ring* ring, One one)
{
	ring->one = one;
}