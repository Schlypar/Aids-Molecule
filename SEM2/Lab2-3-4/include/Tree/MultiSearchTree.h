#pragma once

#include "MultiBinaryTree.h"
#include "concepts.h"

template <Comparible T>
class MultiBinarySearch : public MultiBinaryTree<T, T>
{
	template <typename T1, typename T2>
	using TreeNode = typename Tree<T1, T2>::TreeNode;

	template <typename T1, typename T2>
	using NodePtr = TreeNode<T1, T2>*;

public:
	MultiBinarySearch()
	    : MultiBinaryTree<T, T>()
	{
	}

	MultiBinarySearch(const T& startValue)
	    : MultiBinaryTree<T, T>(startValue)
	{
	}

	MultiBinarySearch(const T& startValue, KGen<T, Multi<T>> kGenMulti)
	    : MultiBinaryTree<T, T>(startValue, kGenMulti)
	{
	}

	MultiBinarySearch(TreeNode<T, Multi<T>>* startRoot)
	    : MultiBinaryTree<T, T>(startRoot)
	{
	}

	MultiBinarySearch(const MultiBinaryTree<T, T>& other)
	    : MultiBinaryTree<T, T>(other)
	{
	}

	MultiBinarySearch(const MultiBinaryTree<T, T>* other)
	    : MultiBinaryTree<T, T>(other)
	{
	}
};