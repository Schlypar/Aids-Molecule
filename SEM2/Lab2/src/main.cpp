
#include "ADT.h"
// #include <iostream>

// #include "catch2"

int main()
{
	Logger::setPriority(ErrorPriority);

	int data[] = { 1,2,3,4,5,6,7,8,9 };

	ArraySequence<int>* array = new ArraySequence<int>(data, sizeof(data)/sizeof(int));

	int anotherData[] = { 0,9,8,7,6,5,4,3,2,1 };

	ArraySequence<int>* anotherArray = new ArraySequence<int>(anotherData, sizeof(anotherData)/sizeof(int));

	ArraySequence<int>* concated = (ArraySequence<int>*)(((array))->Concat(((anotherArray))));

	// auto iter = concated->_Begin();
	// auto end = concated->_End();

	// for (; !(iter->_isEquals(end)); iter->_Next())
	// 	std::cout << iter->_GetCurrent() << std::endl;
	
	// delete iter; delete end;

	// ArraySequence<int>* norm = (ArraySequence<int>*)concated;

	for (int data : *concated)
		print(data, " ");

	delete concated;
	delete array;
	delete anotherArray;
	
	return 0;
}