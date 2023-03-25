#include "ADT.h"
#include "ArraySequence.h"
#include "Sequence.h"
#include "moduleTest.h"

#define LINE "\n<==================================================>\n"

LogPriority Logger::priority = TracePriority;

void Test()
{
	TestSegmentedList();
	TestArray();
	TestSequence();
}

int main()
{
	Logger::setPriority(ErrorPriority);
	Test();

	Sequence<int>* seq = (Sequence<int>*) new ArraySequence<int>;
	 
	return 0;
}
