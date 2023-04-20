
#include "Logger.h"
#include "Allocator.h"
#include "Matrix.h"

#include "moduleTest.h"

LogPriority Logger::priority = TracePriority;

int main()
{
    Logger::setPriority(ErrorPriority);

    Matrix<int> first = {3,4,
                        1,7,-1,1,
                        2,1,2,2,
                        0,2,3,10};

    Matrix<int> second = {3,4,
                        4,7,4,
                        5,3,-1,
                        2,0,2,
                        1,0,1};
    
    Matrix<int> result = first.Transpose() * second;

    print(result, '\n');

    auto RowsComb = result.RowsLinearCombination(10, 0, 1);

    print(RowsComb, '\n');

    auto ColumnComb = result.ColumnsLinearCombination(10, 0, 1);

    print(ColumnComb, '\n');

    int oneNorm = result.OneNorm();

    print(oneNorm, '\n');

    int infNorm = result.InfNorm();

    print(infNorm, '\n');

    auto euclidianNorm = result.EuclideanNorm<float>();

    print(euclidianNorm, '\n', '\n');

    Vector<int> row = {4, 1,2,3,4};

    auto concated = result.AddRow(row);

    print(concated, '\n');

    Vector<int> column = {5, 5,4,3,2,1};

    auto columnConcated = result.AddColumn(column);

    print(columnConcated, '\n');

    auto positionedMatrix = result.AddMatrix(IdentityMatrix<int>(result.GetNumberOfRows()));

    print(positionedMatrix, '\n');

    auto original = positionedMatrix.FirstPartOfMatrix(5);

    print(original, '\n');

    auto Identity = positionedMatrix.SecondPartOfMatrix(5);

    print(Identity, '\n');

    auto det = determinant(original);

    print(det, '\n');

    Matrix<float> newOriginal = Matrix<float>{3,3,
                                            2.78,2,3,
                                            4,5.1,6,
                                            7,8,10};

    auto Inverse = newOriginal.InverseMatrix();
    
    print(Inverse, '\n');

    Matrix<float> newResult = Matrix<float>{newOriginal.GetNumberOfRows(), newOriginal.GetNumberOfColumns(), float()};

    for (Index i = 0; i < 3; i++)
    {
        for (Index j = 0; j < 3; j++)
        {
            newResult[i][j] = float(newOriginal[i][j]);
        }
    }

    auto possibleIdentity = newResult * Inverse;

    print(possibleIdentity, '\n');

    return 0;        
}