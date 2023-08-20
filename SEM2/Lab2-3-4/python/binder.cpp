#include "Pointer.h"
#include <ADT.h>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <utility>

LogPriority Logger::priority = InfoPriority;

namespace py = pybind11;

template <class T>
class ptr_wrapper
{

private:
	T* ptr;

public:
	ptr_wrapper()
	    : ptr(nullptr)
	{
	}

	ptr_wrapper(T* ptr)
	    : ptr(ptr)
	{
	}

	ptr_wrapper(const ptr_wrapper& other)
	    : ptr(other.ptr)
	{
	}

	T& operator*() const
	{
		return *ptr;
	}

	T* operator->() const
	{
		return ptr;
	}

	T* get() const
	{
		return ptr;
	}

	void destroy()
	{
		delete ptr;
	}

	T& operator[](std::size_t idx) const
	{
		return ptr[idx];
	}
};

int set_size()
{
	int size;

	std::cout << "Input size: ";
	std::cin >> size;
	std::cout << "\n";

	return size;
}

ptr_wrapper<int> generate_raw_array(int size)
{
	srand((unsigned) time(0));
	ptr_wrapper<int> array = new int[size];

	for (auto i = 0; i < size; i++)
		array[i] = rand() % 100 - rand() % 200;

	return array;
}

SharedPtr<int> generate_mysmart_array(int size)
{
	srand((unsigned) time(0));
	SharedPtr<int> array = new int[size];
	array.setDeleter(1);

	for (auto i = 0; i < size; i++)
		array[i] = rand() % 100 - rand() % 200;

	return array;
}

std::shared_ptr<int[]> generate_stl_array(int size)
{
	srand((unsigned) time(0));
	std::shared_ptr<int[]> array(new int[size]);

	for (auto i = 0; i < size; i++)
		array[i] = rand() % 100 - rand() % 200;

	return array;
}

void heapify_raw(ptr_wrapper<int> array, int size, int i)
{
	int largest = i;

	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < size && array[left] > array[largest])
		largest = left;

	if (right < size && array[right] > array[largest])
		largest = right;

	if (largest != i)
	{
		std::swap(array[i], array[largest]);

		heapify_raw(array, size, largest);
	}
}

void heapify_mysmart(SharedPtr<int> array, int size, int i)
{
	int largest = i;

	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < size && array[left] > array[largest])
		largest = left;

	if (right < size && array[right] > array[largest])
		largest = right;

	if (largest != i)
	{
		std::swap(array[i], array[largest]);

		heapify_mysmart(array, size, largest);
	}
}

void heapify_stl(std::shared_ptr<int[]> array, int size, int i)
{
	int largest = i;

	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < size && array[left] > array[largest])
		largest = left;

	if (right < size && array[right] > array[largest])
		largest = right;

	if (largest != i)
	{
		std::swap(array[i], array[largest]);

		heapify_stl(array, size, largest);
	}
}

void heapSortRaw(ptr_wrapper<int> array, int size)
{
	for (int i = size / 2 - 1; i >= 0; i--)
		heapify_raw(array, size, i);

	for (int i = size - 1; i > 0; i--)
	{
		std::swap(array[0], array[i]);
		heapify_raw(array, i, 0);
	}

	array.destroy();
}

void heapSortMysmart(SharedPtr<int> array, int size)
{
	for (int i = size / 2 - 1; i >= 0; i--)
		heapify_mysmart(array, size, i);

	for (int i = size - 1; i > 0; i--)
	{
		std::swap(array[0], array[i]);
		heapify_mysmart(array, i, 0);
	}
}

void heapSortStl(std::shared_ptr<int[]> array, int size)
{
	for (int i = size / 2 - 1; i >= 0; i--)
		heapify_stl(array, size, i);

	for (int i = size - 1; i > 0; i--)
	{
		std::swap(array[0], array[i]);
		heapify_stl(array, i, 0);
	}
}

void print_raw(ptr_wrapper<int> array, int size)
{
	for (auto i = 0; i < size; i++)
		std::cout << array[i] << " ";

	std::cout << '\n';
}

void print_myimpl(SharedPtr<int> array, int size)
{
	for (auto i = 0; i < size; i++)
		std::cout << array[i] << " ";

	std::cout << '\n';
}

PYBIND11_MODULE(smart_pointers, m)
{
	py::class_<ptr_wrapper<int>>(m, "pint");
	py::class_<SharedPtr<int>>(m, "mypint");

	m.doc() = "Test";

	m.def("generate_raw", &generate_raw_array, R"pbdoc(
	       Generates raw pointer to an array
	   )pbdoc");

	m.def("generate_myimpl", &generate_mysmart_array, R"pbdoc(
	       Generates my implementation of smart pointer to an array
	   )pbdoc");

	m.def("sort_raw", &heapSortRaw, R"pbdoc(
	       Sort an array that is represented by a raw pointer

	       sort_raw(array, size)
	   )pbdoc");

	m.def("sort_myimpl", &heapSortMysmart, R"pbdoc(
	       Sort an array that is represented by my impl of pointer to an array

	       sort_raw(array, size)
	   )pbdoc");

	m.def("print_raw", &print_raw, R"pbdoc(
            Prints array with given size

            print_raw(array, size)
	   )pbdoc");

	m.def("print_myimpl", &print_myimpl, R"pbdoc(
            Prints array with given size

            print_myimpl(array, size)
	   )pbdoc");
}
