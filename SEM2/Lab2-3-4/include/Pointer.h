#pragma once

#include <cstddef>
#include <functional>
#include <utility>

template <typename T>
class Deleter
{
private:
	std::function<void(T*)> deleter;

public:
	Deleter()
	    : deleter([](T* block) -> void { delete block; })
	{
	}

	Deleter(int kostyl)
	    : deleter([](T* block) -> void { delete[] block; })
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
			deleter.delete_block(ptr);
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
private:
	T* ptr;
	int* counter;
	Deleter<T> deleter = Deleter<T>();

public:
	SharedPtr() noexcept
	    : ptr()
	    , counter(new int(0))
	{
	}

	SharedPtr(int kostyl) noexcept
	    : ptr()
	    , counter(new int(0))
	    , deleter(kostyl)
	{
	}

	SharedPtr(std::nullptr_t) noexcept
	    : ptr(nullptr)
	    , counter(0)
	{
	}

	SharedPtr(std::nullptr_t, int kostyl) noexcept
	    : ptr(nullptr)
	    , counter(0)
	    , deleter(kostyl)
	{
	}

	SharedPtr(T* object) noexcept
	    : ptr(object)
	    , counter(new int(1))
	{
	}

	SharedPtr(T* object, int kostyl) noexcept
	    : ptr(object)
	    , counter(new int(1))
	    , deleter(kostyl)
	{
	}

	SharedPtr(const SharedPtr<T>& other) noexcept
	    : ptr(other.ptr)
	    , counter(other.counter)
	    , deleter(other.deleter)
	{
		if (other.counter)
			(*counter)++;
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
			(*counter)++;

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
		other.counter = nullptr;
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
					deleter.delete_block(ptr);

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
class WeakPtr
{
private:
	T* ptr;
	Deleter<T> deleter = Deleter<T>();

public:
	WeakPtr() noexcept
	    : ptr()
	{
	}

	WeakPtr(int kostyl) noexcept
	    : ptr()
	    , deleter(kostyl)
	{
	}

	WeakPtr(std::nullptr_t) noexcept
	    : ptr(nullptr)
	{
	}

	WeakPtr(std::nullptr_t, int kostyl) noexcept
	    : ptr(nullptr)
	    , deleter(kostyl)
	{
	}

	WeakPtr(T* object) noexcept
	    : ptr(object)
	{
	}

	WeakPtr(T* object, int kostyl) noexcept
	    : ptr(object)
	    , deleter(kostyl)
	{
	}

	WeakPtr(const WeakPtr<T>& other) noexcept
	    : ptr(other.ptr)
	    , deleter(other.deleter)
	{
	}

	~WeakPtr() noexcept
	{
		deleter.delete_block(ptr);
	}

	void setDeleter(const Deleter<T>& deleter)
	{
		this->deleter = deleter;
	}

	WeakPtr<T>& operator=(const WeakPtr<T>& other) noexcept
	{
		this->ptr = other.ptr;

		return *this;
	}

	WeakPtr<T>& operator=(std::nullptr_t) noexcept
	{
		// reset();
		ptr = nullptr;

		return *this;
	}

	WeakPtr(WeakPtr<T>&& other)
	{
		// if (this->ptr)
		// 	delete ptr;

		ptr = other.ptr;
		other.ptr = nullptr;
	}

	WeakPtr<T>& operator=(WeakPtr<T>&& other)
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

	void swap(WeakPtr<T>& other) noexcept
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
void swap(WeakPtr<T>& left, WeakPtr<T>& right)
{
	left.swap(right);
}
