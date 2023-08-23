#pragma once

#include "BinarySearchTree.h"
#include "Tree/BinaryTree.h"
#include "concepts.h"

template <Comparable T>
class OrderedSet : private BinarySearchTree<T>
{
	template <typename T1, typename T2>
	using TreeNode = typename Tree<T1, T2>::TreeNode;

	template <typename T1, typename T2>
	using NodePtr = TreeNode<T1, T2>*;

private:
	OrderedSet(const T& startValue, KGen<T, T> kGen)
	    : BinarySearchTree<T>(startValue, kGen)
	{
	}

	OrderedSet(TreeNode<T, T>* startRoot)
	    : BinarySearchTree<T>(startRoot)
	{
	}

	OrderedSet(KGen<T, T> kGen)
	    : BinarySearchTree<T>(kGen)
	{
	}

public:
	OrderedSet()
	    : BinarySearchTree<T>()
	{
	}

	OrderedSet(const T& startValue)
	    : BinarySearchTree<T>(startValue)
	{
	}

	OrderedSet(const OrderedSet<T>& other)
	    : BinarySearchTree<T>((BinarySearchTree<T>*) (&other))
	{
	}

	OrderedSet(const OrderedSet<T>* other)
	    : BinarySearchTree<T>((BinarySearchTree<T>*) (other))
	{
	}

	// Adds value to the set
	OrderedSet<T>* Include(const T& value) noexcept
	{
		return (OrderedSet<T>*) (((BinarySearchTree<T>*) this)->Add(value));
	}

	// Deletes value from a set
	void Erase(const T& value) noexcept
	{
		((BinarySearchTree<T>*) this)->Delete(value);
	}

	// Mathematical union of two sets
	OrderedSet<T>* Union(const OrderedSet<T>* other) const noexcept;

	// Mathematical intersection of two sets
	OrderedSet<T>* Intersection(const OrderedSet<T>* other) const noexcept;

	// Mathematical difference between two sets
	OrderedSet<T>* Difference(const OrderedSet<T>* other) const noexcept;

	bool IncludesSubset(const OrderedSet<T>* other);

	bool isEquals(const OrderedSet<T>* other);

	bool Includes(const T& value) const noexcept
	{
		return this->isThere(value);
	}

	// Returns number of elements in a set
	friend Size Cardinalis(const OrderedSet<T>* set)
	{
		Size power = 0;

		set->Traverse(Left, Right, Root, [&power](const T& val) -> void { power++; });

		return power;
	}

	friend Size Cardinalis(SharedPtr<OrderedSet<T>> set)
	{
		Size power = 0;

		set->Traverse(Left, Right, Root, [&power](const T& val) -> void { power++; });

		return power;
	}

	friend std::ostream& operator<<(std::ostream& stream, const OrderedSet<T>& set)
	{
		stream << "{ ";
		set.Traverse(Left, Root, Right, [&stream](const T& val) -> void { stream << val << " "; });
		stream << "}";

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const OrderedSet<T>* set)
	{
		stream << "{ ";
		set->Traverse(Left, Root, Right, [&stream](const T& val) -> void { stream << val << " "; });
		stream << "}";

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const SharedPtr<OrderedSet<T>> set)
	{
		stream << "{ ";
		set->Traverse(Left, Root, Right, [&stream](const T& val) -> void { stream << val << " "; });
		stream << "}";

		return stream;
	}

	Tree<T, T>* Create() const noexcept override
	{
		return (Tree<T, T>*) new OrderedSet<T>(this->kGen);
	}

	Tree<T, T>* Copy() const noexcept override
	{
		return (Tree<T, T>*) new OrderedSet<T>(this);
	}
};

template <Comparable T>
OrderedSet<T>* OrderedSet<T>::Union(const OrderedSet<T>* other) const noexcept
{
	OrderedSet<T>* result = (OrderedSet<T>*) this->Copy();

	result->Concat((Tree<T, T>*) other);

	return result;
}

template <Comparable T>
OrderedSet<T>* OrderedSet<T>::Intersection(const OrderedSet<T>* other) const noexcept
{
	OrderedSet<T>* result = (OrderedSet<T>*) this->Create();

	auto add = [this, other, result](const T& value) -> void {
		if (this->Includes(value) && other->Includes(value))
			result->Include(value);
	};

	auto copy = this->Traverse(Left, Right, Root, add);
	delete copy;

	return result;
}

template <Comparable T>
OrderedSet<T>* OrderedSet<T>::Difference(const OrderedSet<T>* other) const noexcept
{
	OrderedSet<T>* result = (OrderedSet<T>*) this->Create();

	auto isUnique = [this, other](const T& value) -> bool {
		if (this->Includes(value) && other->Includes(value))
			return false;

		return true;
	};

	auto addIfUnique = [isUnique, result, this](const T& value) -> void {
		if (isUnique(value) && this->Includes(value))
			result->Add(value);
	};

	auto copy = this->Traverse(Left, Right, Root, addIfUnique);
	delete copy;

	return result;
}

template <Comparable T>
bool OrderedSet<T>::IncludesSubset(const OrderedSet<T>* other)
{
	bool result = true;
	bool* includes = &result;

	auto alterIncludes = [&includes, this](const T& value) -> void {
		if (this->Includes(value) == false)
			*includes = false;
	};

	auto copy = other->Traverse(Left, Right, Root, alterIncludes);
	delete copy;

	return result;
}

template <Comparable T>
bool OrderedSet<T>::isEquals(const OrderedSet<T>* other)
{
	if (this->IncludesSubset(other) && other->IncludesSubset(this))
		return true;
	else
		return false;
}
