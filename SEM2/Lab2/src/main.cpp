
#include "ADT.h"
// #include <iostream>

// #include "catch2"

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

	ArraySequence<int>* array = new ArraySequence<int>(data, sizeof(data)/sizeof(int));

	int anotherData[] = { 0,9,8,7,6,5,4,3,2,1 };

	ArraySequence<int>* anotherArray = new ArraySequence<int>(anotherData, sizeof(anotherData)/sizeof(int));

	ArraySequence<int>* concated = (ArraySequence<int>*)array->Concat(anotherArray);

	for (auto data : *concated)
		print(data, " ");
	
	print("\n");

	std::cout << concated << std::endl;

	ArraySequence<int>* subArray = (ArraySequence<int>*)array->GetSubsequence(1, 8);

	print(subArray, "\n");

	ArraySequence<int>* mappedArray = (ArraySequence<int>*)array->Map(PlusTwo);

	print("Map ------> ", mappedArray, "\n");

	ArraySequence<int>* wheredArray = (ArraySequence<int>*)array->Where(LessThanFive);

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