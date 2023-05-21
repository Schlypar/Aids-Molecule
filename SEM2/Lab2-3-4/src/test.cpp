
#include "Tree/BinaryHeap.h"
#include "Tree/BinaryTree.h"
#include "Tree/MultiBinaryTree.h"
#include "Tree/OrderedSet.h"
#include "Tuple.h"
#include "functional/Algorithms.h"
#include <ADT.h>

LogPriority Logger::priority = TracePriority;

template <typename T1, typename T2>
using TreePtr = SharedPtr<Tree<T1, T2>>;

template <typename T1, typename T2>
using ConstTreePtr = SharedPtr<const Tree<T1, T2>>;

template <typename T1, typename T2>
using BinTreePtr = SharedPtr<BinaryTree<T1, T2>>;

template <typename T1, typename T2>
using BinHeapPtr = SharedPtr<BinaryHeap<T1, T2>>;

template <typename T>
using BinSearchPtr = SharedPtr<BinarySearchTree<T>>;

template <typename T1, typename T2>
using MultiBinTreePtr = SharedPtr<MultiBinaryTree<T1, T2>>;

template <typename T1>
using SetPtr = SharedPtr<OrderedSet<T1>>;

int main()
{
	Logger::setPriority(ErrorPriority);

	// TreePtr<int, int> Tree = (BinaryTree<int, int>*) new BinarySearchTree<int>(1);

	// Tree->Add(5);
	// print(Tree, '\n');

	// BinTreePtr<int, int> Tree1 = (BinaryTree<int, int>*) new BinarySearchTree<int>(2);
	// Tree1->Add(5);

	// Tree->Concat(Tree1.Get());
	// print(Tree, '\n');

	SetPtr<int> set = new OrderedSet<int>(1);
	set->Append(5)->Append(5);
	print(set, '\n');

	SetPtr<int> set1 = new OrderedSet<int>(2);
	set1->Append(5);

	SetPtr<int> un = set->Union(set1.Get());
	print(un, '\n');

	SetPtr<int> intersection = set->Intersection(set1.Get());
	print(intersection, '\n');

	SetPtr<int> diff = set->Difference(set1.Get());
	print(diff, '\n');

	return 0;
}
