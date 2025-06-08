#ifndef MATRIX_HPP_INCLUDED
#define MATRIX_HPP_INCLUDED

#include <stddef.h>

template<typename T>
void transpose(const T *src, T *dst, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            dst[j * rows + i] = src[i * cols + j];
        }
    }
}

template<typename T>
bool diagonal(const T *src, int dim)
{
    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j)
        {
            if (i != j && src[i][j] != 0)
                return false;
        }
    }
    return true;
}

template<typename T>
bool identity(const T *src, int dim)
{
    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j)
        {
            if (i != j && src[i][j] != 0)
            {
                return false;
            }
            else if (i == j && src[i][j] != 1)
            {
                return false;
            }
        }
    }
    return true;
}

// TODO this can be down faster.
// Iterate only on the upper triangle values.
template<typename T>
bool symmetric(const T *src, int dim)
{

    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j)
        {
            if (i != j && src[i][j] != src[j][i])
            {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
void add(const T *src1, const T *src2, T *trg, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            trg[i][j] = src1[i][j] + src2[i][j];
        }
    }
}

template<typename S, typename T>
void scalarMult(S scalar, const T *matrix, T *trg, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            trg[i][j] = scalar * matrix[i][j];
        }
    }
}

template<typename T, size_t R1, size_t C1, size_t R2, size_t C2>
void mult(const T (&s1)[R1][C1], const T (&s2)[R2][C2], T (&trg)[R1][C2])
{
    static_assert(C1 == R2, "Incompatible matrix dimensions");
    for (int i = 0; i < R1; ++i)
    {
        for (int j = 0; j < C2; ++j)
        {
            trg[i][j] = 0;
            for (int k = 0; k < C1; ++k)
            {
                trg[i][j] += s1[i][k] * s2[k][j];
            }
        }
    }
}

template<typename T>
T det(const T (&m)[1][1])
{
    return m[0][0];
}

template<typename T>
T det(const T (&m)[2][2])
{
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

template<typename T>
T det(const T (&m)[3][3])
{
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) +
           m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

template<typename T>
bool inverse(const T (&m)[2][2], T (&inv)[2][2])
{
    T determinant = det(m);
    if (determinant == 0)
        return false; // Singular matrix

    T inv_det = 1 / determinant;
    inv[0][0] = m[1][1] * inv_det;
    inv[0][1] = -m[0][1] * inv_det;
    inv[1][0] = -m[1][0] * inv_det;
    inv[1][1] = m[0][0] * inv_det;

    return true;
}

template<typename T>
bool inverse(const T (&m)[3][3], T (&inv)[3][3])
{
    T determinant = det(m);
    if (determinant == 0)
        return false; // Singular matrix

    T inv_det = 1 / determinant;

    inv[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * inv_det;
    inv[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) * inv_det;
    inv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * inv_det;

    inv[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * inv_det;
    inv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * inv_det;
    inv[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * inv_det;

    inv[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * inv_det;
    inv[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * inv_det;
    inv[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * inv_det;

    return true;
}


#endif