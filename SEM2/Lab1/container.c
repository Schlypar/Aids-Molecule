#include "container.h"

Container* createContainer()
{
	return (Container*) malloc(sizeof(Container));
}

int getSize(Container* container)
{
	return container->size;
}

void initialiseSize(Container* container, int size)
{
	container->size = size;
}
