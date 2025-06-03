#ifndef MALLOCATOR_HPP_INCLUDED
#define MALLOCATOR_HPP_INCLUDED

#include <cstddef>
#include <limits>
#include <new>
#include <stdlib.h>

template<typename T>
struct Mallocator
{
public:
    using value_type = T;
    Mallocator() = default;

    template<class U>
    Mallocator(const Mallocator<U> &) noexcept
    {}

    template<class U>
    auto operator==(const Mallocator<U> &) const noexcept
    {
        return true;
    }

    template<class U>
    auto operator!=(const Mallocator<U> &) const noexcept
    {
        return false;
    }

    auto allocate(size_t n) const -> T *
    {
        if (n == 0)
        {
            return nullptr;
        }
        if (n > std::numeric_limits<size_t>::max() / sizeof(T))
        {
            throw std::bad_array_new_length{};
        }
        void *const pv = malloc(n * sizeof(T));
        if (pv == nullptr)
        {
            throw std::bad_alloc{};
        }
        return static_cast<T *>(pv);
    }

    auto deallocate(T *p, size_t) const noexcept -> void
    {
        free(p);
    }
};

#endif