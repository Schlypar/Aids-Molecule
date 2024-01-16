
#pragma once

#include <cstddef>
#include <optional>

template <typename T>
class UniquePtr
{
private:
	T* ptr;

public:
	UniquePtr()
	    : ptr(nullptr)
	{
	}

	UniquePtr(std::nullptr_t)
	    : ptr(nullptr)
	{
	}

	UniquePtr(T* object)
	    : ptr(object)
	{
	}

	UniquePtr(UniquePtr<T>&& other)
	    : ptr(other.ptr)
	{
		other.ptr = nullptr;
	}

	~UniquePtr()
	{
		delete ptr;
	}

	UniquePtr<T>& operator=(UniquePtr<T>&& other)
	{
		this->swap(other);
		return *this;
	}

	UniquePtr<T>& operator=(std::nullptr_t)
	{
		reset();
		return *this;
	}

	UniquePtr(const UniquePtr<T>& other) = delete;
	UniquePtr<T>& operator=(const UniquePtr<T>& other) = delete;

	T* operator->()
	{
		return ptr;
	}

	T& operator*()
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

	operator T()
	{
		return ptr;
	}

	T* Get()
	{
		return ptr;
	}

	T* release()
	{
		T* res = nullptr;
		std::swap(res, ptr);
		return res;
	}

	void swap(UniquePtr<T>& other)
	{
		std::swap(ptr, other.ptr);
	}

	void reset()
	{
		delete ptr;
		ptr = nullptr;
	}
};

template <typename T>
class UniquePtr<T[]>
{
private:
	T* ptr;

public:
	UniquePtr()
	    : ptr(nullptr)
	{
	}

	UniquePtr(std::nullptr_t)
	    : ptr(nullptr)
	{
	}

	UniquePtr(T* object)
	    : ptr(object)
	{
	}

	UniquePtr(UniquePtr<T[]>&& other)
	    : ptr(other.ptr)
	{
		other.ptr = nullptr;
	}

	~UniquePtr()
	{
		delete[] ptr;
	}

	UniquePtr<T[]>& operator=(UniquePtr<T[]>&& other)
	{
		this->swap(other);
		return *this;
	}

	UniquePtr<T[]>& operator=(std::nullptr_t)
	{
		reset();
		return *this;
	}

	UniquePtr(const UniquePtr<T[]>& other) = delete;
	UniquePtr<T[]>& operator=(const UniquePtr<T[]>& other) = delete;

	T* operator->()
	{
		return ptr;
	}

	T& operator*()
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

	operator T()
	{
		return ptr;
	}

	T* Get()
	{
		return ptr;
	}

	T* release()
	{
		T* res = nullptr;
		std::swap(res, ptr);
		return res;
	}

	void swap(UniquePtr<T[]>& other)
	{
		std::swap(ptr, other.ptr);
	}

	void reset()
	{
		delete[] ptr;
		ptr = nullptr;
	}
};

template <typename T>
void swap(UniquePtr<T>& left, UniquePtr<T>& right)
{
	left.swap(right);
}
