
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

	String lol = "abcd ";
	print(lol, '\n');

	String lol1 = lol + " _<====>__---><---  ";
	print(lol1, '\n');

	String lol2 = lol * 2;
	print(lol2, '\n');

	Size index = lol2.LFind('a');
	print(index, '\n');

	auto p = lol2.Split(' ');
	String left = p.GetLeft();
	String right = p.GetRight();
	print(left, " | ", right, '\n');

	if (lol2.isThere(" abc"))
		print("Wow\n");
	else
		print("Not so wow\n");

	lol1.Slice(6, "AAAAAAA");
	print(lol1, '\n');

	Size countA = lol2.Count('a');
	print(countA, '\n');

	lol1.Replace('A', 'a', lol1.Count('A'));
	print(lol1, '\n');

	return 0;
}
