
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

Base* CreateBase()
{
	return (Base*) malloc(sizeof(Base));
}

void freeBase(Base* base)
{
	base->free(base);
}

Base* copyBase(Base* base)
{
	return base->copy(base);
}

char* toStringBase(Base* base)
{
	return base->toString(base);
}

char* printBase(Base* base)
{
	return base->print(base);
}

char* dumpBase(Base* base)
{
	return base->dump(base);
}

char* getType(Base* base)
{
	char* typeName = _strdup(base->typeName);
	return typeName;
}

void initialiseFree(Base* base, Free free)
{
	base->free = free;
}

void initialiseCopy(Base* base, Copy copy)
{
	base->copy = copy;
}

void initialiseToString(Base* base, toString toString)
{
	base->toString = toString;
}

void initialisePrint(Base* base, Print print)
{
	base->print = print;
}

void initialiseDump(Base* base, Dump dump)
{
	base->dump = dump;
}

int baseIsThisType(char* typeName, char* suspect)
{
	return (strcmp(typeName, suspect) == STRING_EQUALS) ? TRUE : FALSE;
}