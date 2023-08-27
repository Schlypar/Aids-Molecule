#pragma once

#include "Pointer.h"
#include "Sequence/ListSequence.h"

template <typename T>
class Stack
{
private:
	ListSequence<T> stack;

public:
	Stack()
	    : stack()
	{
	}

	Stack(const T* data, Size count)
	    : stack(data, count)
	{
	}

	Stack(const Stack<T>& other)
	    : stack(other.stack)
	{
	}

	Stack(Stack<T>&& other)
	    : stack(std::move(other.stack))
	{
	}

	Stack(const Sequence<T>& other)
	    : stack(other)
	{
	}

	Stack(Sequence<T>&& other)
	    : stack(std::move(other))
	{
	}

	template <typename... Args>
	Stack(T head, Args... args)
	    : stack(ListSequence<T>(head, args...))
	{
	}

	~Stack()
	{
	}

	bool isEmpty() const noexcept
	{
		return stack.isEmpty();
	}

	T Pop()
	{
		T res = stack.GetLast();
		stack.Remove(stack.GetLength() - 1);
		return res;
	}

	Stack<T>* Push(const T& value) noexcept
	{
		stack.Append(value);
		return this;
	}

	Size Count() const noexcept
	{
		return stack.GetLength();
	}

	T Peek(const Index index) const noexcept
	{
		return stack.Get(index);
	}

	T& operator[](const Index index)
	{
		return stack.Get(index);
	}

	Stack<T>& operator=(const Stack<T>& other)
	{
		this->stack = other.stack;

		return *this;
	}

	Stack<T>& operator=(Stack<T>&& other)
	{
		this->stack = std::move(other.stack);

		return *this;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Stack<T>& stack)
	{
		// stream << stack.stack;

		stream << " ";
		for (auto& data : stack.stack)
			stream << data << " ";

		// auto iter = stack.stack.begin();

		// for (; iter != stack.stack.end(); iter++)
		// 	stream << *iter << " ";

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Stack<T>* stack)
	{
		// stream << stack->stack;

		stream << " ";
		for (auto& data : stack->stack)
			stream << data << " ";

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, SharedPtr<Stack<T>> stack)
	{
		// stream << stack->stack;

		stream << " ";
		for (auto& data : stack->stack)
			stream << data << " ";

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const SloppyPtr<Stack<T>>& stack)
	{
		// stream << stack->stack;

		stream << " ";
		for (auto& data : stack->stack)
			stream << data << " ";

		return stream;
	}
};
