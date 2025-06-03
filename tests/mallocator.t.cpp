//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "mallocator.hpp"
#include "doctest.h"
#include <vector>

TEST_CASE("Mallocator: Allocate single int")
{
    Mallocator<int> alloc;
    int *ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    *ptr = 42;
    CHECK(*ptr == 42);
    alloc.deallocate(ptr, 1);
}

TEST_CASE("Mallocator: Allocate multiple ints")
{
    Mallocator<int> alloc;
    size_t count = 5;
    int *ptr = alloc.allocate(count);
    REQUIRE(ptr != nullptr);
    for (size_t i = 0; i < count; ++i)
        ptr[i] = static_cast<int>(i * 2);
    for (size_t i = 0; i < count; ++i)
        CHECK(ptr[i] == static_cast<int>(i * 2));
    alloc.deallocate(ptr, count);
}

TEST_CASE("Mallocator: Allocate zero elements returns nullptr")
{
    Mallocator<int> alloc;
    int *ptr = alloc.allocate(0);
    CHECK(ptr == nullptr);
}

TEST_CASE("Mallocator: Comparison operators")
{
    Mallocator<int> a1;
    Mallocator<float> a2;
    CHECK(a1 == a2);
    CHECK_FALSE(a1 != a2);
}

TEST_CASE("Mallocator: Use with std::vector")
{
    std::vector<int, Mallocator<int>> vec;
    for (int i = 0; i < 4; ++i)
    {
        vec.push_back(i + 10);
    }
    CHECK(vec.size() == 4);
    CHECK(vec[1] == 11);
}
