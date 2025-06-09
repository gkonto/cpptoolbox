#include "vector.hpp"
#include "doctest.h"

TEST_CASE("Dot product")
{
    int u[3] = {1, 2, 3};
    int v[3] = {4, 5, 6};
    auto value = dotProd(u, v);
    CHECK(value == 32);
}

TEST_CASE("Cross product")
{
    int u[3] = {1, 2, 3};
    int v[3] = {4, 5, 6};
    int cprod[3] = {0};
    crossProd(u, v, cprod);
    CHECK(cprod[0] == -3);
    CHECK(cprod[1] == 6);
    CHECK(cprod[2] == -3);
}
