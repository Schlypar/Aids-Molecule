
#include "ADT.h"
#include <iostream>

// #include "catch2"

int main()
{
	// Logger::setPriority(ErrorPriority);

	int data[] = { 1,2,3,4,5,6,7,8,9 };

	ArraySequence<int> array = ArraySequence<int>(data, sizeof(data)/sizeof(int));

	// auto iter = array._Begin();
	// auto end = array._End();

	// for (; !(iter->_isEquals(end)); iter->_Next())
	// 	std::cout << iter->_GetCurrent() << std::endl;
	
	// delete iter; delete end;
	
	for (auto data : array)
		std::cout << data << std::endl;

	return 0;
}