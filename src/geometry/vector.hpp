#ifndef VECTOR_HPP_INCLUDED
#define VECTOR_HPP_INCLUDED

#include <cmath>
#include <stddef.h>

template<typename T, size_t N>
void sub(const T (&b)[N], const T (&a)[N], T (&trg)[N])
{
    for (int i = 0; i < N; ++i)
    {
        trg[i] = b[i] - a[i];
    }
}

template<typename T>
void sub(const T (&b)[3], const T (&a)[3], T (&trg)[3])
{
    trg[0] = b[0] - a[0];
    trg[1] = b[1] - a[1];
    trg[2] = b[2] - a[2];
}

template<typename T, size_t N>
T dotProd(const T (&u)[N], const T (&v)[N])
{
    T value = 0;
    for (size_t i = 0; i < N; ++i)
    {
        value += u[i] * v[i];
    }
    return value;
}

template<typename T, size_t N>
void crossProd(const T (&u)[N], const T (&v)[N], T (&trg)[N])
{
    T t1 = u[0] - u[1];
    T t2 = v[1] + v[2];
    T t3 = u[0] * v[2];
    T t4 = t1 * t2 - t3;
    trg[0] = v[1] * (t1 - u[2]) - t4;
    trg[1] = u[2] * v[0] - t3;
    trg[2] = t4 - u[1] * (v[0] - t2);
}

template<typename T>
T magnitute(const T (&v)[3])
{
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}


template<typename T>
void normalize(const T (&v)[3], T (&n)[3])
{
    T magn = magnitute(v);

    if (magn != 0)
    {
        n[0] = v[0] / magn;
        n[1] = v[1] / magn;
        n[2] = v[2] / magn;
    }
}

#endif