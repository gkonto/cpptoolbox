#ifndef PLANE_HPP_INCLUDED
#define PLANE_HPP_INCLUDED
#include "vector.hpp"

template<typename T>
void normal(const T (&a)[3], const T (&b)[3], const T (&c)[3], T (&n)[3])
{
    T v0[3], v1[3];
    sub(b, a, v0);
    sub(c, a, v1);
    crossProd(v0, v1, n);
    normalize(n, n);
}
#endif