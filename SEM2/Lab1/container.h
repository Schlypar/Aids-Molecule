#pragma once

#include "ring.h"

#include "base.h"

#define TYPE_NAME_CONTAINER "Container"

typedef void (*Append)(struct Container*, Base*);
typedef Base* (*Get)(struct Container*, int);
typedef void (*Resize)(struct Container*, int);
typedef void (*Map)(struct Container*, BaseF);
typedef void (*Where)(struct Container*, int (*)(Base*), BaseF);
typedef void (*Concat)(struct Container*, struct Container*);

typedef struct Container
{
	Base base;
	int size;
	Append append;
	Get get;
	Resize resize;
	Map map;
	Where where;
	Concat concat;
} Container;

Container* createContainer();

int getSize(Container*);

void initialiseSize(Container*, int);
