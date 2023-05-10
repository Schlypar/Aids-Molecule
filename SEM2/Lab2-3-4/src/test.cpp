#include "Tree/BinaryTree.h"
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

	BinTreePtr<int, int> tree = (BinaryTree<int, int>*) new BinaryTree<int, int>(1, [](int value) -> int { return value; });
	tree->Add(-1)->Add(-2);
	tree->Add(-3);
	tree->Add(-4)->Add(-5)->Add(-6)->Add(-7);
	tree->Add(-8)->Add(-9)->Add(-10)->Add(-11);
	print(tree, '\n');

	// auto rootbf = tree->BalanceFactor(tree->GetRoot());
	// print(rootbf, '\n');

	// ConstTreePtr<int, int> copy = tree->Copy();

	// print(*copy, '\n');

	// TreePtr<int, int> res = copy->Map(
	// 	[](int& value) -> int
	// 	{
	// 		value = value * 10;
	// 		return value;
	// 	});

	// print(*copy, '\n');
	// print(res, '\n');

	// BinTreePtr<int, int> res2 = (BinaryTree<int, int>*) tree->Where([](int& value) -> bool { return value % 2 == 0;
	// });

	// print(res2, '\n');

	// print(tree, '\n');
	// tree->LeftRightRotation();
	// // tree->Test();
	// print(tree, '\n');

	return 0;
}