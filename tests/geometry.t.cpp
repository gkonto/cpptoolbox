#include "geometry.hpp"
#include "doctest.h"

TEST_CASE("Point in triangle")
{
    float u, v, w;
    Barycentric(a, b, c, p, u, v, w);
    CHECK(v >= 0.0f && w >= 0.0f && (v + w) <= 1.0f);
}
