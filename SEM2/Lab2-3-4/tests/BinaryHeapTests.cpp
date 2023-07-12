#include "Logger.h"
#include "moduleTest.h"

#include "Pair.h"
#include "Sequence/ListSequence.h"
#include "Tree/BinaryHeap.h"
#include "str/CString.h"

template <typename T1, typename T2>
using BinHeapPtr = SharedPtr<BinaryHeap<T1, T2>>;

template <typename T1, typename T2>
using ConstTreePtr = SharedPtr<const Tree<T1, T2>>;

template <typename T1, typename T2>
using TreeNode = typename Tree<T1, T2>::TreeNode;

template <typename T1, typename T2>
using NodePtr = TreeNode<T1, T2>*;

/*
BinaryHeap constructors is just constructors of BinaryTree class and
because of that there is no need for testing them
*/

TEST_CASE("Add is working correctly")
{
	Pair<int, String> startValue = { 100, "root" };
	auto kGen = [](const Pair<int, String>& val) -> int { return val.GetLeft(); };

	BinHeapPtr<int, Pair<int, String>> heap = new BinaryHeap<int, Pair<int, String>>(startValue, kGen);

	REQUIRE(heap->GetRoot()->data.GetRight() == "root");
	REQUIRE(heap->GetRoot()->data.GetLeft() == 100);

	heap->Add({ 25, "left" })->Add({ 50, "right" });

	REQUIRE(heap->GetRoot()->left->data.GetLeft() == 25);
	REQUIRE(heap->GetRoot()->left->data.GetRight() == "left");

	REQUIRE(heap->GetRoot()->right->data.GetLeft() == 50);
	REQUIRE(heap->GetRoot()->right->data.GetRight() == "right");

	heap->Add({ 101, "new root" })->Add({ 99, "new left" })->Add({ -1, "bottom node" });

	// adding node with same priority to check (not an error until it can't be inserted)
	heap->Add({ 50, "just" })->Add({ 50, "as" })->Add({ 2, "intended" });

	// check for root change
	REQUIRE(heap->GetRoot()->data.GetRight() == "new root");
	REQUIRE(heap->GetRoot()->data.GetLeft() == 101);

	// check for all
	heap->Traverse(Left, Root, Right, [](NodePtr<int, Pair<int, String>> node) -> void {
		if (node->left != nullptr)
			REQUIRE(node->key > node->left->key);

		if (node->right != nullptr)
			REQUIRE(node->key > node->right->key);
	});

	// check for balancing
	size_t depthOfLeftSub = heap->Depth(heap->GetRoot()->left.Get(), 0);
	size_t depthOfRightSub = heap->Depth(heap->GetRoot()->right.Get(), 0);
	int diff = depthOfLeftSub - depthOfRightSub;

	if (diff > 0)
		REQUIRE(diff < 2);
	else
		REQUIRE(diff > -2);
}

TEST_CASE("Delete method deletes correctly")
{
	Pair<int, String> startValue = { 100, "root" };
	auto kGen = [](const Pair<int, String>& val) -> int { return val.GetLeft(); };

	BinHeapPtr<int, Pair<int, String>> heap = new BinaryHeap<int, Pair<int, String>>(startValue, kGen);
	heap->Add({ 25, "left" })->Add({ 50, "right" });

	// check deletion of non-roots
	REQUIRE(heap->isThere({ 25, "left" }) == true);
	heap->Delete({ 25, "left" });
	REQUIRE(heap->isThere({ 25, "left" }) == false);

	// check deletion of root
	REQUIRE(heap->isThere({ 100, "root" }) == true);
	heap->Delete({ 100, "root" });
	REQUIRE(heap->isThere({ 100, "root" }) == false);

	// confirmation
	REQUIRE(heap->GetRoot()->data != startValue);
}
