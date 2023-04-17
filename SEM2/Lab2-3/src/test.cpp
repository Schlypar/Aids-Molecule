
#include "Logger.h"
#include "Allocator.h"
#include "Matrix.h"

#include "moduleTest.h"

LogPriority Logger::priority = TracePriority;

int main()
{
    Logger::setPriority(ErrorPriority);

    Logger::setPriority(ErrorPriority);
    Matrix<float> first = {3,4,
                        1.5, -8.075, 10.001, -1.1,
                        2.225, -4.4, 2.0, 7.75,
                        -0.75, 3.25, 8.5674, 3.7};

    Matrix<float> second = {4,3,
                        1.5, -10.5, 0.5,
                        0.2567, -10.3, 0.6667,
                        0.99991, 2.1, -2.0,
                        -1.5, 1.25, 1};

    // print(first);
    // // Logger::Debug("Before");
    // // first = first - 5;
    // // Logger::Debug("After");
    // print("\n\n");
    // // print(first);
    // // print("\n\n");
    // print(second);
    // print("\n\n");

    Matrix<float> result = first * second;

    print(result);

    return 0;        
}