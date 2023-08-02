#include "Allocator.h"
#include "Iterator.h"
#include "Sequence/ArraySequence.h"
#include <ADT.h>

LogPriority Logger::priority = InfoPriority;

int square(int e)
{
	return e * e;
}

int main()
{
	Sequence<int> *seq = new ArraySequence<int>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

	for (auto e : *seq)
		std::cout << e << " ";

	std::cout << "\n";

	delete seq;

	return 0;
}
