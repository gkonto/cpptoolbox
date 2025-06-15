#include "doctest.h"
#include "geom_structs.hpp"
#include "geometry.hpp"

TEST_CASE("Identical boxes should intersect")
{
    AABB3d a = {{0, 0, 0}, {1, 1, 1}};
    AABB3d b = {{0, 0, 0}, {1, 1, 1}};
    CHECK(intersection(a, b));
}

TEST_CASE("Overlapping boxes should intersect")
{
    AABB3d a = {{0, 0, 0}, {2, 2, 2}};
    AABB3d b = {{1, 1, 1}, {1, 1, 1}};
    CHECK(intersection(a, b));
}

TEST_CASE("Touching boxes along one axis should intersect")
{
    AABB3d a = {{0, 0, 0}, {1, 1, 1}};
    AABB3d b = {{2, 0, 0}, {1, 1, 1}}; // Touching at x = 1
    CHECK(intersection(a, b));
}

TEST_CASE("Boxes too far apart should not intersect")
{
    AABB3d a = {{0, 0, 0}, {1, 1, 1}};
    AABB3d b = {{3, 0, 0}, {1, 1, 1}}; // Gap along x-axis
    CHECK_FALSE(intersection(a, b));
}

TEST_CASE("Corner-touching boxes should intersect")
{
    AABB3d a = {{0, 0, 0}, {1, 1, 1}};
    AABB3d b = {{2, 2, 2}, {1, 1, 1}}; // Touches at (1,1,1)
    CHECK(intersection(a, b));
}

TEST_CASE("One box entirely inside another should intersect")
{
    AABB3d outer = {{0, 0, 0}, {3, 3, 3}};
    AABB3d inner = {{0, 0, 0}, {1, 1, 1}};
    CHECK(intersection(outer, inner));
}


TEST_CASE("UpdateAABB - Identity Transform")
{
    AABB3d a = {{1.0f, 2.0f, 3.0f}, {1.0f, 1.0f, 1.0f}};
    float m[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    float t[3] = {0, 0, 0};
    AABB3d b;

    UpdateAABB(a, m, t, b);

    CHECK(b.c.x == doctest::Approx(1.0f));
    CHECK(b.c.y == doctest::Approx(2.0f));
    CHECK(b.c.z == doctest::Approx(3.0f));
    CHECK(b.r[0] == doctest::Approx(1.0f));
    CHECK(b.r[1] == doctest::Approx(1.0f));
    CHECK(b.r[2] == doctest::Approx(1.0f));
}

TEST_CASE("UpdateAABB - Translation Only")
{
    AABB3d a = {{0, 0, 0}, {1, 1, 1}};
    float m[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    float t[3] = {5, -3, 2};
    AABB3d b;

    UpdateAABB(a, m, t, b);

    CHECK(b.c.x == doctest::Approx(5.0f));
    CHECK(b.c.y == doctest::Approx(-3.0f));
    CHECK(b.c.z == doctest::Approx(2.0f));
    CHECK(b.r[0] == doctest::Approx(1.0f));
    CHECK(b.r[1] == doctest::Approx(1.0f));
    CHECK(b.r[2] == doctest::Approx(1.0f));
}

TEST_CASE("UpdateAABB - Rotation and Scale")
{
    AABB3d a = {{0, 0, 0}, {1, 2, 3}};
    float m[3][3] = {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}};
    float t[3] = {0, 0, 0};
    AABB3d b;

    UpdateAABB(a, m, t, b);

    // center remains 0 since a.c is 0 and t is 0
    CHECK(b.c.x == doctest::Approx(0.0f));
    CHECK(b.c.y == doctest::Approx(0.0f));
    CHECK(b.c.z == doctest::Approx(0.0f));

    // radii are affected by abs(rotation)
    CHECK(b.r[0] == doctest::Approx(2.0f));
    CHECK(b.r[1] == doctest::Approx(1.0f));
    CHECK(b.r[2] == doctest::Approx(3.0f));
}

TEST_CASE("Sphere intersection tests")
{
    Sphere a = {{0, 0, 0}, 1.0f};

    SUBCASE("Spheres overlap")
    {
        Sphere b = {{1, 0, 0}, 1.0f}; // Distance = 1, radiusSum = 2
        CHECK(intersection(a, b) == true);
    }

    SUBCASE("Spheres just touch")
    {
        Sphere b = {{2, 0, 0}, 1.0f}; // Distance = 2, radiusSum = 2
        CHECK(intersection(a, b) == true);
    }

    SUBCASE("Spheres do not intersect")
    {
        Sphere b = {{3, 0, 0}, 1.0f}; // Distance = 3, radiusSum = 2
        CHECK(intersection(a, b) == false);
    }

    SUBCASE("One sphere inside another")
    {
        Sphere b = {{0, 0, 0}, 0.5f}; // Same center
        CHECK(intersection(a, b) == true);
    }

    SUBCASE("Identical spheres")
    {
        Sphere b = {{0, 0, 0}, 1.0f}; // Exactly the same
        CHECK(intersection(a, b) == true);
    }
}
