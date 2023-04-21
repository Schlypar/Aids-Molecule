
#include "ADT.h"
#include "Matrix.h"
#include <memory>


LogPriority Logger::priority = TracePriority;

int main()
{
    Logger::setPriority(ErrorPriority);

    const Matrix<complex> matrix = {2,2,
                                complex{2, 1}, complex{1, -1},
                                complex{-1, 1}, complex{5, -1}};

    // const Matrix<float> matrix = {2,2,
    //                         2,1,
    //                         -1,5};

    print(matrix, '\n');

    auto augmented = matrix.AddMatrix(IdentityMatrix<complex>(matrix.GetNumberOfColumns()));
    print(augmented, '\n');

    auto triangular = augmented.Triangular();
    print(triangular, '\n');

    auto inverseGauss = triangular.InverseGauss();
    print(inverseGauss, '\n');

    auto result = inverseGauss.SecondPartOfMatrix(2);
    print(result, '\n');

    auto identity = matrix * result;
    print(identity, '\n');

    return 0;        
}