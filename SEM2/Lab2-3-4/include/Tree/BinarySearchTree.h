#pragma once

#include "BinaryTree.h"
#include "Tree/ITree.h"
#include "concepts.h"

template <Comparible T>
class BinarySearchTree : public BinaryTree<T, T>
{

	template <typename T1, typename T2>
	using TreeNode = typename Tree<T1, T2>::TreeNode;

	template <typename T1, typename T2>
	using NodePtr = TreeNode<T1, T2>*;

public:
	BinarySearchTree()
	    : BinaryTree<T, T>()
	{
	}

	BinarySearchTree(const T& startValue)
	    : BinaryTree<T, T>(startValue)
	{
		this->root->key = this->kGen(startValue);
	}

	BinarySearchTree(KGen<T, T> kGen)
	    : BinaryTree<T, T>(kGen)
	{
	}

	BinarySearchTree(const T& startValue, KGen<T, T> kGen)
	    : BinaryTree<T, T>(startValue, kGen)
	{
	}

	BinarySearchTree(TreeNode<T, T>* startRoot)
	    : BinaryTree<T, T>(startRoot)
	{
	}

	BinarySearchTree(const BinaryTree<T, T>& other)
	    : BinaryTree<T, T>(other)
	{
	}

	BinarySearchTree(const BinarySearchTree<T>& other)
	    : BinaryTree<T, T>((BinaryTree<T, T>*) (&other))
	{
	}

	BinarySearchTree(const BinarySearchTree<T>* other)
	    : BinaryTree<T, T>((BinaryTree<T, T>*) other)
	{
	}

	Tree<T, T>* Create() const noexcept override
	{
		return (Tree<T, T>*) new BinarySearchTree<T>(this->kGen);
	}

	Tree<T, T>* Copy() const noexcept override
	{
		return (Tree<T, T>*) new BinarySearchTree<T>(*this);
	}

	friend std::ostream& operator<<(std::ostream& stream, SharedPtr<BinarySearchTree<T>> tree)
	{
		stream << (BinaryTree<T, T>*) tree.Get();

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const BinarySearchTree<T>& tree)
	{
		stream << ((BinaryTree<T, T>*) (&tree));

		return stream;
	}
};