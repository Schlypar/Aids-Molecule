#include "Logger.h"
#include "moduleTest.h"

#include "Sequence/ListSequence.h"
#include "Tree/BinaryTree.h"

template <typename T1, typename T2>
using BinTreePtr = SharedPtr<BinaryTree<T1, T2>>;

template <typename T1, typename T2>
using ConstTreePtr = SharedPtr<const Tree<T1, T2>>;

template <typename T1, typename T2>
using TreeNode = typename Tree<T1, T2>::TreeNode;

template <typename T1, typename T2>
using NodePtr = TreeNode<T1, T2>*;

TEST_CASE("Default constructor")
{
	BinTreePtr<int, int> tree = new BinaryTree<int, int>();

	REQUIRE(tree->GetRoot() == nullptr);
}

TEST_CASE("Binary tree can be constructed from a starting value")
{
	int startValue = 1;
	BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue);

	REQUIRE(tree->GetRoot() != nullptr);
	REQUIRE(tree->GetRoot()->data == startValue);
	REQUIRE(tree->GetRoot()->key == startValue);
}

TEST_CASE("Binary tree can be constructed with a value and a function to map it into another type")
{
	double startValue = 1.5;
	auto kGen = [](const double& val) -> int { return int(val); };

	BinTreePtr<int, double> tree = new BinaryTree<int, double>(startValue, kGen);

	REQUIRE(tree->GetRoot() != nullptr);
	REQUIRE(tree->GetRoot()->data == startValue);
	REQUIRE(tree->GetRoot()->key == kGen(startValue));
}

TEST_CASE("Binary tree Add() is working correctly")
{
	int startValue = 1;
	auto kGen = [](const int& val) -> int { return val; };
	BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
	tree->Add(0)->Add(2);

	REQUIRE(tree->GetRoot()->key < tree->GetRoot()->right->key);
	REQUIRE(tree->GetRoot()->key > tree->GetRoot()->left->key);

	tree->Add(10)->Add(5)->Add(6)->Add(-1)->Add(-2)->Add(-5);
	REQUIRE(tree->GetRoot()->key < tree->GetRoot()->right->key);
	REQUIRE(tree->GetRoot()->key > tree->GetRoot()->left->key);
}

TEST_CASE("Binary tree can be copied from another")
{
	int startValue = 1;
	BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue);
	tree->Add(0)->Add(2);

	BinTreePtr<int, int> copy = new BinaryTree<int, int>(*tree);

	REQUIRE(tree->GetRoot() != copy->GetRoot());

	REQUIRE(tree->GetRoot()->data == copy->GetRoot()->data);
	REQUIRE(tree->GetRoot()->key == copy->GetRoot()->key);

	REQUIRE(tree->GetRoot()->left->data == copy->GetRoot()->left->data);
	REQUIRE(tree->GetRoot()->left->key == copy->GetRoot()->left->key);

	REQUIRE(tree->GetRoot()->left->data == copy->GetRoot()->left->data);
	REQUIRE(tree->GetRoot()->left->key == copy->GetRoot()->left->key);
}

TEST_CASE("Binary tree can be copied from pointer to another")
{
	int startValue = 1;
	BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue);
	tree->Add(0)->Add(2);

	BinTreePtr<int, int> copy = new BinaryTree<int, int>(*tree.Get());

	REQUIRE(tree->GetRoot() != copy->GetRoot());

	REQUIRE(tree->GetRoot()->data == copy->GetRoot()->data);
	REQUIRE(tree->GetRoot()->key == copy->GetRoot()->key);

	REQUIRE(tree->GetRoot()->left->data == copy->GetRoot()->left->data);
	REQUIRE(tree->GetRoot()->left->key == copy->GetRoot()->left->key);

	REQUIRE(tree->GetRoot()->left->data == copy->GetRoot()->left->data);
	REQUIRE(tree->GetRoot()->left->key == copy->GetRoot()->left->key);
}

TEST_CASE("Depth calculated correctly")
{
	SECTION("Depth() is working correct")
	{
		BinTreePtr<int, int> tree = new BinaryTree<int, int>();

		size_t depth = tree->Depth();
		REQUIRE(depth == 0);

		tree->Add(0)->Add(1)->Add(-1);

		depth = tree->Depth();
		REQUIRE(depth == 2);

		tree->Add(2);
		depth = tree->Depth();
		REQUIRE(depth == 3);
	}

	SECTION("Depth(NodePtr<Tkey, Tvalue>) is working correct")
	{
		BinTreePtr<int, int> tree = new BinaryTree<int, int>();

		size_t depth = tree->Depth(tree->GetRoot());
		REQUIRE(depth == 0);

		tree->Add(0)->Add(1)->Add(-1);

		depth = tree->Depth(tree->GetRoot()->right.Get());
		REQUIRE(depth == 1);

		tree->Add(2);
		depth = tree->Depth(tree->GetRoot()->right.Get());
		REQUIRE(depth == 2);
	}

	SECTION("Depth(NodePtr<Tkey, Tvalue>, Size) is working correct")
	{
		BinTreePtr<int, int> tree = new BinaryTree<int, int>();

		size_t depth = tree->Depth(tree->GetRoot(), 0);
		REQUIRE(depth == 0);

		tree->Add(0)->Add(1)->Add(-1);

		depth = tree->Depth(tree->GetRoot()->right.Get(), 1);
		REQUIRE(depth == 2);

		tree->Add(2);
		depth = tree->Depth(tree->GetRoot()->right.Get(), 1);
		REQUIRE(depth == 3);
	}
}

TEST_CASE("Find returns correct node")
{
	SECTION("Find(const Tvalue&) is working correctly")
	{
		int startValue = 1;
		auto kGen = [](const int& val) -> int { return val; };

		BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
		tree->Add(0)->Add(2)->Add(10)->Add(5)->Add(6)->Add(-1)->Add(-2)->Add(-5);

		auto node = tree->Find(0);
		REQUIRE(node->data == 0);

		node = tree->Find(10);
		REQUIRE(node->data == 10);

		node = tree->Find(-2);
		REQUIRE(node->data == -2);

		node = tree->Find(100500);
		REQUIRE(node == nullptr);
	}
	SECTION("Find(Sequence<TraverseOrder>*) is working correctly")
	{
		SECTION("No exceptions")
		{
			int startValue = 1;
			auto kGen = [](const int& val) -> int { return val; };

			BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
			tree->Add(0)->Add(2);

			ListSequence<TraverseOrder> seq = { Left };

			auto node = tree->Find(&seq);
			REQUIRE(node->data == 0);

			tree->Add(-1)->Add(-2);
			ListSequence<TraverseOrder> seq2 = { Left, Right };
			node = tree->Find(&seq2);
			REQUIRE(node->data == 0);
		}
		SECTION("Exceptions")
		{
			int startValue = 1;
			auto kGen = [](const int& val) -> int { return val; };

			BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
			tree->Add(0)->Add(2);

			tree->Add(-1)->Add(-2);
			ListSequence<TraverseOrder> seq2 = { Left, Root };
			REQUIRE_THROWS_AS(tree->Find(&seq2), Exception);

			ListSequence<TraverseOrder> seq3 = { Right, Right };
			REQUIRE_THROWS_AS(tree->Find(&seq3), Exception);
		}
	}
}

TEST_CASE("Delete method deletes node with correct value")
{
	SECTION("Deletion of non-root nodes")
	{
		int startValue = 1;
		auto kGen = [](const int& val) -> int { return val; };
		BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
		tree->Add(0)->Add(2)->Add(3)->Add(-1);

		REQUIRE(tree->isThere(0) == true);
		tree->Delete(0);
		REQUIRE(tree->isThere(0) == false);

		REQUIRE(tree->isThere(2) == true);
		tree->Delete(2);
		REQUIRE(tree->isThere(2) == false);

		REQUIRE(tree->Depth() == 2);
	}
	SECTION("Deletion of root node")
	{
		int startValue = 1;
		auto kGen = [](const int& val) -> int { return val; };
		BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
		tree->Add(0)->Add(2)->Add(3)->Add(-1);

		REQUIRE(tree->isThere(startValue) == true);
		tree->Delete(startValue);
		REQUIRE(tree->isThere(startValue) == false);

		REQUIRE(tree->GetRoot()->data != startValue);
	}
}

TEST_CASE("Map is working correctly")
{
	SECTION("Non-const map is working correctly")
	{
		int startValue = 1;
		auto kGen = [](const int& val) -> int { return val; };
		BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
		tree->Add(0)->Add(2)->Add(3)->Add(-1);

		REQUIRE(tree->GetRoot()->data == startValue);

		auto plusOne = [](int& val) -> int { // reusing from sequence hence dull return
			val++;
			return val;
		};

		tree->Map(plusOne);
		REQUIRE(tree->GetRoot()->data == startValue + 1);
	}
	SECTION("Const map working correctly")
	{
		int startValue = 1;
		auto kGen = [](const int& val) -> int { return val; };
		BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
		tree->Add(0)->Add(2)->Add(3)->Add(-1);

		ConstTreePtr<int, int> constTree = new BinaryTree<int, int>(tree.Get());

		REQUIRE(constTree->GetRoot()->data == startValue);

		auto plusOne = [](int& val) -> int { // reusing from sequence hence dull return
			val++;
			return val;
		};

		auto mappedTree = constTree->Map(plusOne);

		REQUIRE(mappedTree->GetRoot()->data == startValue + 1);
		REQUIRE(constTree->GetRoot()->data == startValue);
		REQUIRE(tree->GetRoot()->data == startValue);

		delete mappedTree;
	}
}

TEST_CASE("Where method filters Tree into a new tree")
{
	bool checkIfWorks = true;
	bool* ptrToCheck = &checkIfWorks;

	auto checkIfEven = [&ptrToCheck](const int& val) -> void {
		if (val % 2 != 0)
			*ptrToCheck = false;
	};

	auto checkIfOdd = [&ptrToCheck](const int& val) -> void {
		if (val % 2 == 0)
			*ptrToCheck = false;
	};

	int startValue = 0;
	auto kGen = [](const int& val) -> int { return val; };
	BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
	tree->Add(1)->Add(2)->Add(3)->Add(4)->Add(5)->Add(6);

	auto isEven = [](int& val) -> bool { return val % 2 == 0; };
	auto isOdd = [](int& val) -> bool { return val % 2 != 0; };

	auto evenedTree = tree->Where(isEven);
	REQUIRE(evenedTree->GetRoot() != tree->GetRoot());
	evenedTree->Traverse(Left, Right, Root, checkIfEven);
	REQUIRE(checkIfWorks == true);
	delete evenedTree;

	auto oddedTree = tree->Where(isOdd);
	REQUIRE(oddedTree->GetRoot() != tree->GetRoot());
	oddedTree->Traverse(Left, Right, Root, checkIfOdd);
	REQUIRE(checkIfWorks == true);
	delete oddedTree;
}

TEST_CASE("Concat method correctly concatenates two trees")
{
	int startValue = 0;
	auto kGen = [](const int& val) -> int { return val; };
	BinTreePtr<int, int> tree = new BinaryTree<int, int>(startValue, kGen);
	tree->Add(1)->Add(2);

	BinTreePtr<int, int> tree2 = new BinaryTree<int, int>(startValue, kGen);
	tree2->Add(-1)->Add(-2);

	auto concated = tree->Concat(tree2.Get());

	// check if concatanation was done in right order
	concated->Traverse(Left, Root, Right, [](NodePtr<int, int> node) -> void {
		if (node->left != nullptr)
			REQUIRE(node->key > node->left->key);

		if (node->right != nullptr)
			REQUIRE(node->key < node->right->key);
	});

	// check if it contains both of trees
	tree->Traverse(Left, Root, Right, [concated](const int& val) -> void { REQUIRE(concated->isThere(val)); });
	tree2->Traverse(Left, Root, Right, [concated](const int& val) -> void { REQUIRE(concated->isThere(val)); });
}
