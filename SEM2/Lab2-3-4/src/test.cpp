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
	tree->Add(-1)->Add(-2);
	tree->Add(-3);
	tree->Add(-4)->Add(-5)->Add(-6)->Add(-7);
	tree->Add(-8)->Add(-9)->Add(-10)->Add(-11);

	print(tree, '\n');

	tree->Delete(-3);
	print(tree, '\n');

	auto resNode = tree->isThere(-4);
	print(resNode, '\n');

	Sequence<TraverseOrder>* seq = Allocator<TraverseOrder>::AllocateArraySequence(Right, Right, Left);

	try
	{
		auto res = tree->Find(seq);
		print(res->data, '\n');
	}
	catch (Exception e)
	{
		print("Error was made\n");
	}

	TreePtr<int, int> secondTree = (Tree<int, int>*) new BinaryTree<int, int>(1, startKGen);

	secondTree->Add(10)->Add(12)->Add(-1)->Add(-2)->Add(0);

	print(secondTree, '\n');

	BinTreePtr<int, int> res = (BinaryTree<int, int>*) tree->Merge(secondTree.Get());

	print(res, '\n');
	res->Delete(-7);
	print(res, '\n');
	res->Delete(-5);
	print(res, '\n');

	delete seq;
	return 0;
}