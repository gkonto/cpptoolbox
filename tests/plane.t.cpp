#include "plane.hpp"
#include "doctest.h"

TEST_CASE("Normal vector on plane")
{
    float a[3] = {0.0f, 0.0f, 0.0f};
    float b[3] = {1.0f, 0.0f, 0.0f};
    float c[3] = {0.0f, 1.0f, 0.0f};
    float n[3];

    normal(a, b, c, n);

    CHECK(std::abs(n[0] - 0.0f) < 1e-6);
    CHECK(std::abs(n[1] - 0.0f) < 1e-6);
    CHECK(std::abs(n[2] - 1.0f) < 1e-6);
}
