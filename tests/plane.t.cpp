#include "doctest.h"
#include "geometry.hpp"

TEST_CASE("Normal vector on plane")
{
    float a[3] = {0.0f, 0.0f, 0.0f};
    float b[3] = {1.0f, 0.0f, 0.0f};
    float c[3] = {0.0f, 1.0f, 0.0f};

    auto n = normal(a, b, c);

    CHECK(std::abs(n.x - 0.0f) < 1e-6);
    CHECK(std::abs(n.y - 0.0f) < 1e-6);
    CHECK(std::abs(n.z - 1.0f) < 1e-6);
}
