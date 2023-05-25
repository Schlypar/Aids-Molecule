#pragma once

#include "ITree.h"
#include "Logger.h"
#include "Pointer.h"
#include "Sequence/ArraySequence.h"
#include "Sequence/Sequence.h"

#define NOT_DONE true

template <Comparible Tkey, typename Tvalue>
class BinaryTree : public Tree<Tkey, Tvalue>
{
	template <typename T1, typename T2>
	using TreeNode = typename Tree<T1, T2>::TreeNode;

	template <typename T1, typename T2>
	using NodePtr = TreeNode<T1, T2>*;

protected:
	WeakPtr<TreeNode<Tkey, Tvalue>> root;
	KGen<Tkey, Tvalue> kGen = nullptr;

public:
	BinaryTree()
	    : root(nullptr)
	    , kGen([](const Tvalue& val) -> Tkey { return Tkey(val); })
	{
		Logger::Warn("Default constructor of BinaryTree<T>");
	}

	BinaryTree(const Tvalue& startValue)
	    : root(new TreeNode<Tkey, Tvalue>(startValue))
	    , kGen([](const Tvalue& val) -> Tkey { return Tkey(val); })
	{
		root->key = this->kGen(startValue);
		Logger::Info("Starting value constructor of BinaryTree<T>");
	}

	BinaryTree(const Tvalue& startValue, KGen<Tkey, Tvalue> kGen)
	    : root(new TreeNode<Tkey, Tvalue>(startValue))
	    , kGen(kGen)
	{
		root->key = this->kGen(startValue);
		Logger::Info("Starting value constructor of BinaryTree<T>");
	}

	BinaryTree(KGen<Tkey, Tvalue> kGen)
	    : root()
	    , kGen(kGen)
	{
		Logger::Info("Starting value constructor of BinaryTree<T>");
	}

	BinaryTree(TreeNode<Tkey, Tvalue>* startRoot, KGen<Tkey, Tvalue> kGen)
	    : root(startRoot)
	    , kGen(kGen)
	{
		Logger::Info("Starting root node constructor of BinaryTree<T>");
	}

	BinaryTree(const BinaryTree<Tkey, Tvalue>& other)
	    : root(nullptr)
	    , kGen(other.kGen)
	{
		if (other.root != nullptr)
		{
			root = new TreeNode<Tkey, Tvalue>(other.root->data);
			root->key = this->kGen(other.root->data);
			CopyNodes(root.Get(), other.root.Get());
		}
	}

	BinaryTree(const BinaryTree<Tkey, Tvalue>* other)
	    : root(nullptr)
	    , kGen(other->kGen)
	{
		if (other->root != nullptr)
		{
			root = new TreeNode<Tkey, Tvalue>(other->root->data);
			root->key = this->kGen(other->root->data);
			CopyNodes(root.Get(), other->root.Get());
		}
	}

	virtual ~BinaryTree()
	{
	}

	NodePtr<Tkey, Tvalue> GetRoot() const noexcept override
	{
		return root.Get();
	}

	Size Depth(NodePtr<Tkey, Tvalue> startNode) const noexcept override
	{
		if (startNode == nullptr)
			return 0;

		Size depth = 0;
		if (startNode->left)
			depth = std::max(depth, Depth(startNode->left.Get(), 1));

		if (startNode->right)
			depth = std::max(depth, Depth(startNode->right.Get(), 1));

		if (startNode->left == nullptr && startNode->right == nullptr)
			return 1;
		else
			return depth;
	}

	Size Depth(NodePtr<Tkey, Tvalue> startNode, Size depth) const noexcept override
	{
		if (startNode == nullptr)
			return depth;

		depth++;
		Size leftDepth = 0;
		Size rightDepth = 0;

		if (startNode->left)
			leftDepth = Depth(startNode->left.Get(), leftDepth);

		if (startNode->right)
			rightDepth = Depth(startNode->right.Get(), rightDepth);

		return depth + std::max(leftDepth, rightDepth);
	}

	Size Depth() const noexcept override
	{
		auto startNode = this->root.Get();

		if (startNode == nullptr)
			return 0;

		Size depth = 1;
		Size leftDepth = 0;
		Size rightDepth = 0;

		if (startNode->left)
			leftDepth = std::max(depth, Depth(startNode->left.Get(), leftDepth));

		if (startNode->right)
			rightDepth = std::max(depth, Depth(startNode->right.Get(), rightDepth));

		return depth + std::max(leftDepth, rightDepth);
	}

	NodePtr<Tkey, Tvalue> Find(const Tvalue& value) const noexcept override
	{
		if (root == nullptr)
			return nullptr;

		Tkey keyValue = this->kGen(value);

		NodePtr<Tkey, Tvalue> current = this->root.Get();

		while (NOT_DONE)
			if (current != nullptr && keyValue < current->key)
				current = current->left.Get();
			else if (current != nullptr && keyValue > current->key)
				current = current->right.Get();
			else if (current != nullptr && keyValue == current->key)
				return current;
			else if (current == nullptr)
				return nullptr;
	}

	NodePtr<Tkey, Tvalue> Find(Sequence<TraverseOrder>* sequence) const override
	{
		if (!(sequence->isEmpty()) && root == nullptr)
		{
			Logger::Trace("At Find(Sequence<TraverseOrder>*) at BinaryTree<T>");
			logException(EXCEPTION_BAD_CONTAINER);
			throw EXCEPTION_BAD_CONTAINER;
		}

		return Find(sequence, 0, GetRoot());
	}

	bool isThere(const Tvalue& value) const noexcept override
	{
		return (Find(value) == nullptr) ? false : true;
	}

	Tree<Tkey, Tvalue>* Add(const Tvalue& value) noexcept override
	{
		if (root == nullptr)
		{
			root = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(value));
			root->key = this->kGen(value);
			return this;
		}

		Tkey valueKey = this->kGen(value);

		NodePtr<Tkey, Tvalue> current = this->root.Get();

		while (NOT_DONE)
		{
			if (current->key == valueKey)
				return this;

			if (current->left == nullptr && valueKey < current->key)
			{
				current->left = new TreeNode<Tkey, Tvalue>(value);
				current->left->key = this->kGen(value);
				current->left->parent = current;

				Balance();
				return this;
			}

			if (current->right == nullptr && valueKey >= current->key)
			{

				current->right = new TreeNode<Tkey, Tvalue>(value);
				current->right->key = this->kGen(value);
				current->right->parent = current;

				Balance();
				return this;
			}

			if (valueKey < current->key)
				current = current->left.Get();
			else
				current = current->right.Get();
		}

		Balance();
		return this;
	}

	void DeleteRoot() noexcept
	{
		if (root->left == nullptr && root->right == nullptr)
		{
			delete GetRoot();
			this->root = nullptr;
		}
		else if (root->left != nullptr && root->right == nullptr)
		{
			UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = GetRoot();
			this->root = root->left.Get();

			nodeToBeDeleted->left = nullptr;
			nodeToBeDeleted->right = nullptr;
		}
		else if (root->left == nullptr && root->right != nullptr)
		{
			UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = GetRoot();
			this->root = root->right.Get();

			nodeToBeDeleted->left = nullptr;
			nodeToBeDeleted->right = nullptr;
		}
		else if (Depth(root->left.Get()) < Depth(root->right.Get()))
		{
			if (MostLeft(root->right.Get()) != nullptr)
			{
				UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = MostLeft(root->right.Get());
				root->data = std::move(nodeToBeDeleted->data);
				root->key = std::move(nodeToBeDeleted->key);

				if (nodeToBeDeleted->parent != nullptr)
					nodeToBeDeleted->parent->left = nullptr;

				nodeToBeDeleted = nullptr;
			}
			else
			{
				UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = GetRoot();
				root->right->left = root->left.Get();

				this->root = nodeToBeDeleted->right.Get();

				nodeToBeDeleted->left = nullptr;
				nodeToBeDeleted->right = nullptr;

				if (nodeToBeDeleted->parent != nullptr)
					nodeToBeDeleted->parent->right = nullptr;

				nodeToBeDeleted->left = nullptr;
				nodeToBeDeleted->right = nullptr;
				nodeToBeDeleted = nullptr;
			}
		}
		else if (MostRight(root->left.Get()) != nullptr)
		{
			UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = MostRight(root->left.Get());
			root->data = std::move(nodeToBeDeleted->data);
			root->key = std::move(nodeToBeDeleted->key);

			if (nodeToBeDeleted->parent != nullptr)
				nodeToBeDeleted->parent->left = nullptr;

			nodeToBeDeleted = nullptr;
		}
		else
		{
			UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = GetRoot();
			root->left->right = root->right.Get();

			this->root = nodeToBeDeleted->left.Get();

			nodeToBeDeleted->left = nullptr;
			nodeToBeDeleted->right = nullptr;

			if (nodeToBeDeleted->parent != nullptr)
				nodeToBeDeleted->parent->right = nullptr;

			nodeToBeDeleted->left = nullptr;
			nodeToBeDeleted->right = nullptr;
			nodeToBeDeleted = nullptr;
		}
	}

	void Delete(const Tvalue& value) noexcept override
	{
		if (root == nullptr)
			return;

		Tkey keyValue = this->kGen(value);

		if (keyValue == root->key)
		{
			DeleteRoot();
			Balance();
			return;
		}

		NodePtr<Tkey, Tvalue> current = GetRoot();

		while (NOT_DONE)
		{
			if (current == nullptr)
				return;

			if (keyValue < current->key)
				current = current->left.Get();
			else if (keyValue > current->key)
				current = current->right.Get();
			else if (Depth(current->left.Get()) < Depth(current->right.Get()))
			{
				if (MostLeft(current->right.Get()) != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = MostLeft(current->right.Get());

					nodeToBeDeleted->parent->left = nodeToBeDeleted->right;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					Balance();
					return;
				}
				else if (MostRight(current->left.Get()) != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = MostRight(current->left.Get());

					nodeToBeDeleted->parent->right = nodeToBeDeleted->left;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					Balance();
					return;
				}
				else if (current->right != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = current->right.Get();

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					nodeToBeDeleted->parent->right = nullptr;
					nodeToBeDeleted->parent = nullptr;

					Balance();
					return;
				}
				else if (current->left != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = current->left.Get();

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					nodeToBeDeleted->parent->left = nullptr;
					nodeToBeDeleted->parent = nullptr;

					Balance();
					return;
				}

				NodePtr<Tkey, Tvalue> parent = current->parent;
				current->parent = nullptr;

				(parent->left.Get() == current) ? parent->left = nullptr : parent->right = nullptr;
				current->parent = nullptr;

				delete current;
				Balance();
				return;
			}
			else
			{
				if (MostRight(current->left.Get()) != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = MostRight(current->left.Get());

					nodeToBeDeleted->parent->right = nodeToBeDeleted->left;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					Balance();
					return;
				}
				else if (MostLeft(current->right.Get()) != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = MostLeft(current->right.Get());

					nodeToBeDeleted->parent->left = nodeToBeDeleted->right;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					Balance();
					return;
				}
				else if (current->left != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = current->left.Get();

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					nodeToBeDeleted->parent->left = nullptr;
					nodeToBeDeleted->parent = nullptr;

					Balance();
					return;
				}
				else if (current->right != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Tvalue>> nodeToBeDeleted = current->right.Get();

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					nodeToBeDeleted->parent->right = nullptr;
					nodeToBeDeleted->parent = nullptr;

					Balance();
					return;
				}

				NodePtr<Tkey, Tvalue> parent = current->parent;
				(parent->left.Get() == current) ? parent->left = nullptr : parent->right = nullptr;
				current->parent = nullptr;

				delete current;
				Balance();
				return;
			}
		}
	}

	Tree<Tkey, Tvalue>* Create() const noexcept override
	{
		return (Tree<Tkey, Tvalue>*) new BinaryTree<Tkey, Tvalue>(this->kGen);
	}

	Tree<Tkey, Tvalue>* Create(TreeNode<Tkey, Tvalue>* root) const noexcept override
	{
		return (Tree<Tkey, Tvalue>*) new BinaryTree<Tkey, Tvalue>(root, this->kGen);
	}

	Tree<Tkey, Tvalue>* Copy() const noexcept override
	{
		return (Tree<Tkey, Tvalue>*) new BinaryTree<Tkey, Tvalue>(*this);
	}

	void Traverse(NodePtr<Tkey, Tvalue> startNode, TraverseOrder first, TraverseOrder second, TraverseOrder third,
		std::function<void(Tvalue&)> func) override
	{
		if (first == second || first == third || second == third)
		{
			Logger::Info("At Traverse(TraverseOrder, TraverseOrder, TraverseOrder) at BinaryTree<T>");
			logException(EXCEPTION_BAD_LOGIC);
			throw EXCEPTION_BAD_LOGIC;
		}

		if (startNode == nullptr)
			return;

		switch (first)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, func);
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, func);
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, func);
				break;
			default:
				break;
		}
	}

	Tree<Tkey, Tvalue>* Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third,
		std::function<void(Tvalue&)> func) const override
	{
		BinaryTree<Tkey, Tvalue>* result = new BinaryTree<Tkey, Tvalue>(*this);

		result->Traverse(first, second, third, func);

		return result;
	}

	void Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(Tvalue&)> func) override
	{
		if (first == second || first == third || second == third)
		{
			Logger::Info("At Traverse(TraverseOrder, TraverseOrder, TraverseOrder) at BinaryTree<T>");
			logException(EXCEPTION_BAD_LOGIC);
			throw EXCEPTION_BAD_LOGIC;
		}

		NodePtr<Tkey, Tvalue> startNode = this->root.Get();

		if (startNode == nullptr)
			return;

		switch (first)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, func);
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, func);
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, func);
				break;
			default:
				break;
		}
	}

	void Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third,
		std::function<void(NodePtr<Tkey, Tvalue>)> funcWithNodes) override
	{
		if (first == second || first == third || second == third)
		{
			Logger::Info("At Traverse(TraverseOrder, TraverseOrder, TraverseOrder) at BinaryTree<T>");
			logException(EXCEPTION_BAD_LOGIC);
			throw EXCEPTION_BAD_LOGIC;
		}

		NodePtr<Tkey, Tvalue> startNode = this->root.Get();

		if (startNode == nullptr)
			return;

		switch (first)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, funcWithNodes);
				break;
			case Root:
				funcWithNodes(startNode);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, funcWithNodes);
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, funcWithNodes);
				break;
			case Root:
				funcWithNodes(startNode);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, funcWithNodes);
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, funcWithNodes);
				break;
			case Root:
				funcWithNodes(startNode);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, funcWithNodes);
				break;
			default:
				break;
		}
	}

	void Traverse(NodePtr<Tkey, Tvalue> startNode, TraverseOrder first, TraverseOrder second, TraverseOrder third,
		std::function<void(NodePtr<Tkey, Tvalue>)> funcWithNodes) override
	{
		if (first == second || first == third || second == third)
		{
			Logger::Info("At Traverse(TraverseOrder, TraverseOrder, TraverseOrder) at BinaryTree<T>");
			logException(EXCEPTION_BAD_LOGIC);
			throw EXCEPTION_BAD_LOGIC;
		}

		if (startNode == nullptr)
			return;

		switch (first)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, funcWithNodes);
				break;
			case Root:
				funcWithNodes(startNode);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, funcWithNodes);
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, funcWithNodes);
				break;
			case Root:
				funcWithNodes(startNode);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, funcWithNodes);
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				Traverse(startNode->left.Get(), first, second, third, funcWithNodes);
				break;
			case Root:
				funcWithNodes(startNode);
				break;
			case Right:
				Traverse(startNode->right.Get(), first, second, third, funcWithNodes);
				break;
			default:
				break;
		}
	}

	void Dump(TraverseOrder first, TraverseOrder second, TraverseOrder third) const noexcept override
	{
		Dump(std::cout, GetRoot(), first, second, third);
	}

	friend std::ostream& operator<<(std::ostream& stream, const BinaryTree<Tkey, Tvalue>& tree)
	{
		if (tree.root != nullptr)
			tree.Dump(stream, tree.GetRoot(), Left, Root, Right);

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const SharedPtr<BinaryTree<Tkey, Tvalue>>& tree)
	{
		if (tree->root != nullptr)
			tree->Dump(stream, tree->GetRoot(), Left, Root, Right);

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const BinaryTree<Tkey, Tvalue>* tree)
	{
		if (tree->root != nullptr)
			tree->Dump(stream, tree->GetRoot(), Left, Root, Right);

		return stream;
	}

	void LeftLeftRotation() noexcept
	{
		root = LeftLeftRotation(root.Get());
	}

	void RightRightRotation() noexcept
	{
		root = RightRightRotation(root.Get());
	}

	void RightLeftRotation() noexcept
	{
		root = RightLeftRotation(root.Get());
	}

	void LeftRightRotation() noexcept
	{
		root = LeftRightRotation(root.Get());
	}

protected:
	void CopyNodes(NodePtr<Tkey, Tvalue> copyNode, const NodePtr<Tkey, Tvalue> originalNode) const noexcept override
	{
		if (originalNode->left)
		{
			copyNode->left = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->left->data));
			copyNode->left->key = originalNode->left->key;

			copyNode->left->parent = copyNode;
			CopyNodes(copyNode->left.Get(), originalNode->left.Get());
		}
		if (originalNode->right)
		{
			copyNode->right = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->right->data));
			copyNode->right->key = originalNode->right->key;

			copyNode->right->parent = copyNode;
			CopyNodes(copyNode->right.Get(), originalNode->right.Get());
		}
	}

	void CopyNodes(NodePtr<Tkey, Tvalue> copyNode, const NodePtr<Tkey, Tvalue> originalNode,
		bool (*filter)(Tvalue& value)) const noexcept override
	{
		if (originalNode->left)
		{
			if (filter(originalNode->left->data))
			{
				copyNode->left = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->left->data));
				copyNode->left->key = originalNode->left->key;

				copyNode->left->parent = copyNode;
				CopyNodes(copyNode->left.Get(), originalNode->left.Get(), filter);
			}
			else
				CopyNodes(copyNode, originalNode->left.Get(), filter);
		}
		if (originalNode->right)
		{
			if (filter(originalNode->right->data))
			{
				copyNode->right = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->right->data));
				copyNode->right->key = originalNode->right->key;

				copyNode->right->parent = copyNode;
				CopyNodes(copyNode->right.Get(), originalNode->right.Get(), filter);
			}
			else
				CopyNodes(copyNode, originalNode->right.Get(), filter);
		}
	}

	NodePtr<Tkey, Tvalue> Find(Sequence<TraverseOrder>* sequence, const Index index, NodePtr<Tkey, Tvalue> node) const
	{
		if (node == nullptr)
		{
			Logger::Trace("At Find(Sequence<TraverseOrder>*, const Index, NodePtr) at BinaryTree<T>");
			logException(EXCEPTION_BAD_LOGIC);
			throw EXCEPTION_BAD_LOGIC;
		}

		if (sequence->GetLength() == index)
			return node;

		switch (sequence->Get(index))
		{
			case Left:
				return Find(sequence, index + 1, node->left.Get());
				break;
			case Right:
				return Find(sequence, index + 1, node->right.Get());
			default:
				Logger::Trace("At Find(Sequence<TraverseOrder>*, const Index, NodePtr) at BinaryTree<T>");
				logException(EXCEPTION_BAD_LOGIC);
				throw EXCEPTION_BAD_LOGIC;
		}
	}

	std::ostream& printTree(std::ostream& stream, const NodePtr<Tkey, Tvalue>& startNode, TraverseOrder first,
		TraverseOrder second, TraverseOrder third) const
	{
		if (first == second || first == third || second == third)
		{
			Logger::Info("At Traverse(TraverseOrder, TraverseOrder, TraverseOrder) at BinaryTree<T>");
			logException(EXCEPTION_BAD_LOGIC);
			throw EXCEPTION_BAD_LOGIC;
		}

		if (startNode == nullptr)
			return stream;

		switch (first)
		{
			case Left:
				if (startNode->left != nullptr)
				{
					stream << "{";
					printTree(stream, startNode->left.Get(), first, second, third);
					stream << "}";
				}
				break;
			case Root:
				stream << "(";
				stream << startNode->data;
				stream << ")";
				break;
			case Right:
				if (startNode->right != nullptr)
				{
					stream << "[";
					printTree(stream, startNode->left.Get(), first, second, third);
					stream << "]";
				}
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				if (startNode->left != nullptr)
				{
					stream << "{";
					printTree(stream, startNode->left.Get(), first, second, third);
					stream << "}";
				}
				break;
			case Root:
				stream << "(";
				stream << startNode->data;
				stream << ")";
				break;
			case Right:
				if (startNode->right != nullptr)
				{
					stream << "[";
					printTree(stream, startNode->left.Get(), first, second, third);
					stream << "]";
				}
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				{
					if (startNode->left != nullptr)
					{
						stream << "{";
						printTree(stream, startNode->left.Get(), first, second, third);
						stream << "}";
					}
					break;
				}
			case Root:
				{
					stream << "(";
					stream << startNode->data;
					stream << ")";
					break;
				}
			case Right:
				if (startNode->right != nullptr)
				{
					stream << "[";
					printTree(stream, startNode->right.Get(), first, second, third);
					stream << "]";
				}
				break;
			default:
				break;
		}

		return stream;
	}

	std::ostream& Dump(std::ostream& stream, const NodePtr<Tkey, Tvalue> startNode, TraverseOrder first, TraverseOrder second,
		TraverseOrder third) const noexcept override
	{
		switch (first)
		{
			case Left:
				stream << "{";
				printTree(stream, GetRoot()->left.Get(), first, second, third);
				stream << "}";
				break;
			case Root:
				stream << "(";
				stream << GetRoot()->data;
				stream << ")";
				break;
			case Right:
				stream << "[";
				printTree(stream, GetRoot()->right.Get(), first, second, third);
				stream << "]";
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				stream << "{";
				printTree(stream, GetRoot()->left.Get(), first, second, third);
				stream << "}";
				break;
			case Root:
				stream << "(";
				stream << GetRoot()->data;
				stream << ")";
				break;
			case Right:
				stream << "[";
				printTree(stream, GetRoot()->right.Get(), first, second, third);
				stream << "]";
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				stream << "{";
				printTree(stream, GetRoot()->left.Get(), first, second, third);
				stream << "}";
				break;
			case Root:
				stream << "(";
				stream << GetRoot()->data;
				stream << ")";
				break;
			case Right:
				stream << "[";
				printTree(stream, GetRoot()->right.Get(), first, second, third);
				stream << "]";
				break;
			default:
				break;
		}

		return stream;
	}

	NodePtr<Tkey, Tvalue> LeftLeftRotation(NodePtr<Tkey, Tvalue> node) noexcept
	{
		NodePtr<Tkey, Tvalue> son = node->left.Get();

		son->parent = node->parent;
		node->parent = son;

		node->left = son->right;
		son->right = node;

		return son;
	}

	NodePtr<Tkey, Tvalue> RightRightRotation(NodePtr<Tkey, Tvalue> node) noexcept
	{
		NodePtr<Tkey, Tvalue> son = node->right.Get();

		son->parent = node->parent;
		node->parent = son;

		node->right = son->left;
		son->left = node;

		return son;
	}

	NodePtr<Tkey, Tvalue> RightLeftRotation(NodePtr<Tkey, Tvalue> node) noexcept
	{
		NodePtr<Tkey, Tvalue> son = node->right.Get();
		NodePtr<Tkey, Tvalue> grandSon = son->left.Get();

		if (grandSon == nullptr || son == nullptr)
			return GetRoot();

		grandSon->parent = node->parent;
		son->parent = grandSon;
		node->parent = grandSon;

		node->right = grandSon->left;
		son->left = grandSon->right;

		grandSon->left = node;
		grandSon->right = son;

		return grandSon;
	}

	NodePtr<Tkey, Tvalue> LeftRightRotation(NodePtr<Tkey, Tvalue> node) noexcept
	{
		NodePtr<Tkey, Tvalue> son = node->left.Get();
		NodePtr<Tkey, Tvalue> grandSon = son->right.Get();

		if (grandSon == nullptr || son == nullptr)
			return GetRoot();

		grandSon->parent = node->parent;
		son->parent = grandSon;
		node->parent = grandSon;

		node->left = grandSon->right;
		son->right = grandSon->left;

		grandSon->left = son;
		grandSon->right = node;

		return grandSon;
	}

	int BalanceFactor() const noexcept override
	{
		return BalanceFactor(root.Get());
	}

	int BalanceFactor(NodePtr<Tkey, Tvalue> startNode) const noexcept override
	{
		if (startNode == nullptr)
			return 0;

		return Depth(startNode->left.Get()) - Depth(startNode->right.Get());
	}

	void Balance() noexcept override
	{
		if (root == nullptr)
			return;

		if (BalanceFactor(root->left.Get()) >= 2)
		{
			Traverse(root->left.Get(), Left, Right, Root,
				[this](NodePtr<Tkey, Tvalue> nodePtr) -> void { this->Balance(nodePtr); });
		}

		if (BalanceFactor(root->right.Get()) <= -2)
		{
			Traverse(root->right.Get(), Right, Left, Root,
				[this](NodePtr<Tkey, Tvalue> nodePtr) -> void { this->Balance(nodePtr); });
		}

		if (BalanceFactor(root.Get()) == 2 && BalanceFactor(root->left.Get()) == 1)
			this->LeftLeftRotation();
		else if (BalanceFactor(root.Get()) == -2 && BalanceFactor(root->right.Get()) == -1)
			this->RightRightRotation();
		else if (BalanceFactor(root.Get()) == -2 && BalanceFactor(root->right.Get()) == 1)
			this->RightLeftRotation();
		else if (BalanceFactor(root.Get()) == 2 && BalanceFactor(root->left.Get()) == -1)
			this->LeftRightRotation();
	}

	void Balance(NodePtr<Tkey, Tvalue> startNode) noexcept
	{
		if (startNode == nullptr)
			return;

		if (BalanceFactor(startNode) == 2 && BalanceFactor(startNode->right.Get()) == 0)
		{
			NodePtr<Tkey, Tvalue> parent = startNode->parent;
			startNode = this->LeftLeftRotation(startNode);
			parent->left = startNode;
		}
		else if (BalanceFactor(startNode) == -2 && BalanceFactor(startNode->left.Get()) == 0)
		{
			NodePtr<Tkey, Tvalue> parent = startNode->parent;
			startNode = this->RightRightRotation(startNode);
			parent->right = startNode;
		}

		if (BalanceFactor(startNode) == 2 && BalanceFactor(startNode->left.Get()) == 1)
			this->LeftLeftRotation();
		else if (BalanceFactor(startNode) == -2 && BalanceFactor(startNode->right.Get()) == -1)
			this->RightRightRotation();
		else if (BalanceFactor(startNode) == -2 && BalanceFactor(startNode->right.Get()) == 1)
			this->RightLeftRotation();
		else if (BalanceFactor(startNode) == 2 && BalanceFactor(startNode->left.Get()) == -1)
			this->LeftRightRotation();
	}
};
