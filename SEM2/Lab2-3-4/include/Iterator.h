#pragma once

#include <iterator>

template <typename Iterator, typename TransformFunction>
class OutputTransformIterator
{
private:
	Iterator iterator_;
	TransformFunction transformFunction_;

public:
	using IteratorCategory = std::output_iterator_tag;

	explicit OutputTransformIterator(Iterator iterator, TransformFunction transformFunction)
	    : iterator_(iterator)
	    , transformFunction_(transformFunction)
	{
	}

	OutputTransformIterator& operator++()
	{
		++iterator_;
		return *this;
	}

	OutputTransformIterator& operator++(int)
	{
		++*this;
		return *this;
	}

	OutputTransformIterator& operator*()
	{
		return *this;
	}

	template <typename T>
	OutputTransformIterator& operator=(T const& value)
	{
		*iterator_ = transformFunction_(value);
		return *this;
	}
};

template <typename TransformFunction>
class OutputTransformer
{
private:
	TransformFunction transformFunction_;

public:
	explicit OutputTransformer(TransformFunction transformFunction)
	    : transformFunction_(transformFunction)
	{
	}

	template <typename Iterator>
	OutputTransformIterator<Iterator, TransformFunction> operator()(Iterator iterator) const
	{
		return OutputTransformIterator<Iterator, TransformFunction>(iterator, transformFunction_);
	}
};

template <typename T>
class AbstractIterator
{
public:
	virtual ~AbstractIterator()
	{
	}

	virtual AbstractIterator<T>& operator++() = 0;

	virtual AbstractIterator<T>& operator--() = 0;

	virtual T& operator*() const = 0;

	virtual bool equal(const AbstractIterator<T>& other) const = 0;

	virtual AbstractIterator<T>* copy() const = 0;

	bool operator==(const AbstractIterator<T>& other) const
	{
		return equal(*this, other);
	}
};
