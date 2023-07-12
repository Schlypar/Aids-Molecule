#pragma once

#include "Logger.h"

#include "Sequence/Stack.h"
#include "concepts.h"

#include "Logger.h"
#include "Pointer.h"
#include "Sequence/IContainer.h"
#include "Sequence/Sequence.h"
#include <functional>

// Needed for traverse method.
// Serves as parameters for the order of traversal of a tree
enum TraverseOrder
{
	Left,
	Root,
	Right
};

// Function that will generate key for key-value pairs
template <typename Tkey, typename Tvalue>
using KGen = Tkey (*)(const Tvalue&);

template <typename T>
using Multi = Stack<T>;

template <typename T>
using MultiRaw = Stack<T>*;

// template <typename Tkey, typename Tvalue>
// using KGenMulti = Tkey (*)(const Tvalue&);

template <typename Tkey, typename Tvalue>
class Tree
{
public:
	struct TreeNode
	{
		Tkey key;
		Tvalue data;

		TreeNode* parent;
		WeakPtr<TreeNode> left;
		WeakPtr<TreeNode> right;

		TreeNode()
		    : key(Tkey())
		    , data(Tvalue())
		    , parent(nullptr)
		    , left(nullptr)
		    , right(nullptr)
		{
			key = kGen(data);
		}

		TreeNode(const Tvalue& value)
		    : key(Tkey())
		    , data(value)
		    , parent(nullptr)
		    , left(nullptr)
		    , right(nullptr)
		{
		}

		TreeNode(Tvalue&& value)
		    : key(Tkey())
		    , data(std::move(value))
		    , parent(nullptr)
		    , left(nullptr)
		    , right(nullptr)
		{
		}

		TreeNode(TreeNode&& other)
		{
			key = other.key;
			other.key = Tkey();

			data = other.data;
			other.data = Tvalue();

			parent = other.parent;
			other.parent = nullptr;

			left = other.left;
			other.left = nullptr;

			right = other.right;
			other.right = nullptr;
		}

		~TreeNode()
		{
		}

		// Finds the most right node from the current one.
		// So right->right... until there is no right.
		//
		// Returns null if there is no right node.
		friend TreeNode* MostRight(TreeNode* startNode) noexcept
		{
			if (startNode == nullptr)
				return nullptr;

			TreeNode* current = startNode;

			while (current->right != nullptr)
				current = current->right.Get();

			return (current != startNode) ? current : nullptr;
		}

		// Finds the most left node from the current one.
		// So left->left... until there is no left.
		//
		// Returns null if there is no left node.
		friend TreeNode* MostLeft(TreeNode* startNode) noexcept
		{
			if (startNode == nullptr)
				return nullptr;

			TreeNode* current = startNode;

			while (current->left != nullptr)
				current = current->left.Get();

			return (current != startNode) ? current : nullptr;
		}
	};

public:
	template <typename T1, typename T2>
	using NodePtr = TreeNode*;

	virtual ~Tree()
	{
		Logger::Info("Destroyed Tree<T>");
	}

	Tree<Tkey, Tvalue>* Map(Func<Tvalue> func);

	Tree<Tkey, Tvalue>* Map(Func<Tvalue> func) const;

	Tree<Tkey, Tvalue>* Where(Condition<Tvalue> condition) const noexcept;

	Tree<Tkey, Tvalue>* Where(Condition<Tvalue> condition) noexcept;

	Tree<Tkey, Tvalue>* Concat(Tree<Tkey, Tvalue>* other) noexcept;

	Tree<Tkey, Tvalue>* Concat(Tree<Tkey, Tvalue>* other) const noexcept;

	// Same as concat. Only becomes useful for Multi<T> cases
	Tree<Tkey, Tvalue>* Merge(Tree<Tkey, Tvalue>* other) noexcept;

	// Same as concat. Only becomes useful for Multi<T> cases
	Tree<Tkey, Tvalue>* Merge(Tree<Tkey, Tvalue>* other) const noexcept;

	// Finds node by value and returns pointer to it.
	virtual NodePtr<Tkey, Tvalue> Find(const Tvalue& value) const noexcept = 0;
	// Finds node by path and returns pointer to it. Throws if path is invalid.
	virtual NodePtr<Tkey, Tvalue> Find(Sequence<TraverseOrder>* sequenceOfTraversion) const = 0;

	// Checks if value is present in the tree.
	virtual bool isThere(const Tvalue& value) const noexcept = 0;

	// Calculates depth of a tree starting at the startNode
	virtual Size Depth(NodePtr<Tkey, Tvalue> startNode) const noexcept = 0;
	// Calculates depth of a tree starting at the startNode with parametrized initial depth
	virtual Size Depth(NodePtr<Tkey, Tvalue> startNode, Size depth) const noexcept = 0;
	// Calculates depth of a tree starting at the root
	virtual Size Depth() const noexcept = 0;

	// Inserts value to a tree
	virtual Tree<Tkey, Tvalue>* Add(const Tvalue& value) noexcept = 0;

	// Deletes value from a tree
	virtual void Delete(const Tvalue& value) noexcept = 0;

	// Returns pointer to the root node
	virtual NodePtr<Tkey, Tvalue> GetRoot() const noexcept = 0;

	// Travesing tree in specified order and maps some function to the values in nodes.
	virtual void Traverse(NodePtr<Tkey, Tvalue> startNode, TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(Tvalue&)> func) = 0;
	// Travesing tree in specified order and maps some function to the values in nodes. This method is for immutable data
	virtual void Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(Tvalue&)> func) = 0;

	// Travesing tree in specified order and maps some function to the nodes.
	virtual void Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(NodePtr<Tkey, Tvalue>)> func) = 0;
	// Travesing tree in specified order and maps some function to the nodes. This method is for immutable data
	virtual void Traverse(NodePtr<Tkey, Tvalue> startNode, TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(NodePtr<Tkey, Tvalue>)> func) = 0;

	// Traverse for chaining multiple actions on a single tree
	virtual Tree<Tkey, Tvalue>* Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(Tvalue&)> func) const = 0;

	// Returns diff of depths of left and right subtrees starting from root node
	virtual int BalanceFactor() const noexcept = 0;
	// Returns diff of depths of left and right subtrees starting from 'startNode'
	virtual int BalanceFactor(NodePtr<Tkey, Tvalue> startNode) const noexcept = 0;
	// Function that balances a tree. Automatically calls itself after each Add or Delete methods
	virtual void Balance() noexcept = 0;

	virtual Tree<Tkey, Tvalue>* Create() const noexcept = 0;
	virtual Tree<Tkey, Tvalue>* Create(TreeNode* root) const noexcept = 0;
	virtual Tree<Tkey, Tvalue>* Copy() const noexcept = 0;

	virtual void CopyNodes(NodePtr<Tkey, Tvalue> copyNode, const NodePtr<Tkey, Tvalue> originalNode) const noexcept = 0;
	virtual void CopyNodes(NodePtr<Tkey, Tvalue> copyNode, const NodePtr<Tkey, Tvalue> originalNode, bool (*filter)(Tvalue&)) const noexcept = 0;

	virtual std::ostream& Dump(std::ostream& stream, const NodePtr<Tkey, Tvalue> startNode, TraverseOrder first, TraverseOrder second, TraverseOrder third) const noexcept = 0;
	virtual void Dump(TraverseOrder first, TraverseOrder second, TraverseOrder third) const noexcept = 0;

	friend std::ostream& operator<<(std::ostream& stream, const Tree<Tkey, Tvalue>& tree)
	{
		tree.Dump(stream, tree.GetRoot(), Left, Root, Right);

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const SharedPtr<Tree<Tkey, Tvalue>>& tree)
	{
		tree->Dump(stream, tree->GetRoot(), Left, Root, Right);

		return stream;
	}
};

template <typename Tkey, typename Tvalue>
Tree<Tkey, Tvalue>* Tree<Tkey, Tvalue>::Map(Func<Tvalue> func)
{
	Traverse(Left, Root, Right, func);

	return this;
}

template <typename Tkey, typename Tvalue>
Tree<Tkey, Tvalue>* Tree<Tkey, Tvalue>::Map(Func<Tvalue> func) const
{
	Tree<Tkey, Tvalue>* result = this->Copy();

	result->Traverse(Left, Root, Right, func);

	return result;
}

template <typename Tkey, typename Tvalue>
Tree<Tkey, Tvalue>* Tree<Tkey, Tvalue>::Where(Condition<Tvalue> condition) const noexcept
{
	Tree<Tkey, Tvalue>* result = Create();

	auto copy = this->Traverse(Root, Left, Right, [result, condition](Tvalue& val) -> void {
		if (condition(val))
			result->Add(val);
	});

	delete copy;

	return result;
}

template <typename Tkey, typename Tvalue>
Tree<Tkey, Tvalue>* Tree<Tkey, Tvalue>::Where(Condition<Tvalue> condition) noexcept
{
	Tree<Tkey, Tvalue>* result = Create();

	this->Traverse(Root, Left, Right, [result, condition](Tvalue& val) -> void {
		if (condition(val))
			result->Add(val);
	});

	return result;
}

template <typename Tkey, typename Tvalue>
Tree<Tkey, Tvalue>* Tree<Tkey, Tvalue>::Concat(Tree<Tkey, Tvalue>* other) noexcept
{
	auto addEvery = [this, other](const Tvalue& value) -> void { this->Add(value); };

	other->Traverse(Left, Root, Right, addEvery);

	return this;
}

template <typename Tkey, typename Tvalue>
Tree<Tkey, Tvalue>* Tree<Tkey, Tvalue>::Concat(Tree<Tkey, Tvalue>* other) const noexcept
{
	auto result = this->Copy();

	auto addEvery = [result, other](const Tvalue& value) -> void { result->Add(value); };

	other->Traverse(Left, Right, Root, addEvery);

	result->Balance();

	return result;
}

template <typename Tkey, typename Tvalue>
Tree<Tkey, Tvalue>* Tree<Tkey, Tvalue>::Merge(Tree<Tkey, Tvalue>* other) noexcept
{
	auto isUnique = [this, other](Tvalue& value) -> void {
		if (!(this->isThere(value)) && other->isThere(value))
			this->Add(value);
	};

	other->Traverse(Left, Root, Right, isUnique);

	this->Balance();

	return this;
}

template <typename Tkey, typename Tvalue>
Tree<Tkey, Tvalue>* Tree<Tkey, Tvalue>::Merge(Tree<Tkey, Tvalue>* other) const noexcept
{
	auto result = this->Copy();

	auto addIfUnique = [this, other, result](Tvalue& value) -> void {
		if (!(this->isThere(value)) && other->isThere(value))
			result->Add(value);
	};

	other->Traverse(Left, Root, Right, addIfUnique);

	result->Balance();

	return result;
}
