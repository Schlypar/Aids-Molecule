#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>

template <typename T>
class Deleter
{
private:
	std::function<void(T*)> deleter;

public:
	Deleter()
	    : deleter([](T* block) -> void
		      { delete block; })
	{
	}

	Deleter(int kostyl)
	    : deleter([](T* block) -> void
		      { delete[] block; })
	{
	}

	Deleter(const Deleter<T>& other)
	    : deleter(other.deleter)
	{
	}

	void delete_block(T* block)
	{
		this->deleter(block);
	}
};

template <class T>
class UniquePtr

{
private:
	T* ptr;
	Deleter<T> deleter = Deleter<T>();

public:
	UniquePtr() noexcept
	    : ptr(nullptr)
	{
	}

	UniquePtr(int kostyl) noexcept
	    : ptr(nullptr)
	    , deleter(kostyl)
	{
	}

	UniquePtr(std::nullptr_t) noexcept
	    : ptr(nullptr)
	{
	}

	UniquePtr(std::nullptr_t, int kostyl) noexcept
	    : ptr(nullptr)
	    , deleter(kostyl)
	{
	}

	UniquePtr(T* object) noexcept
	    : ptr(object)
	{
	}

	UniquePtr(T* object, int kostyl) noexcept
	    : ptr(object)
	    , deleter(kostyl)
	{
	}

	UniquePtr(UniquePtr<T>&& other) noexcept
	    : ptr(nullptr)
	    , deleter(other.deleter)
	{
		this->swap(other);
	}

	~UniquePtr()
	{
		if (this->ptr != nullptr)
		{
			deleter.delete_block(ptr);
		}
	}

	void setDeleter(const Deleter<T>& deleter)
	{
		this->deleter = deleter;
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

	operator T()
	{
		return ptr;
	}

	T* Get() const noexcept
	{
		return this->ptr;
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
		// delete temp;
		deleter.delete_block(temp);
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

	template <typename U>
	friend class WeakPtr;

private:
	T* ptr;
	int* counter;
	Deleter<T> deleter = Deleter<T>();

	int* GetCounter() const
	{
		return counter;
	}

public:
	SharedPtr() noexcept
	    : ptr(nullptr)
	    , counter(new int(0))
	{
	}

	SharedPtr(std::nullptr_t) noexcept
	    : ptr(nullptr)
	    , counter(new int(0))
	{
	}

	SharedPtr(T* object) noexcept
	    : ptr(object)
	    , counter(new int(1))
	{
	}

	SharedPtr(const SharedPtr<T>& other) noexcept
	    : ptr(other.ptr)
	    , counter(other.counter)
	    , deleter(other.deleter)
	{
		if (other.counter)
		{
			(*counter)++;
		}
	}

	~SharedPtr() noexcept
	{
		cleanup();
	}

	void setDeleter(const Deleter<T>& deleter)
	{
		this->deleter = deleter;
	}

	SharedPtr<T>& operator=(const SharedPtr<T>& other) noexcept
	{
		cleanup();

		this->ptr = other.ptr;
		this->counter = other.counter;
		if (counter)
		{
			(*counter)++;
		}

		return *this;
	}

	SharedPtr<T>& operator=(std::nullptr_t) noexcept
	{
		reset();

		return *this;
	}

	SharedPtr<T>& operator=(SharedPtr<T>&& other)
	{
		cleanup();

		ptr = other.ptr;
		counter = other.counter;

		other.ptr = nullptr;
		other.counter = nullptr;

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

	T* Get() const noexcept
	{
		return this->ptr;
	}

	T* release() noexcept
	{
		T* result = nullptr;
		std::swap(result, ptr);
		*counter = 0;
		return result;
	}

	void swap(SharedPtr<T>& other) noexcept
	{
		std::swap(ptr, other.ptr);
		std::swap(counter, other.counter);
	}

	int UseCount() const
	{
		return *(this->counter);
	}

	bool Unique() const
	{
		return UseCount() == 1;
	}

	void reset() noexcept
	{
		T* temp = release();
		*counter = 0;
		// delete temp;
		deleter.delete_block(temp);
	}

private:
	void cleanup() noexcept
	{
		if (counter)
		{
			(*counter)--;

			if (*counter <= 0)
			{
				if (ptr != nullptr)
				{
					deleter.delete_block(ptr);
				}

				delete counter;
			}
		}
	}
};

template <class T>
void swap(SharedPtr<T>& left, SharedPtr<T>& right)
{
	left.swap(right);
}

template <class T>
class SloppyPtr
{
private:
	T* ptr;
	Deleter<T> deleter = Deleter<T>();

public:
	SloppyPtr() noexcept
	    : ptr()
	{
	}

	SloppyPtr(int kostyl) noexcept
	    : ptr()
	    , deleter(kostyl)
	{
	}

	SloppyPtr(std::nullptr_t) noexcept
	    : ptr(nullptr)
	{
	}

	SloppyPtr(std::nullptr_t, int kostyl) noexcept
	    : ptr(nullptr)
	    , deleter(kostyl)
	{
	}

	SloppyPtr(T* object) noexcept
	    : ptr(object)
	{
	}

	SloppyPtr(T* object, int kostyl) noexcept
	    : ptr(object)
	    , deleter(kostyl)
	{
	}

	SloppyPtr(const SloppyPtr<T>& other) noexcept
	    : ptr(other.ptr)
	    , deleter(other.deleter)
	{
	}

	~SloppyPtr() noexcept
	{
		deleter.delete_block(ptr);
	}

	void setDeleter(const Deleter<T>& deleter)
	{
		this->deleter = deleter;
	}

	SloppyPtr<T>& operator=(const SloppyPtr<T>& other) noexcept
	{
		this->ptr = other.ptr;

		return *this;
	}

	SloppyPtr<T>& operator=(std::nullptr_t) noexcept
	{
		// reset();
		ptr = nullptr;

		return *this;
	}

	SloppyPtr(SloppyPtr<T>&& other)
	{
		// if (this->ptr)
		// 	delete ptr;

		ptr = other.ptr;
		other.ptr = nullptr;
	}

	SloppyPtr<T>& operator=(SloppyPtr<T>&& other)
	{
		// if (this->ptr)
		// 	delete ptr;

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

	operator T()
	{
		return ptr;
	}

	T* Get() const noexcept
	{
		return this->ptr;
	}

	T* release() noexcept
	{
		T* result = nullptr;
		std::swap(result, ptr);
		return result;
	}

	void swap(SloppyPtr<T>& other) noexcept
	{
		std::swap(ptr, other.ptr);
	}

	void reset() noexcept
	{
		T* temp = release();
		delete temp;
	}

	bool expired() const
	{
		return this->ptr == nullptr;
	}
};

template <class T>
void swap(SloppyPtr<T>& left, SloppyPtr<T>& right)
{
	left.swap(right);
}

template <typename T>
class WeakPtr
{
private:
	T* pointer;
	int* counter;

public:
	WeakPtr()
	    : pointer(nullptr)
	    , counter(nullptr)
	{
	}

	WeakPtr(const SharedPtr<T>& ptr)
	    : pointer(ptr.ptr)
	    , counter(ptr.counter)
	{
		(*counter)++;
	}

	~WeakPtr() = default;

	WeakPtr& operator=(const SharedPtr<T>& other)
	{
		this->pointer = other.ptr;
		this->counter = other.counter;

		return *this;
	}

	WeakPtr& operator=(const WeakPtr<T>& other)
	{
		this->pointer = other.pointer;
		this->counter = other.counter;

		return *this;
	}

	bool Expired() const
	{
		return UseCount() == 0;
	}

	int UseCount() const
	{
		return *counter;
	}

	SharedPtr<T> Lock() const
	{
		if (Expired())
		{
			return SharedPtr<T>(nullptr);
		}

		SharedPtr<T> ptr = nullptr;
		ptr.ptr = this->pointer;
		ptr.counter = this->counter;

		(*counter)++;

		return ptr;
	}

	void swap(const WeakPtr<T>& other)
	{
		std::swap(this->pointer, other.pointer);
		std::swap(this->counter, other.counter);
	}
};

template <typename T>
void swap(WeakPtr<T>& left, WeakPtr<T>& right)
{
	left.swap(right);
}
