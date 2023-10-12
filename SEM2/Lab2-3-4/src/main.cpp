#include "Pointer.h"
#include "Sorter/HeapSort.h"
#include <ADT.h>
#include <cwchar>
#include <functional>
#include <memory>

LogPriority Logger::priority = FatalPriority;

template <typename T>
inline constexpr bool is_array_type = false;

template <typename T>
inline constexpr bool is_array_type<T[]> = true;

template <typename T>
	requires(!is_array_type<T>)
using NonArray = T;

template <typename T>
	requires(is_array_type<T>)
using IsArray = T;

template <typename T>
struct _Deleter
{
	std::function<void(T*)> deleter = nullptr;

	_Deleter()
	{
		if (is_array_type<T>)
		{
			this->deleter = [](T* ptr)
			{ delete[] ptr; };
		}
		else
		{
			this->deleter = [](T* ptr)
			{ delete ptr; };
		}
	}

	void operator()(T* ptr)
	{
		deleter(ptr);
	}
};

template <typename T>
class Inner
{
	_Deleter<T> deleter_fn;
	T data;

	Inner()
	{
		if (is_array_type<T>)
		{
			this->deleter_fn = [](T ptr)
			{ delete[] ptr; };
		}
		else
		{
			this->deleter_fn = [](T ptr)
			{ delete ptr; };
		}
	}
};

template <typename T>
class Someclass
{
protected:
	_Deleter<T> deleter_fn;
	T data;

public:
	template <typename K, typename... Args>
	friend Someclass<IsArray<K>> Make(Args... args);

	template <typename K>
	friend Someclass<K> Make(K data);

	Someclass(T pointer)
	{
		deleter_fn = _Deleter<T>();
		data = pointer;
	}
};

template <typename T>
inline Someclass<NonArray<T>> Make(T data)
{
	return Someclass<T>(new T(data));
}

template <typename T, typename Y>
inline Someclass<IsArray<T>> Make(Y data)
{
	return Someclass<T>(data);
}

int main()
{
	// std::shared_ptr<int[]> ptr = std::shared_ptr<int[]>(new int[5]);
	// Someclass<int[]> test = Make<int[]>(new int[3]);
}
