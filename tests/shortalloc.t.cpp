//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "shortalloc.hpp"
#include "arena.hpp"
#include "barycentric.hpp"
#include "doctest.h"
#include <string>
#include <type_traits>
#include <vector>

TEST_CASE("Arena basic allocation within buffer")
{
    Arena<256> arena;

    std::byte *p1 = arena.allocate(32);
    std::byte *p2 = arena.allocate(64);

    REQUIRE(p1 != nullptr);
    REQUIRE(p2 != nullptr);
    CHECK(arena.pointer_in_buffer(p1));
    CHECK(arena.pointer_in_buffer(p2));
    CHECK(p2 > p1);
}

TEST_CASE("Arena allocation exceeding buffer size falls back to heap")
{
    Arena<64> arena;
    std::byte *p = arena.allocate(512); // intentionally large

    REQUIRE(p != nullptr);
    CHECK_FALSE(arena.pointer_in_buffer(p));

    arena.deallocate(p, 512); // should delete heap memory
}

TEST_CASE("Arena deallocate moves ptr_ back when LIFO")
{
    Arena<128> arena;
    std::byte *p1 = arena.allocate(32);
    std::byte *p2 = arena.allocate(32);

    arena.deallocate(p2, 32); // p2 was the last allocation, should reduce ptr_
    CHECK(arena.used() <= 64);

    arena.deallocate(p1, 32); // p1 now at top, so reset possible again
    CHECK(arena.used() == 0);
}

TEST_CASE("Arena reset resets ptr_ to buffer start")
{
    Arena<128> arena;
    (void)arena.allocate(64);
    CHECK(arena.used() > 0);

    arena.reset();
    CHECK(arena.used() == 0);
}

TEST_CASE("ShortAlloc: Basic usage")
{
    Arena<512> arena;
    ShortAlloc<int, 512> alloc(arena);

    int *p = alloc.allocate(4);
    for (int i = 0; i < 4; ++i)
        p[i] = i * 2;

    for (int i = 0; i < 4; ++i)
        CHECK(p[i] == i * 2);

    alloc.deallocate(p, 4);
}

TEST_CASE("ShortAlloc: Comparison and rebind")
{
    Arena<256> arena1;
    Arena<256> arena2;

    ShortAlloc<int, 256> a1(arena1);
    ShortAlloc<int, 256> a2(arena1);
    ShortAlloc<int, 256> a3(arena2);
    ShortAlloc<float, 256> a4(arena1);

    CHECK(a1 == a2);
    CHECK(a1 != a3);
    CHECK(a1 == a4);

    typename ShortAlloc<int, 256>::template rebind<double>::other rebound(
        arena1);
    static_assert(std::is_same_v<decltype(rebound), ShortAlloc<double, 256>>,
                  "Rebind failed");
}

TEST_CASE("ShortAlloc with std::vector")
{
    Arena<1024> arena;
    using Alloc = ShortAlloc<std::string, 1024>;
    std::vector<std::string, Alloc> vec{Alloc(arena)};

    vec.emplace_back("Short");
    vec.emplace_back("Alloc");
    vec.emplace_back("Test");

    REQUIRE(vec.size() == 3);
    CHECK(vec[0] == "Short");
    CHECK(vec[1] == "Alloc");
    CHECK(vec[2] == "Test");
}
