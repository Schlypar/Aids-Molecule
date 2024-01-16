
#pragma once

#include <cstddef>
#include <type_traits>

template <typename T>
class SharedPtr
{
private:
	T* ptr = nullptr;
	int* counterBlock = nullptr;

public:
	SharedPtr()
	    : ptr(nullptr)
	    , counterBlock(new int(1))
	{
	}

	SharedPtr(std::nullptr_t)
	    : ptr(nullptr)
	    , counterBlock(new int(1))
	{
	}

	SharedPtr(T* data)
	    : ptr(data)
	    , counterBlock(new int(1))
	{
	}

	SharedPtr(const SharedPtr<T>& other)
	    : ptr(other.ptr)
	    , counterBlock(other.counterBlock)
	{
		(*counterBlock)++;
	}

	~SharedPtr()
	{
		cleanup();
	}

	SharedPtr<T>& operator=(const SharedPtr<T>& other)
	{
		if (this == &other)
		{
			return *this;
		}

		cleanup();

		this->ptr = other.ptr;
		this->counterBlock = other.counterBlock;
		(*counterBlock)++;

		return *this;
	}

	SharedPtr<T>& operator=(std::nullptr_t)
	{
		reset();

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

	T& operator[](int index) const
	{
		return ptr[index];
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

	T* Get() const noexcept
	{
		return this->ptr;
	}

	T* release()
	{
		T* res = nullptr;
		std::swap(this->ptr, res);
		return res;
	}

	void reset()
	{
		T* temp = release();
		delete temp;
		delete counterBlock;
		counterBlock = new int(1);
	}

	void swap(SharedPtr<T>& other) noexcept
	{
		std::swap(ptr, other.ptr);
	}

	int UseCount() const
	{
		return *counterBlock;
	}

	bool Unique() const
	{
		return UseCount() == 1;
	}

private:
	void cleanup() noexcept
	{
		if (!counterBlock)
		{
			delete ptr;
			return;
		}
		(*counterBlock)--;
		if (*counterBlock <= 0)
		{
			delete ptr;
			delete counterBlock;
		}
	}
};

template <typename T>
class SharedPtr<T[]>
{
private:
	T* ptr = nullptr;
	int* counterBlock = nullptr;

public:
	SharedPtr()
	    : ptr(nullptr)
	    , counterBlock(new int(1))
	{
	}

	SharedPtr(std::nullptr_t)
	    : ptr(nullptr)
	    , counterBlock(new int(1))
	{
	}

	SharedPtr(T* data)
	    : ptr(data)
	    , counterBlock(new int(1))
	{
	}

	SharedPtr(const SharedPtr<T[]>& other)
	    : ptr(other.ptr)
	    , counterBlock(other.counterBlock)
	{
		(*counterBlock)++;
	}

	~SharedPtr()
	{
		cleanup();
	}

	SharedPtr<T[]>& operator=(const SharedPtr<T[]>& other)
	{
		if (this == &other)
		{
			return *this;
		}

		cleanup();

		this->ptr = other.ptr;
		this->counterBlock = other.counterBlock;
		(*counterBlock)++;

		return *this;
	}

	SharedPtr<T[]>& operator=(std::nullptr_t)
	{
		reset();

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

	T& operator[](int index) const
	{
		return ptr[index];
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

	T* Get() const noexcept
	{
		return this->ptr;
	}

	T* release()
	{
		T* res = nullptr;
		std::swap(this->ptr, res);
		return res;
	}

	void reset()
	{
		T* temp = release();
		delete temp;
		delete counterBlock;
		counterBlock = new int(1);
	}

	void swap(SharedPtr<T[]>& other) noexcept
	{
		std::swap(ptr, other.ptr);
	}

	int UseCount() const
	{
		return *counterBlock;
	}

	bool Unique() const
	{
		return UseCount() == 1;
	}

private:
	void cleanup() noexcept
	{
		if (!counterBlock)
		{
			delete[] ptr;
			return;
		}
		(*counterBlock)--;
		if (*counterBlock <= 0)
		{
			delete[] ptr;
			delete counterBlock;
		}
	}
};

template <class T>
void swap(SharedPtr<T>& left, SharedPtr<T>& right)
{
	left.swap(right);
}
