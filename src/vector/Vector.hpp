#ifndef VECTOR_HEADER_INCLUDED
#define VECTOR_HEADER_INCLUDED
#include <algorithm>
#include <cstddef>
#include <utility>

template<class T>
class Vector
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = pointer;
    using const_iterator = const_pointer;
private:
    pointer elems{};
    size_type nelems{}, cap{};
public:
    size_type size() const { return nelems; }
    size_type capacity() const { return cap; }
    bool empty() const { return size() == 0; }
    iterator begin() { return elems; }
    const_iterator begin() const { return elems; }
    iterator end() { return begin() + size(); }
    const_iterator end() const { return begin() + size(); }
    const_iterator cend() const { return end(); }
    const_iterator cbegin() const { return begin(); }
    Vector() = default;
    Vector(size_type n, const_reference init)
        : elems{new value_type[n]}, nelems{n}, cap{n}
    {
        try
        {
            std::fill(begin(), end(), init);
        } catch (...)
        {
            delete [] elems;
            throw;
        }
    }
    Vector(const Vector& other)
        : elems{new value_type[other.size()]},
    nelems{other.size()}, cap{other.size()}
    {
        try
        {
            std::copy(other.begin(), other.end(), begin());
        } catch (...)
        {
            delete [] elems;
            throw;
        }
    }
    Vector(Vector&& other) noexcept
        : elems{std::exchange(other.elems, nullptr)},
        nelems{std::exchange(other.nelems, 0)},
        cap{std::exchange(other.cap, 0)} {}
    Vector(std::initializer_list<T> src)
        : elems{new value_type[src.size()]},
        nelems{src.size()}, cap{src.size()}
    {
        try
        {
            std::copy(src.begin(), src.end(), begin());
        } catch (...)
        {
            delete [] elems;
            throw;
        }
    }
    ~Vector()
    {
        delete [] elems;
    }
    void push_back(const_reference val)
    {
        if (full())
            grow();
        elems[size()] = val;
        ++nelems;
    }
    void push_back(T &&val)
    {
        if (full())
            grow();
        elems[size()] = std::move(val);
        ++nelems;
    }
    template<class ...Args>
    reference emplace_back(Args &&...args)
    {
        if (full())
            grow();
        elems[size()] = value_type(std::forward<Args>(args)...);
        ++nelems;
        return back();
    }
    template<class It>
    iterator insert(const_iterator pos, It first, It last)
    {
        iterator pos_ = const_cast<iterator>(pos);
        const std::size_t remaining = capacity() - size();
        const std::size_t n = std::distance(first, last);
        if (remaining < n)
        {
            auto index = std::distance(begin(), pos_);
            resize(capacity() + n - remaining);
            pos_ = std::next(begin(), index);
        }
        std::copy_backward(pos_, end(), end() + n);
        std::copy(first, last, pos_);
        nelems += n;
        return pos_;
    }
    iterator erase(const_iterator pos)
    {
        iterator pos_ = const_char<iterator>(pos);
        if (pos_ == end()) return pos_;
        std::copy(std::next(pos_), end(), pos_);
        *std::prev(end()) = {};
        --nelems;
        return pos_;
    }
private:
    bool full() const { return size() == capacity(); }
    void grow()
    {
        resize(capacity() ? capacity() * 2 : 16);
    }
    void resize(size_type new_cap)
    {
        if (new_cap <= capacity()) return;
        auto p = new T[new_cap];
        if constexpr(std::is_nothrow_move_assignable_v<T>)
        {
            std::move(begin(), end(), p);
        } else try
        {
            std::copy(begin(), end(), p);
        } catch (...)
        {
            delete[]p;
            throw;
        }
        delete []elems;
        elems = p;
        cap = new_cap;
    }
};


#endif
