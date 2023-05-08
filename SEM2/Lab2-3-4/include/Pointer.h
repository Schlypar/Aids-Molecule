#pragma once

#include <cstddef>
#include <utility>

template <class T>
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

	UniquePtr<T>& operator=(UniquePtr<T>&& other) noexcept
	{
		this->swap(other);

		return *this;
	}

	UniquePtr<T>& operator=(std::nullptr_t) noexcept
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
	UniquePtr<T>& operator=(UniquePtr<U>&& other) noexcept
	{
		UniquePtr<T> temp(other.release());
		temp.swap(*this);
		return *this;
	}

	UniquePtr(const UniquePtr<T>& other) = delete;
	UniquePtr<T>& operator=(const UniquePtr<T>& other) = delete;

	T* operator->() const
	{
		return ptr;
	}

	T& operator*() const
	{
		return *ptr;
	}

	operator bool() const
	{
		return ptr;
	}

	bool operator==(T* other) const
	{
		return ptr == other;
	}

	bool operator==(std::nullptr_t) const
	{
		return ptr == nullptr;
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

template <class T>
void swap(UniquePtr<T>& left, UniquePtr<T>& right)
{
	left.swap(right);
}

template <class T>
class SharedPtr
{
private:
	T* ptr;
	int counter;

public:
	SharedPtr() noexcept
	    : ptr()
	    , counter(0)
	{
	}

	SharedPtr(std::nullptr_t) noexcept
	    : ptr(nullptr)
	    , counter(0)
	{
	}

	SharedPtr(T* object) noexcept
	    : ptr(object)
	    , counter(1)
	{
	}

	SharedPtr(const SharedPtr<T>& other) noexcept
	    : ptr(other.ptr)
	    , counter(other.counter + 1)
	{
	}

	~SharedPtr() noexcept
	{
		counter--;

		if (counter <= 0)
			delete ptr;
	}

	SharedPtr<T>& operator=(const SharedPtr<T>& other) noexcept
	{
		this->ptr = other.ptr;
		this->counter = other.counter + 1;

		return *this;
	}

	SharedPtr<T>& operator=(std::nullptr_t) noexcept
	{
		reset();

		return *this;
	}

	SharedPtr(SharedPtr<T>&& other)
	    : ptr(other.ptr)
	    , counter(other.counter)
	{
		other.ptr = nullptr;
		other.counter = 0;
	}

	SharedPtr<T>& operator=(SharedPtr<T>&& other)
	{
		if (this->ptr)
			delete ptr;

		ptr = other.ptr;
		counter = other.counter;

		other.ptr = nullptr;
		other.counter = 0;

		return *this;
	}

	T* operator->() const
	{
		return ptr;
	}

	T& operator*() const
	{
		return *ptr;
	}

	operator bool() const
	{
		return ptr;
	}

	bool operator==(T* other) const
	{
		return ptr == other;
	}

	bool operator==(std::nullptr_t) const
	{
		return ptr == nullptr;
	}

	T* release() noexcept
	{
		T* result = nullptr;
		std::swap(result, ptr);
		counter = 0;
		return result;
	}

	void swap(SharedPtr<T>& other) noexcept
	{
		std::swap(ptr, other.ptr);
		std::swap(counter, other.counter);
	}

private:
	void reset() noexcept
	{
		T* temp = release();
		counter = 0;
		delete temp;
	}
};

template <class T>
void swap(SharedPtr<T>& left, SharedPtr<T>& right)
{
	left.swap(right);
}

template <class T>
class WeakPtr
{
private:
	T* ptr;

public:
	WeakPtr() noexcept
	    : ptr()
	{
	}

	WeakPtr(std::nullptr_t) noexcept
	    : ptr(nullptr)
	{
	}

	WeakPtr(T* object) noexcept
	    : ptr(object)
	{
	}

	WeakPtr(const WeakPtr<T>& other) noexcept
	    : ptr(other.ptr)
	{
	}

	~WeakPtr() noexcept
	{
		delete ptr;
	}

	WeakPtr<T>& operator=(const WeakPtr<T>& other) noexcept
	{
		this->ptr = other.ptr;

		return *this;
	}

	WeakPtr<T>& operator=(std::nullptr_t) noexcept
	{
		reset();

		return *this;
	}

	WeakPtr(WeakPtr<T>&& other)
	{
		if (this->ptr)
			delete ptr;

		ptr = other.ptr;
		other.ptr = nullptr;
	}

	WeakPtr<T>& operator=(WeakPtr<T>&& other)
	{
		if (this->ptr)
			delete ptr;

		ptr = other.ptr;
		other.ptr = nullptr;

		return *this;
	}

	T* operator->() const
	{
		return ptr;
	}

	T& operator*() const
	{
		return *ptr;
	}

	operator bool() const
	{
		return ptr;
	}

	bool operator==(T* other) const
	{
		return ptr == other;
	}

	bool operator==(std::nullptr_t) const
	{
		return ptr == nullptr;
	}

	T* release() noexcept
	{
		T* result = nullptr;
		std::swap(result, ptr);
		return result;
	}

	void swap(WeakPtr<T>& other) noexcept
	{
		std::swap(ptr, other.ptr);
	}

	void reset() noexcept
	{
		T* temp = release();
		delete temp;
	}
};

template <class T>
void swap(WeakPtr<T>& left, WeakPtr<T>& right)
{
	left.swap(right);
}