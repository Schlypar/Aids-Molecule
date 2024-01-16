#include "Sequence/ArraySequence.h"
#include "Sequence/Sequence.h"
#include <ADT.h>

LogPriority Logger::priority = FatalPriority;

int main()
{
	Sequence<int>* seq = new ArraySequence<int> { 1, 2, 3, 4, 5 };

	for (auto x : *seq)
	{
		std::cout << x << "\n";
	}

	return 0;
}
