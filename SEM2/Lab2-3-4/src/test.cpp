
#include "Allocator.h"
#include "Sequence/ArraySequence.h"
#include <ADT.h>

LogPriority Logger::priority = InfoPriority;

int main()
{
	Sequence<int>* seq = new ArraySequence<int>(1, 2, 3, 4, 5);

	for (int e : *seq)
		std::cout << e << " ";

	delete seq;

	std::cout << '\n';
}
