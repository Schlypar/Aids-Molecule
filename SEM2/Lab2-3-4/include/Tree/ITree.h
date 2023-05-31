#pragma once

#include "Logger.h"

#include "Sequence/Stack.h"
#include "concepts.h"

#include "Logger.h"
#include "Pointer.h"
#include "Sequence/IContainer.h"
#include "Sequence/Sequence.h"
#include <functional>

enum TraverseOrder
{
	Left,
	Root,
	Right
};

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

		// TreeNode(const Tvalue& value, KGen<Tkey, Tvalue> kGen)
		//     : key(Tkey())
		//     , data(value)
		//     , parent(nullptr)
		//     , left(nullptr)
		//     , right(nullptr)
		// {
		// 	key = kGen(data);
		// }

		// TreeNode(const Tvalue& value, KGenMulti<Tkey, Tvalue> kGen)
		//     : key(Tkey(value))
		//     , data(value)
		//     , parent(nullptr)
		//     , left(nullptr)
		//     , right(nullptr)
		// {
		// 	key = kGenMulti(data);
		// }

		// TreeNode(const Tkey& key, const Tvalue& value)
		//     : key(key)
		//     , data(value)
		//     , kGen([](Tvalue value) -> Tkey { return Tkey(value); })
		//     , kGenMulti([](Stack<Tvalue> stack) -> Tkey { return Tkey(stack.Peek(0)); })
		//     , parent(nullptr)
		//     , left(nullptr)
		//     , right(nullptr)
		// {
		// 	key = kGen(data);
		// }

		// TreeNode(const Tkey& key, const Tvalue& value, KGen<Tkey, Tvalue> kGen)
		//     : key(key)
		//     , data(value)
		//     , kGen(kGen)
		//     , kGenMulti([](Stack<Tvalue> stack) -> Tkey { return Tkey(stack.Peek(0)); })
		//     , parent(nullptr)
		//     , left(nullptr)
		//     , right(nullptr)
		// {
		// 	// key = kGen(data);
		// }

		// TreeNode(const Tkey& key, const Tvalue& value, KGenMulti<Tkey, Tvalue> kGen)
		//     : key(key)
		//     , data(value)
		//     , kGen(nullptr)
		//     , kGenMulti(kGen)
		//     , parent(nullptr)
		//     , left(nullptr)
		//     , right(nullptr)
		// {
		// 	// key = kGenMulti(data);
		// }

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

		friend TreeNode* MostRight(TreeNode* startNode) noexcept
		{
			if (startNode == nullptr)
				return nullptr;

			TreeNode* current = startNode;

			while (current->right != nullptr)
				current = current->right.Get();

			return (current != startNode) ? current : nullptr;
		}

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

	Tree<Tkey, Tvalue>* Map(Func<Tvalue> func)
	{
		Traverse(Left, Root, Right, func);

		return this;
	}

	Tree<Tkey, Tvalue>* Map(Func<Tvalue> func) const
	{
		Tree<Tkey, Tvalue>* result = this->Copy();

		result->Traverse(Left, Root, Right, func);

		return result;
	}

	Tree<Tkey, Tvalue>* Where(Condition<Tvalue> condition) const noexcept
	{
		Tree<Tkey, Tvalue>* result = Create();

		auto copy = this->Traverse(Root, Left, Right, [result, condition](Tvalue& val) -> void {
			if (condition(val))
				result->Add(val);
		});

		delete copy;

		return result;
	}

	Tree<Tkey, Tvalue>* Where(Condition<Tvalue> condition) noexcept
	{
		Tree<Tkey, Tvalue>* result = Create();

		this->Traverse(Root, Left, Right, [result, condition](Tvalue& val) -> void {
			if (condition(val))
				result->Add(val);
		});

		return result;
	}

	Tree<Tkey, Tvalue>* Concat(Tree<Tkey, Tvalue>* other) noexcept
	{
		auto addEvery = [this, other](const Tvalue& value) -> void { this->Add(value); };

		other->Traverse(Left, Root, Right, addEvery);

		return this;
	}

	Tree<Tkey, Tvalue>* Concat(Tree<Tkey, Tvalue>* other) const noexcept
	{
		auto result = this->Copy();

		auto addEvery = [result, other](const Tvalue& value) -> void { result->Add(value); };

		other->Traverse(Left, Right, Root, addEvery);

		result->Balance();

		return result;
	}

	Tree<Tkey, Tvalue>* Merge(Tree<Tkey, Tvalue>* other) noexcept
	{
		auto isUnique = [this, other](Tvalue& value) -> void {
			if (!(this->isThere(value)) && other->isThere(value))
				this->Add(value);
		};

		other->Traverse(Left, Root, Right, isUnique);

		this->Balance();

		return this;
	}

	// only becomes useful for Multi<T> cases
	Tree<Tkey, Tvalue>* Merge(Tree<Tkey, Tvalue>* other) const noexcept
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

	virtual NodePtr<Tkey, Tvalue> Find(const Tvalue& value) const noexcept = 0;
	virtual NodePtr<Tkey, Tvalue> Find(Sequence<TraverseOrder>* sequenceOfTraversion) const = 0;

	virtual bool isThere(const Tvalue& value) const noexcept = 0;

	virtual Size Depth(NodePtr<Tkey, Tvalue> startNode) const noexcept = 0;
	virtual Size Depth(NodePtr<Tkey, Tvalue> startNode, Size depth) const noexcept = 0;
	virtual Size Depth() const noexcept = 0;

	virtual Tree<Tkey, Tvalue>* Add(const Tvalue& value) noexcept = 0;
	virtual void Delete(const Tvalue& value) noexcept = 0;
	virtual NodePtr<Tkey, Tvalue> GetRoot() const noexcept = 0;

	virtual void Traverse(NodePtr<Tkey, Tvalue> startNode, TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(Tvalue&)> func) = 0;
	virtual void Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(Tvalue&)> func) = 0;

	virtual void Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(NodePtr<Tkey, Tvalue>)> func) = 0;
	virtual void Traverse(NodePtr<Tkey, Tvalue> startNode, TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(NodePtr<Tkey, Tvalue>)> func) = 0;

	virtual Tree<Tkey, Tvalue>* Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(Tvalue&)> func) const = 0;

	virtual int BalanceFactor() const noexcept = 0;
	virtual int BalanceFactor(NodePtr<Tkey, Tvalue> startNode) const noexcept = 0;
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
