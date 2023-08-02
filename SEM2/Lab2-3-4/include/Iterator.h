#pragma once

#include <iterator>

template <typename Iterator, typename TransformFunction>
class output_transform_iterator
{
public:
	using iterator_category = std::output_iterator_tag;

	explicit output_transform_iterator(Iterator iterator, TransformFunction transformFunction)
	    : iterator_(iterator)
	    , transformFunction_(transformFunction)
	{
	}

	output_transform_iterator& operator++()
	{
		++iterator_;
		return *this;
	}

	output_transform_iterator& operator++(int)
	{
		++*this;
		return *this;
	}

	output_transform_iterator& operator*()
	{
		return *this;
	}

	template <typename T>
	output_transform_iterator& operator=(T const& value)
	{
		*iterator_ = transformFunction_(value);
		return *this;
	}

private:
	Iterator iterator_;
	TransformFunction transformFunction_;
};

template <typename TransformFunction>
class output_transformer
{
public:
	explicit output_transformer(TransformFunction transformFunction)
	    : transformFunction_(transformFunction)
	{
	}

	template <typename Iterator>
	output_transform_iterator<Iterator, TransformFunction> operator()(Iterator iterator) const
	{
		return output_transform_iterator<Iterator, TransformFunction>(iterator, transformFunction_);
	}

private:
	TransformFunction transformFunction_;
};

// template <typename TransformFunction>
// output_transformer<TransformFunction> make_output_transformer(TransformFunction transformFunction)
// {
// 	return output_transformer<TransformFunction>(transformFunction);
// }

template <typename T>
class AbstractIterator
{
public:
	virtual ~AbstractIterator()
	{
	}

	virtual AbstractIterator<T>& operator++() = 0;
	// virtual AbstractIterator<T> operator++(int) = 0;

	virtual AbstractIterator<T>& operator--() = 0;
	// virtual AbstractIterator<T> operator--(int) = 0;

	virtual T& operator*() const = 0;

	virtual bool equal(const AbstractIterator<T>& other) const = 0;

	bool operator==(const AbstractIterator<T>& other) const
	{
		return this->equal(other);
	}

	virtual AbstractIterator<T>* copy() const = 0;
};
