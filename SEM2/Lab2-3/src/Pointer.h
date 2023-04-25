#pragma once

#include <cstddef>
#include <utility>


template <typename T>
class UniquePtr
{
private:
    T* ptr;
public:
    UniquePtr() noexcept
        : ptr(nullptr)
    {
    }

    UniquePtr(std::nullptr_t) noexcept
        : ptr(nullptr)
    {
    }

    UniquePtr(T* object) noexcept
        : ptr(object)
    {
    }

    UniquePtr(UniquePtr<T>&& other) noexcept
        : ptr(nullptr)
    {
        this->swap(other);
    }

    ~UniquePtr()
    {
        delete ptr;
    }

    UniquePtr<T>& operator= (UniquePtr<T>&& other) noexcept
    {
        this->swap(other);

        return *this;
    }

    UniquePtr<T>& operator= (std::nullptr_t) noexcept
    {
        reset();

        return *this;
    }

    template <typename U>
    UniquePtr(UniquePtr<U>&& other) noexcept
    {
        UniquePtr<T> temp(other.release());
        temp.swap(*this);
    }

    template <typename U>
    UniquePtr<T>& operator= (UniquePtr<U>&& other) noexcept
    {
        UniquePtr<T> temp(other.release());
        temp.swap(*this);
        return *this;
    }

    UniquePtr(const UniquePtr<T>& other) = delete;
    UniquePtr<T>& operator= (const UniquePtr<T>& other) = delete;

    T* operator-> () const
    {
        return ptr;
    }

    T& operator* () const
    {
        return *ptr;
    }

    operator bool() const
    {
        return ptr;
    }

    T* release() noexcept
    {
        T* result = nullptr;
        std::swap(result, ptr);
        return result;
    }

    void swap(UniquePtr<T>& other) noexcept
    {
        std::swap(ptr, other.ptr);
    }

    void reset() noexcept
    {
        T* temp = release();
        delete temp;
    }
};

template <typename T>
void swap (UniquePtr<T>& left, UniquePtr<T>& right)
{
    left.swap(right);
}