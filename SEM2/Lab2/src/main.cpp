#include <iostream>
#include <ostream>

#include "Tuple.h"
#include "List.h"
#include "Array.h"
#include "SegmentedList.h"

// #include "catch2"

int main()
{
	Logger::setPriority(ErrorPriority);

	SegmentedList<int> list;
	list.Prepend(375);
	list.Append(250);
	list.Prepend(125);

	print(list, "\n");

	int data[] = { 1,2,3,4,5,6,7,8,9,0 };	
	SegmentedList<int>secondList = SegmentedList<int>(data, sizeof(data)/sizeof(int));

	print(list, " ---- ", secondList, '\n');

	SegmentedList<int>concated = list.Concat(secondList);

	print(concated, '\n');

	for (int data : list)
		print(data, ' ');
	
	print("\n");

	Array<int> memTest;

	Array<int> array = Array<int>(data, sizeof(data)/sizeof(int));

	print('\n');

	Array<int> govno = Array<int>(data, sizeof(data)/sizeof(int));

	array.Clear();

	array = govno;

	print(array, '\n');

	return 0;
}