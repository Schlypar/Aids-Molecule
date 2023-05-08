#include <ADT.h>

LogPriority Logger::priority = TracePriority;

template <typename T1, typename T2>
using TreePtr = SharedPtr<Tree<T1, T2>>;

template <typename T1, typename T2>
using ConstTreePtr = SharedPtr<const Tree<T1, T2>>;

int main()
{
	Logger::setPriority(ErrorPriority);

	TreePtr<int, int> tree = (Tree<int, int>*) new BinaryTree<int, int>(1,
		[](int value) -> int { return value * value / 2 - 1 - value; });
	tree->Add(2)->Add(5);

	tree->Add(-1);

	tree->Add(10)->Add(-10)->Add(15)->Add(-15);

	ConstTreePtr<int, int> copy = tree->Copy();

	print(*copy, '\n');

	TreePtr<int, int> res = copy->Map([](int& value) -> int {
		value = value * 10;
		return value;
	});

	print(*copy, '\n');
	print(res, '\n');

	TreePtr<int, int> res2 = copy->Where([](int& value) -> bool { return value % 2 == 0; });

	print(res2, '\n');

	return 0;
}