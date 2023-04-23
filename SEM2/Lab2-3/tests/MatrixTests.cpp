#include "Logger.h"
#include "moduleTest.h"

#include "Matrix.h"
#include <catch2/catch.hpp>

TEST_CASE("Matrix constructors", "[Matrix]") {

    SECTION("Square matrix constructor") {
        int data = 5;
        int dimension = 3;

        Matrix<int> mat(dimension, data);

        REQUIRE(mat.GetNumberOfRows() == dimension);
        REQUIRE(mat.GetNumberOfColumns() == dimension);

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                REQUIRE(mat.Get(i, j) == data);
            }
        }
    }

    SECTION("Matrix constructor with given dimensions") {
        int data = 3;
        int rows = 2;
        int columns = 3;

        Matrix<int> mat(rows, columns, data);

        REQUIRE(mat.GetNumberOfRows() == rows);
        REQUIRE(mat.GetNumberOfColumns() == columns);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                REQUIRE(mat.Get(i, j) == data);
            }
        }
    }

    SECTION("Matrix constructor with data pointer") {
        int data[] = {1, 2, 3, 4, 5, 6};
        int rows = 2;
        int columns = 3;

        Matrix<int> mat(rows, columns, data);

        REQUIRE(mat.GetNumberOfRows() == rows);
        REQUIRE(mat.GetNumberOfColumns() == columns);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                REQUIRE(mat.Get(i, j) == data[i * columns + j]);
            }
        }
    }

    SECTION("Matrix constructor with data array") {
        int data[] = {1, 2, 3, 4, 5, 6};
        int rows = 2;
        int columns = 3;
        Array<int> arr(data, rows * columns);

        Matrix<int> mat(rows, columns, arr);

        REQUIRE(mat.GetNumberOfRows() == rows);
        REQUIRE(mat.GetNumberOfColumns() == columns);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                REQUIRE(mat.Get(i, j) == data[i * columns + j]);
            }
        }
    }

    SECTION("Matrix constructor with parameter pack") {
        int rows = 2;
        int columns = 3;

        Matrix<int> mat(rows, columns, 1, 2, 3, 4, 5);

        REQUIRE(mat.GetNumberOfRows() == rows);
        REQUIRE(mat.GetNumberOfColumns() == columns);

        REQUIRE(mat.Get(0, 0) == 1);
        REQUIRE(mat.Get(0, 1) == 2);
        REQUIRE(mat.Get(0, 2) == 3);
        REQUIRE(mat.Get(1, 0) == 4);
        REQUIRE(mat.Get(1, 1) == 5);
        REQUIRE(mat.Get(1, 2) == 0); // since only 5 values provided
    }

    SECTION("Copy Constructor")
    {
        // Create a matrix
        Matrix<int> m1(2, 2, 1);
        m1.Set(0, 0, 0);
        m1.Set(1, 1, 2);

        // Copy the matrix
        Matrix<int> m2 = m1;

        // Check if the copy was successful
        REQUIRE(m1.Get(0, 0) == m2.Get(0, 0));
        REQUIRE(m1.Get(1, 1) == m2.Get(1, 1));

        // Modify the original matrix
        m1.Set(1, 1, 3);

        // Check if the copy is still the same
        REQUIRE(m2.Get(1, 1) == 2);
    }

    SECTION("Move Constructor")
    {
        // Create a matrix
        Matrix<int> m1(2, 2, 1);
        m1.Set(0, 0, 0);
        m1.Set(1, 1, 2);

        // Move the matrix
        Matrix<int> m2 = std::move(m1);

        // Check if the move was successful
        REQUIRE(m2.Get(0, 0) == 0);
        REQUIRE(m2.Get(1, 1) == 2);

        // Check if the original matrix was cleared
        REQUIRE(m1.GetNumberOfRows() == 0);
        REQUIRE(m1.GetNumberOfColumns() == 0);
    }
}

TEST_CASE("Matrix assignment operators", "[Matrix]")
{
    SECTION("Copy assignment operator")
    {
        Matrix<int> A(3, 3, 0);
        Matrix<int> B(3, 3, 1);

        A = B;

        REQUIRE(A == B);
    }

    SECTION("Move assignment operator")
    {
        Matrix<int> A(3, 3, 0);
        Matrix<int> B(3, 3, 1);

        A = std::move(B);

        REQUIRE(A[0][0] == 1);
        REQUIRE(B.isEmpty());
    }
}

TEST_CASE("Determinant of 2x2 matrix", "[determinant]") {
    Matrix<int> matrix(2, 2, int());
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 4;

    int det = determinant(matrix);

    REQUIRE(det == -2);
}

TEST_CASE("Determinant of non-square matrix", "[determinant]") {
    Matrix<double> matrix2(2, 3, double());
    matrix2[0][0] = 1;
    matrix2[0][1] = 2;
    matrix2[0][2] = 3;
    matrix2[1][0] = 4;
    matrix2[1][1] = 5;
    matrix2[1][2] = 6;

    try
    {
        auto det = determinant(matrix2);
    }
    catch(Exception e)
    {
        REQUIRE(e == EXCEPTION_BAD_CONTAINER);
    }
}

TEST_CASE("Determinant of 3x3 matrix", "[determinant]") {
    Matrix<float> matrix(3, 3);
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[1][0] = 4;
    matrix[1][1] = 5;
    matrix[1][2] = 6;
    matrix[2][0] = 7;
    matrix[2][1] = 8;
    matrix[2][2] = 9;

    float det = determinant(matrix);

    REQUIRE(det == 0);
}

TEST_CASE("Matrix operator* overload", "[Matrix]") {
    SECTION("Valid multiplication") {
        Matrix<int> m1(2,3, 
                        1, 2, 3, 
                        4, 5, 6);

        Matrix<int> m2(3,2, 
                        7, 8, 
                        9, 10,
                        11, 12);

        Matrix<int> expected(2,2,
                            58, 64, 
                            139, 154);

        Matrix<int> result = m1 * m2;

        REQUIRE(result == expected);
    }

    SECTION("Invalid multiplication") {
        Matrix<int> m1(2,2, 1, 2, 
                        3, 4);

        Matrix<int> m2(2,3, 
                        5, 6, 7, 
                        8, 9, 10);

        try
        {
            auto m3 = m1 * m2;
        }
        catch(Exception e)
        {
            REQUIRE(e == EXCEPTION_BAD_CONTAINER);
        }
    }
}
