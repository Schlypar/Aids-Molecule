#include "Logger.h"
#include "Sequence/Sequence.h"
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

int main()
{
	// Logger::setPriority(ErrorPriority);

	auto startKGen = [](int value) -> int { return value; };

	BinTreePtr<int, int> tree = (BinaryTree<int, int>*) new BinaryTree<int, int>(1, startKGen);
	// tree->Add(-1)->Add(-2);
	print(tree, '\n');
	tree->Delete(1);
	print(tree, '\n');

	return 0;
}