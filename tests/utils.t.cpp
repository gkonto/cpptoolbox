#include "utils.hpp"
#include "doctest.h"

TEST_CASE("Test isConvexQuad function")
{
    float quad1[4][3] = {
        {0, 0, 0}, // A
        {1, 0, 0}, // B
        {1, 1, 0}, // C
        {0, 1, 0}  // D
    };
    CHECK(isConvexQuad(quad1[0], quad1[1], quad1[2], quad1[3]) == true);

    float quad2[4][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 1, 0},
        {0.5, 0.5, 0} // Inside the triangle — makes a concave quad
    };
    CHECK(isConvexQuad(quad2[0], quad2[1], quad2[2], quad2[3]) == false);

    float quad3[4][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 1, 1}, // lifted C into Z axis
        {0, 1, 1}  // lifted D too — still convex if projected properly
    };
    CHECK(isConvexQuad(quad3[0], quad3[1], quad3[2], quad3[3]) == true);
}