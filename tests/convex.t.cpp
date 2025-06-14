#include "convex.hpp"
#include "doctest.h"
#include <span>
TEST_CASE("pointFarthestFromEdge basic test")
{
    float a[2] = {0, 0};
    float b[2] = {1, 0};

    std::array<float[2], 3> points = {{
        {0.5f, 1.0f},  // Above the edge (left side if edge is left to right)
        {0.5f, -1.0f}, // Below the edge (right side)
        {0.5f, 2.0f}   // Farthest above
    }};

    CHECK(pointFarthestFromEdge(
              a,
              b,
              std::span<const float[2]>(points.begin(), points.end())) == 2);
}

TEST_CASE("pointFarthestFromEdge with equal distance, test rightMostVal")
{
    float a[2] = {0, 0};
    float b[2] = {2, 0};

    std::array<float[2], 2> points = {{
        {0.5f, 1.0f},
        {1.5f, 1.0f} // Same vertical distance, farther along the edge
    }};

    CHECK(pointFarthestFromEdge(
              a,
              b,
              std::span<const float[2]>(points.begin(), points.end())) == 1);
}

TEST_CASE("pointFarthestFromEdge all points on right or colinear")
{
    float a[2] = {0, 0};
    float b[2] = {1, 0};

    std::array<const float[2], 3> points = {{
        {0.5f, -1.0f}, // Right side
        {1.0f, 0.0f},  // On the edge
        {2.0f, -1.0f}  // Right side
    }};

    CHECK(pointFarthestFromEdge(
              a,
              b,
              std::span<const float[2]>(points.begin(), points.end())) ==
          1); // Technically all are <= 0 in d
}

TEST_CASE("pointFarthestFromEdge empty point set")
{
    float a[2] = {0, 0};
    float b[2] = {1, 0};

    std::span<const float[2]> emptyPoints;
    CHECK(pointFarthestFromEdge(a, b, emptyPoints) == static_cast<size_t>(-1));
}