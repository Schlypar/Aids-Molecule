
#include "Logger.h"
#include "Allocator.h"
#include "Matrix.h"

#include "moduleTest.h"

LogPriority Logger::priority = TracePriority;

int main()
{
    Matrix<int> first = {3,4,
                        1, 1, 1, 1,
                        2, 2, 2, 2,
                        3, 3, 3, 3};

    Matrix<int> second = {4,3,
                        4, 4, 4,
                        3, 3, 3,
                        2, 2, 2,
                        1, 1, 1};

    // print(first, second);
    print(first);
    print("\n\n");
    print(second);

    Matrix<int> result = first * second;

    print(result);

    return 0;        
}