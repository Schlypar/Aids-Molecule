#include "Logger.h"
#include "Sequence/ArraySequence.h"
#include "Sequence/Sequence.h"
#include "Tree/BinaryHeap.h"
#include "Tree/BinarySearchTree.h"
#include "Tree/BinaryTree.h"
#include "Tree/ITree.h"
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

int main()
{
	// Logger::setPriority(ErrorPriority);

	auto startKGen = [](int value) -> int { return value; };

	// BinTreePtr<int, int> tree = (BinaryTree<int, int>*) new BinaryTree<int, int>(1, startKGen);
	// tree->Add(-1)->Add(-2);
	// print(tree, '\n');

	// BinSearchPtr<int> search = (BinarySearchTree<int>*) new BinarySearchTree<int>(tree.Get());
	// print(search, '\n');

	BinTreePtr<int, int> heap = (BinaryTree<int, int>*) new BinaryHeap<int, int>(1, startKGen);
	heap->Add(2)->Add(-1)->Add(5)->Add(-2)->Add(-3)->Add(-7)->Add(0)->Add(6)->Add(-2);
	print(heap, '\n');

	return 0;
}
