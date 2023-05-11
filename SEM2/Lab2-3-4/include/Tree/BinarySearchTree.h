#pragma once

#include "BinaryTree.h"
#include "Tree/ITree.h"

template <typename T>
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
};