#pragma once

#include "Dict/HashTable.h"
#include "Dict/PtrHasher.h"
#include <cstddef>
#include <functional>
#include <iostream>
#include <ostream>
#include <utility>

enum DeleterType
{
	ArrayDeleter,
	ValueDeleter,
};

struct PointerData
{
	int count;
	DeleterType deleteType;

	PointerData()
	    : count(0)
	    , deleteType(ValueDeleter)
	{
	}

	PointerData(DeleterType deleteType)
	    : count(1)
	    , deleteType(deleteType)
	{
	}

	PointerData(int count, DeleterType deleteType)
	    : count(count)
	    , deleteType(deleteType)
	{
	}

	friend std::ostream& operator<<(std::ostream& stream, const PointerData& ptrData)
	{
		switch (ptrData.deleteType)
		{
			case ArrayDeleter:
				stream << "(" << ptrData.count << ", ArrayDeleter)";
				break;
			case ValueDeleter:
				stream << "(" << ptrData.count << ", ValueDeleter)";
				break;
		}

		return stream;
	}
};

static HashTable<void*, PointerData> MemoryTracker = HashTable<void*, PointerData>(new PtrHasher<void*>());

template <typename T>
static bool DeletePointer(T* ptr)
{

	if (ptr != nullptr)
	{
		PointerData& data = MemoryTracker.GetMut((void*) ptr);

		if (data.count > 0)
		{
			data.count--;

			if (data.count <= 0)
			{
				switch (data.deleteType)
				{
					case ValueDeleter:
						delete ptr;
						return true;
					case ArrayDeleter:
						delete[] ptr;
						return true;
				}
			}
		}
	}

	return false;
}

template <typename T>
static void IncrementPointerCount(T* ptr, DeleterType deleterType)
{
	try
	{
		PointerData& data = MemoryTracker.GetMut((void*) ptr);
		data.count++;
	}
	catch (...)
	{
		MemoryTracker.Add({ (void*) ptr, PointerData(1, deleterType) });
	}
}

template <typename T>
static void Untrack(T* ptr)
{
	try
	{
		MemoryTracker.Remove((void*) ptr);
	}
	catch (...)
	{
	}
}

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
		try
		{
			PointerData& data = MemoryTracker.GetMut(this->ptr);
			data.count++;
		}
		catch (...)
		{
			MemoryTracker.Add({ (void*) this->ptr, PointerData(1, ValueDeleter) });
		}
	}

	UniquePtr(T* object, DeleterType deleterType) noexcept
	    : ptr(object)
	{
		try
		{
			PointerData& data = MemoryTracker.GetMut(this->ptr);
			data.count++;
		}
		catch (...)
		{
			MemoryTracker.Add({ (void*) this->ptr, PointerData(1, ValueDeleter) });
		}
	}

	UniquePtr(UniquePtr<T>&& other) noexcept
	    : ptr(nullptr)
	{
		this->swap(other);
	}

	~UniquePtr()
	{
		DeletePointer(this->ptr);
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
		PointerData& data = MemoryTracker.GetMut((void*) temp);

		if (data.count > 0)
		{
			data.count--;
			switch (data.deleteType)
			{
				case ValueDeleter:
					delete temp;
					break;
				case ArrayDeleter:
					delete[] temp;
					break;
			}
		}
	}

	friend std::ostream& operator<<(std::ostream& stream, const UniquePtr<T>& ptr)
	{
		stream << ptr.ptr;

		return stream;
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

public:
	SharedPtr() noexcept
	    : ptr(nullptr)
	{
	}

	SharedPtr(std::nullptr_t) noexcept
	    : ptr(nullptr)
	{
	}

	SharedPtr(T* object) noexcept
	    : ptr(object)
	{
		IncrementPointerCount(object, ValueDeleter);
	}

	SharedPtr(T* object, DeleterType deleterType) noexcept
	    : ptr(object)
	{
		IncrementPointerCount(object, deleterType);
	}

	SharedPtr(const SharedPtr<T>& other) noexcept
	    : ptr(other.ptr)
	{

		IncrementPointerCount(other.ptr, ValueDeleter);
	}

	~SharedPtr() noexcept
	{
		cleanup();
	}

	SharedPtr<T>& operator=(const SharedPtr<T>& other) noexcept
	{
		cleanup();

		this->ptr = other.ptr;
		IncrementPointerCount(this->ptr, ValueDeleter);
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
		Untrack(result);
		return result;
	}

	void swap(SharedPtr<T>& other) noexcept
	{
		std::swap(ptr, other.ptr);
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
		DeletePointer(temp);
	}

private:
	void cleanup() noexcept
	{
		DeletePointer(this->ptr);
	}
};

template <class T>
void swap(SharedPtr<T>& left, SharedPtr<T>& right)
{
	left.swap(right);
}

template <typename T>
class WeakPtr
{
private:
	T* ptr;

public:
	WeakPtr()
	    : ptr(nullptr)
	{
	}

	WeakPtr(const SharedPtr<T>& ptr)
	    : ptr(ptr.ptr)
	{
	}

	~WeakPtr() = default;

	WeakPtr& operator=(const SharedPtr<T>& other)
	{
		this->ptr = other.ptr;

		return *this;
	}

	WeakPtr& operator=(const WeakPtr<T>& other)
	{
		this->ptr = other.ptr;

		return *this;
	}

	bool Expired() const
	{
		// return UseCount() == 0;
		PointerData& data = MemoryTracker.GetMut((void*) this->ptr);
		return data.count == 0;
	}

	int UseCount() const
	{
		// return *counter;
		PointerData& data = MemoryTracker.GetMut((void*) this->ptr);
		return data.count;
	}

	SharedPtr<T> Lock() const
	{
		if (Expired())
		{
			return SharedPtr<T>(nullptr);
		}

		SharedPtr<T> ptr = nullptr;
		ptr.ptr = this->ptr;

		return ptr;
	}

	void swap(WeakPtr<T>& other)
	{
		std::swap(this->ptr, other.ptr);
	}
};

template <typename T>
void swap(WeakPtr<T>& left, WeakPtr<T>& right)
{
	left.swap(right);
}

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
