#include "Logger.h"
#include "Sequence/ArraySequence.h"
#include "Sequence/Sequence.h"
#include "Sequence/Stack.h"

#include "Tree/BinaryHeap.h"
#include "Tree/BinarySearchTree.h"
#include "Tree/BinaryTree.h"
#include "Tree/ITree.h"
#include "Tree/MultiSearchTree.h"

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

template <typename T>
using MultiBinSearchPtr = SharedPtr<MultiSearchTree<T>>;

int main()
{
	Logger::setPriority(ErrorPriority);

	auto startKGen = [](const int& value) -> int { return value; };

	// BinTreePtr<int, int> tree = (BinaryTree<int, int>*) new BinaryTree<int, int>(1, startKGen);
	// tree->Add(-1)->Add(-2)->Add(-10);
	// print(tree, '\n');

	MultiBinSearchPtr<int> search = new MultiSearchTree<int>(1, [](const Multi<int>& stack) -> int { return stack.Peek(0); });
	search->Add(-1)->Add(-2)->Add(-3)->Add(-4)->Add(-5)->Add(-6);
	print(search, '\n');

	// BinTreePtr<int, int> heap = (BinaryTree<int, int>*) new BinaryHeap<int, int>(1, startKGen);
	// heap->Add(2)->Add(-1)->Add(5)->Add(-2)->Add(-3)->Add(-7)->Add(0)->Add(6)->Add(-2);
	// print(heap, '\n');

	return 0;
}
