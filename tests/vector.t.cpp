#include "doctest.h"
#include "geom_structs.hpp"
#include "math_utils.hpp"

TEST_CASE("Dot product")
{
    Point3d u{1, 2, 3};
    Point3d v{4, 5, 6};
    auto value = dotProd(u, v);
    CHECK(value == 32);
}

TEST_CASE("Cross product")
{
    Point3d u{1, 2, 3};
    Point3d v{4, 5, 6};
    auto cprod = crossProd(u, v);
    CHECK(cprod.x == -3);
    CHECK(cprod.y == 6);
    CHECK(cprod.z == -3);
}
