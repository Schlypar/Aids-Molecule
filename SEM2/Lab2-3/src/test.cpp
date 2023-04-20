
#include "ADT.h"

LogPriority Logger::priority = TracePriority;

int main()
{
    Logger::setPriority(ErrorPriority);

    Matrix<complex> matrix = {2,2,
                                complex{2}, complex{1},
                                complex{-1}, complex{5}};

    // Matrix<float> matrix = {2,2,
    //                         2,1,
    //                         -1,5};

    print(matrix, '\n');

    auto det = determinant(matrix);

    print(det, '\n');

    auto inverse = matrix.InverseMatrix();

    print(inverse, '\n');

    auto possibleIdentity = matrix * inverse;

    print(possibleIdentity, '\n');

    return 0;        
}