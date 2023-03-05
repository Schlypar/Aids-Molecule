#pragma once

#include "ring.h"

#define TRUE 1
#define FALSE 0

#define TYPE_NAME_BASE "Base"
#define STRING_EQUALS 0

typedef struct Base;

typedef void(*BaseF)(struct Base*);

typedef void (*Free)(struct Base*);
typedef struct Base* (*Copy)(struct Base*);
typedef char* (*toString)(struct Base*);
typedef toString Print;
typedef toString Dump;

typedef struct Base {
	char* typeName;
	Ring *ring;
	Free free;
	Copy copy;
	toString toString;
	Print print;
	Dump dump;
} Base;

Base* CreateBase();

void freeBase(Base*);

Base* copyBase(Base*);

char* toStringBase(Base*);

char* printBase(Base*);

char* dumpBase(Base*);

char* getType(Base*);

void initialiseFree(Base*, Free);

void initialiseCopy(Base*, Copy);

void initialiseToString(Base*, toString);

void initialisePrint(Base*, Print);

void initialiseDump(Base*, Dump);

int baseIsThisType(char*, char*);

Base* sumBase(Base*, Base*);

Base* multBase(Base*, Base*);