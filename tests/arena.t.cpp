#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "arena.hpp"
#include "doctest.h"

TEST_CASE("Arena size and default state")
{
    Arena<128> arena;
    CHECK(arena.size() == 128);
    CHECK(arena.used() == 0);
}

TEST_CASE("Arena reset sets pointer to beginning")
{
    Arena<128> arena;
    auto *p1 = arena.allocate(16);
    CHECK(arena.used() > 0);
    arena.reset();
    CHECK(arena.used() == 0);
    auto *p2 = arena.allocate(16);
    CHECK(p1 == p2); // allocation reuses the same region
}

TEST_CASE("Arena allocate returns properly aligned pointer")
{
    Arena<128> arena;
    for (int i = 0; i < 10; ++i)
    {
        auto *p = arena.allocate(1);
        auto addr = reinterpret_cast<std::uintptr_t>(p);
        CHECK((addr % alignof(std::max_align_t)) == 0);
    }
}

TEST_CASE("Arena handles in-buffer deallocation correctly")
{
    Arena<128> arena;

    auto *p1 = arena.allocate(16);
    auto *p2 = arena.allocate(16);
    CHECK(p2 > p1);
    auto used = arena.used();

    arena.deallocate(p2, 16); // should deallocate last block
    CHECK(arena.used() < used);

    arena.deallocate(p1, 16); // also valid
    CHECK(arena.used() == 0);
}

TEST_CASE("Arena ignores non-last in-buffer deallocation")
{
    Arena<128> arena;

    auto *p1 = arena.allocate(16);
    auto *p2 = arena.allocate(16);

    arena.deallocate(p1, 16); // not LIFO — should do nothing
    CHECK(arena.used() != 0); // still used
}

TEST_CASE("Arena falls back to heap when full")
{
    Arena<32> arena;
    auto *p1 = arena.allocate(64); // too large

    CHECK(p1 != nullptr);
    CHECK(!arena.pointer_in_buffer(p1)); // should be outside buffer

    arena.deallocate(p1, 64); // should go to ::operator delete
    CHECK(true);              // if we got here, delete didn't crash
}

TEST_CASE("Arena pointer_in_buffer is correct")
{
    Arena<64> arena;
    auto *p = arena.allocate(8);

    CHECK(arena.pointer_in_buffer(p));

    std::byte dummy;
    CHECK(!arena.pointer_in_buffer(&dummy));
}

TEST_CASE("Arena proper alignment")
{
    constexpr size_t SIZE = alignof(std::max_align_t) * 2;
    Arena<SIZE> arena;
    CHECK(arena.size() == SIZE);
    auto *p = arena.allocate(1);
    CHECK(arena.pointer_in_buffer(p));
    auto *p2 = arena.allocate(1);
    CHECK(arena.pointer_in_buffer(p2));
    CHECK(p2 - p == alignof(std::max_align_t));
}