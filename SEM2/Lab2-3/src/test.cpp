
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

    result = Matrix<int>{3,3,
                        2,2,3,
                        4,5,6,
                        7,8,10};

    print(result, '\n');

    // auto RowsComb = result.RowsLinearCombination(10, 0, 1);

    // print(RowsComb, '\n');

    // auto ColumnComb = result.ColumnsLinearCombination(10, 0, 1);

    // print(ColumnComb, '\n');

    // int oneNorm = result.OneNorm();

    // print(oneNorm, '\n');

    // int infNorm = result.InfNorm();

    // print(infNorm, '\n');

    // auto euclidianNorm = result.EuclideanNorm<float>();

    // print(euclidianNorm, '\n');

    // Vector<int> row = {3, 1,2,3};

    // auto concated = result.AddRow(row);

    // print(concated, '\n');

    // Vector<int> column = {3, 3,2,1};

    // auto columnConcated = result.AddColumn(column);

    // print(columnConcated, '\n');

    // auto positionedMatrix = result.AddMatrix(IdentityMatrix<int>(result.GetNumberOfRows()));

    // print(positionedMatrix, '\n');

    // auto original = positionedMatrix.FirstPartOfMatrix(result.GetNumberOfColumns());

    // print(original, '\n');

    // auto Identity = positionedMatrix.SecondPartOfMatrix(result.GetNumberOfColumns());

    // print(Identity, '\n');

    auto det = determinant(result);

    print(det, '\n', '\n');

    auto Inverse = result.InverseMatrix<float>();
    
    print(Inverse, '\n');

    Matrix<float> newResult = Matrix<float>{3,3, float()};

    for (Index i = 0; i < 3; i++)
    {
        for (Index j = 0; j < 3; j++)
        {
            newResult[i][j] = float(result[i][j]);
        }
    }

    auto Identity = newResult * Inverse;

    print(Identity, '\n');

    return 0;        
}