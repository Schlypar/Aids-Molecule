#include "ADT.h"
#include "moduleTest.h"

#define LINE "\n<==================================================>\n"

LogPriority Logger::priority = TracePriority;

int PlusTwo(int& number)
{
	return number + 2;
}

bool LessThanFive(int& number)
{
	return (number < 5) ? true : false;
}

int reduceBySum(int first, int second)
{
	return first + second;
}

int main()
{
	// Logger::setPriority(ErrorPriority);
	TestSegmentedList();
	TestArray();
	TestSequence();

	return 0;
}