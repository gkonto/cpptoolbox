#include "doctest.h"
#include "math_utils.hpp"
#include <cmath>

bool are_equal(double d1, double d2)
{
    return std::abs(d1 - d2) < 0.6;
}

TEST_CASE("Transpose matrix")
{
    constexpr int rows = 2, cols = 3;
    int matrix[rows][cols] = {{1, 2, 3}, {4, 5, 6}};
    int transposed[cols][rows];

    transpose(&matrix[0][0], &transposed[0][0], rows, cols);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            CHECK(matrix[i][j] == transposed[j][i]);
        }
    }
}

TEST_CASE("Matrix is diagonal")
{
    constexpr int rows = 3, cols = 3;
    int matrix[rows][cols] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    CHECK(diagonal(matrix, 3));
}

TEST_CASE("Matrix is not diagonal")
{
    constexpr int rows = 3, cols = 3;
    int matrix[rows][cols] = {{1, 1, 1}, {0, 1, 0}, {0, 0, 1}};
    CHECK(!diagonal(matrix, 3));
}

TEST_CASE("One-dimensional 2D matrix is diagonal")
{
    constexpr int rows = 3, cols = 3;
    int matrix[rows][cols] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    CHECK(diagonal(matrix, 3));
}

TEST_CASE("One-dimensional 2D matrix is not diagonal")
{
    constexpr int rows = 3, cols = 3;
    int matrix[rows][cols] = {1, 2, 3, 4, 1, 5, 6, 7, 1};
    CHECK(!diagonal(matrix, 3));
}

TEST_CASE("Check identity matrix")
{
    constexpr int dim = 3;
    int matrix[dim][dim] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    CHECK(identity(matrix, 3));

    int matrix2[dim][dim] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    CHECK(!identity(matrix2, 3));
}

TEST_CASE("Check matrix is symmetric")
{
    constexpr int dim = 3;
    int matrix[dim][dim] = {{1, 2, 3}, {2, 4, 5}, {3, 5, 6}};
    CHECK(symmetric(matrix, dim));

    int matrix2[dim][dim] = {{1, 2, 1}, {1, 4, 5}, {3, 5, 6}};
    CHECK(!symmetric(matrix2, dim));
}

TEST_CASE("Add matrix")
{
    int rows = 2, cols = 3;
    int matrix1[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int matrix2[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int trg[2][3] = {0};

    add(matrix1, matrix2, trg, 2, 3);

    CHECK(trg[0][0] == 2);
    CHECK(trg[0][1] == 4);
    CHECK(trg[0][2] == 6);
    CHECK(trg[1][0] == 8);
    CHECK(trg[1][1] == 10);
    CHECK(trg[1][2] == 12);
}

TEST_CASE("Scalar - matrix multiplication")
{
    int rows = 2, cols = 3;
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    scalarMult(10, matrix, matrix, 2, 3);
    CHECK(matrix[0][0] == 10);
    CHECK(matrix[0][1] == 20);
    CHECK(matrix[0][2] == 30);
    CHECK(matrix[1][0] == 40);
    CHECK(matrix[1][1] == 50);
    CHECK(matrix[1][2] == 60);
}

TEST_CASE("Matrix multiplication")
{
    int rows = 2, cols = 3;
    int m1[2][3] = {{3, 0, -5}, {2, -1, -4}};
    int m2[3][2] = {{3, -1}, {4, 7}, {2, -2}};
    int trg[2][2] = {{0, 0}, {0, 0}};

    mult(m1, m2, trg);

    CHECK(trg[0][0] == -1);
    CHECK(trg[0][1] == 7);
    CHECK(trg[1][0] == -6);
    CHECK(trg[1][1] == -1);
}

TEST_CASE("Matrix Determinant")
{
    int mat1[1][1] = {{5}};
    int mat2[2][2] = {{4, 6}, {3, 8}};
    int mat3[3][3] = {{1, 2, 3}, {0, 4, 5}, {1, 0, 6}};
    CHECK(det(mat1) == 5);
    CHECK(det(mat2) == 14);
    CHECK(det(mat3) == 22);
}

TEST_CASE("Inverse of 2x2 matrix")
{
    double m[2][2] = {{4, 7}, {2, 6}};
    double inv[2][2] = {};
    bool result = inverse(m, inv);

    CHECK(result == true);
    CHECK(are_equal(inv[0][0], 0.6));
    CHECK(are_equal(inv[0][1], -0.7));
    CHECK(are_equal(inv[1][0], -0.2));
    CHECK(are_equal(inv[1][1], 0.4));
}

TEST_CASE("Inverse of 3x3 matrix")
{
    double m[3][3] = {{3, 0, 2}, {2, 0, -2}, {0, 1, 1}};
    double inv[3][3] = {};
    bool result = inverse(m, inv);

    CHECK(result == true);
    CHECK(are_equal(inv[0][0], 0.2));
    CHECK(are_equal(inv[0][1], 0.2));
    CHECK(are_equal(inv[0][2], 0.0));
    CHECK(are_equal(inv[1][0], -0.2));
    CHECK(are_equal(inv[1][1], 0.3));
    CHECK(are_equal(inv[1][2], 1.0));
    CHECK(are_equal(inv[2][0], 0.2));
    CHECK(are_equal(inv[2][1], -0.3));
    CHECK(are_equal(inv[2][2], 0.0));
}

TEST_CASE("Inverse of singular (non-invertible) matrix")
{
    double m[2][2] = {{2, 4}, {1, 2}}; // determinant is 0
    double inv[2][2] = {};
    bool result = inverse(m, inv);

    CHECK(result == false);
}


// TEST_CASE("Point in triangle")
// {
//     float u, v, w;
//     Barycentric(a, b, c, p, u, v, w);
//     CHECK(v >= 0.0f && w >= 0.0f && (v + w) <= 1.0f);
// }
