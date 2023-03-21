
#include "ADT.h"
// #include <iostream>

// #include "catch2"

#define LINE "\n<==================================================>\n"

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
	Logger::setPriority(ErrorPriority);

	int data[] = { 1,2,3,4,5,6,7,8,9 };

	ListSequence<int>* array = new ListSequence<int>(data, sizeof(data)/sizeof(int));

	int anotherData[] = { 0,9,8,7,6,5,4,3,2,1 };

	ListSequence<int>* anotherArray = new ListSequence<int>(anotherData, sizeof(anotherData)/sizeof(int));

	ListSequence<int>* concated = (ListSequence<int>*)array->Concat(anotherArray);

	for (auto data : *concated)
		print(data, " ");
	
	print("\n");

	std::cout << concated << std::endl;

	ListSequence<int>* subArray = (ListSequence<int>*)array->GetSubsequence(1, 8);

	print(subArray, "\n");

	ListSequence<int>* mappedArray = (ListSequence<int>*)array->Map(PlusTwo);

	print("Map ------> ", mappedArray, "\n");

	ListSequence<int>* wheredArray = (ListSequence<int>*)array->Where(LessThanFive);

	print("Where ----> ",wheredArray, "\n");

	int reducedArray = array->Reduce(reduceBySum, 0);

	print("Array to be reduced: ", array, "\n", "Reducer ----> ", reducedArray, "\n");

	delete array;
	delete anotherArray;
	delete concated;
	delete subArray;
	delete mappedArray;
	delete wheredArray;

	return 0;
}