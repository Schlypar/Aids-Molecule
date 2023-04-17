
#include "Logger.h"
#include "Allocator.h"
#include "Matrix.h"

#include "moduleTest.h"

LogPriority Logger::priority = TracePriority;

int main()
{
    // Logger::setPriority(ErrorPriority);

    Matrix<int> first = {3,4,
                        1,1,1,1,
                        2,2,2,2,
                        3,3,3,3};

    Matrix<int> second = {3,4,
                        4,4,4,4,
                        3,3,3,3,
                        2,2,2,2};

    
    auto sum = first + 1;
    auto sum2 = first + second;

    print(sum,'\n');
    print(sum2,'\n');

    auto diff = first - 2;
    auto diff2 = first - second;

    print(diff,'\n');
    print(diff2,'\n');

    auto lolcheck = first - first;

    print(lolcheck, '\n');

    Matrix<int> result = first * second.Transpose();

    print(result);

    return 0;        
}