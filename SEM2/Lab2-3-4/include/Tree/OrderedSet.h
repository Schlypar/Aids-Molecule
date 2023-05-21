#pragma once

#include "BinarySearchTree.h"
#include "Tree/BinaryTree.h"
#include "concepts.h"

template <Comparible T>
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

	OrderedSet<T>* Append(const T& value) noexcept
	{
		return (OrderedSet<T>*) (((BinarySearchTree<T>*) this)->Add(value));
	}

	void Erase(const T& value) noexcept
	{
		((BinarySearchTree<T>*) this)->Delete(value);
	}

	OrderedSet<T>* Union(const OrderedSet<T>* other) const noexcept
	{
		OrderedSet<T>* result = (OrderedSet<T>*) this->Copy();

		result->Concat((Tree<T, T>*) other);

		return result;
	}

	OrderedSet<T>* Intersection(const OrderedSet<T>* other) const noexcept
	{
		OrderedSet<T>* result = (OrderedSet<T>*) this->Create();

		// result->Merge((Tree<T, T>*) other);

		auto add = [this, other, result](const T& value) -> void {
			if (this->isIn(value) && other->isIn(value))
				result->Append(value);
		};

		auto copy = this->Traverse(Left, Right, Root, add);
		delete copy;

		return result;
	}

	OrderedSet<T>* Difference(const OrderedSet<T>* other) const noexcept
	{
		OrderedSet<T>* result = (OrderedSet<T>*) this->Create();

		auto isUnique = [this, other](const T& value) -> bool {
			if (this->isIn(value) && other->isIn(value))
				return false;

			return true;
		};

		auto addIfUnique = [isUnique, result, this](const T& value) -> void {
			if (isUnique(value) && this->isIn(value))
				result->Add(value);
		};

		auto copy = this->Traverse(Left, Right, Root, addIfUnique);
		delete copy;

		return result;
	}

	bool isIn(const T& value) const noexcept
	{
		return this->isThere(value);
	}

	friend std::ostream& operator<<(std::ostream& stream, const OrderedSet<T>& set)
	{
		set.Traverse(Left, Root, Right, [&stream](const T& val) -> void { stream << val << " "; });

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const OrderedSet<T>* set)
	{
		set->Traverse(Left, Root, Right, [&stream](const T& val) -> void { stream << val << " "; });

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const SharedPtr<OrderedSet<T>> set)
	{
		set->Traverse(Left, Root, Right, [&stream](const T& val) -> void { stream << val << " "; });

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