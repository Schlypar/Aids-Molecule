
#include "Logger.h"
#include "Allocator.h"
#include "Matrix.h"

#include "moduleTest.h"

LogPriority Logger::priority = TracePriority;

int main()
{
    Logger::setPriority(ErrorPriority);

    Matrix<int> first = {3,4,
                        1,1,1,1,
                        2,2,2,2,
                        3,3,3,3};

    Matrix<int> second = {3,4,
                        4,4,4,4,
                        3,3,3,3,
                        2,2,2,2};

    
    Matrix<int> result = first.Transpose() * second;


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

    auto GaussFirstRound = result.GaussMethod<float>();

    print(GaussFirstRound, '\n');

    return 0;        
}