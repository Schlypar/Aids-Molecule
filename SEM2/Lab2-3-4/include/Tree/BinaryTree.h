#pragma once

#include "ITree.h"
#include "Logger.h"
#include "Pointer.h"

#define NOT_DONE true

template <typename Tkey, typename Tvalue>
class BinaryTree : Tree<Tkey, Tvalue>
{
	template <typename T1, typename T2>
	using TreeNode = typename Tree<T1, T2>::TreeNode;

	template <typename T1, typename T2>
	using NodePtr = SharedPtr<TreeNode<T1, T2>>;

	void CopyNodes(NodePtr<Tkey, Tvalue>& copyNode, const NodePtr<Tkey, Tvalue>& originalNode) const noexcept override
	{
		if (originalNode->left)
		{
			copyNode->left = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->left->data,
				originalNode->left->kGen));
			CopyNodes(copyNode->left, originalNode->left);
		}
		if (originalNode->right)
		{
			copyNode->right = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->right->data,
				originalNode->right->kGen));
			CopyNodes(copyNode->right, originalNode->right);
		}
	}

	void CopyNodes(NodePtr<Tkey, Tvalue>& copyNode, const NodePtr<Tkey, Tvalue>& originalNode,
		bool (*filter)(Tvalue& value)) const noexcept override
	{
		if (originalNode->left)
		{
			if (filter(originalNode->left->data))
			{
				copyNode->left = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->left->data,
					originalNode->left->kGen));
				CopyNodes(copyNode->left, originalNode->left, filter);
			}
			else
				CopyNodes(copyNode, originalNode->left, filter);
		}
		if (originalNode->right)
		{
			if (filter(originalNode->right->data))
			{
				copyNode->right = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->right->data,
					originalNode->right->kGen));
				CopyNodes(copyNode->right, originalNode->right, filter);
			}
			else
				CopyNodes(copyNode, originalNode->right, filter);
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
				printTree(stream, startNode->left, first, second, third);
				break;
			case Root:
				stream << startNode->data << " ";
				break;
			case Right:
				printTree(stream, startNode->right, first, second, third);
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				printTree(stream, startNode->left, first, second, third);
				break;
			case Root:
				stream << startNode->data << " ";
				break;
			case Right:
				printTree(stream, startNode->right, first, second, third);
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				printTree(stream, startNode->left, first, second, third);
				break;
			case Root:
				stream << startNode->data << " ";
				break;
			case Right:
				printTree(stream, startNode->right, first, second, third);
				break;
			default:
				break;
		}
	}

	std::ostream& Dump(std::ostream& stream, const NodePtr<Tkey, Tvalue>& startNode, TraverseOrder first,
		TraverseOrder second, TraverseOrder third) const noexcept override
	{
		switch (first)
		{
			case Left:
				stream << "{ ";
				printTree(stream, GetRoot()->left, first, second, third);
				stream << "}";
				break;
			case Root:
				stream << "(";
				stream << GetRoot()->data;
				stream << ")";
				break;
			case Right:
				stream << "[ ";
				printTree(stream, GetRoot()->right, first, second, third);
				stream << "]";
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				stream << "{ ";
				printTree(stream, GetRoot()->left, first, second, third);
				stream << "}";
				break;
			case Root:
				stream << "(";
				stream << GetRoot()->data;
				stream << ")";
				break;
			case Right:
				stream << "[ ";
				printTree(stream, GetRoot()->right, first, second, third);
				stream << "]";
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				stream << "{ ";
				printTree(stream, GetRoot()->left, first, second, third);
				stream << "}";
				break;
			case Root:
				stream << "(";
				stream << GetRoot()->data;
				stream << ")";
				break;
			case Right:
				stream << "[ ";
				printTree(stream, GetRoot()->right, first, second, third);
				stream << "]";
				break;
			default:
				break;
		}

		return stream;
	}

private:
	NodePtr<Tkey, Tvalue> root;

public:
	BinaryTree()
	    : root(nullptr)
	{
		Logger::Info("Default constructor of BinaryTree<T>");
	}

	BinaryTree(const Tvalue& startValue)
	    : root(new TreeNode<Tkey, Tvalue>(startValue))
	{
		Logger::Info("Starting value constructor of BinaryTree<T>");
	}

	BinaryTree(const Tvalue& startValue, KGen<Tkey, Tvalue> kGen)
	    : root(new TreeNode<Tkey, Tvalue>(startValue, kGen))
	{
		Logger::Info("Starting value constructor of BinaryTree<T>");
	}

	BinaryTree(TreeNode<Tkey, Tvalue>* startRoot)
	    : root(startRoot)
	{
		Logger::Info("Starting root node constructor of BinaryTree<T>");
	}

	BinaryTree(const BinaryTree<Tkey, Tvalue>& other)
	    : root(nullptr)
	{
		if (other.root != nullptr)
		{
			root = new TreeNode<Tkey, Tvalue>(other.root->data, other.root->kGen);
			CopyNodes(root, other.root);
		}
	}

	virtual ~BinaryTree()
	{
	}

	Size Depth(NodePtr<Tkey, Tvalue> startNode) const noexcept override
	{
		Size depth = 0;
		if (startNode->left)
			depth = std::max(depth, Depth(startNode->left, depth));

		if (startNode->right)
			depth = std::max(depth, Depth(startNode->right, depth));

		return depth;
	}

	Size Depth(NodePtr<Tkey, Tvalue> startNode, Size depth) const noexcept override
	{
		depth++;
		Size leftDepth = 0;
		Size rightDepth = 0;

		if (startNode->left)
			leftDepth = std::max(depth, Depth(startNode->left, leftDepth));

		if (startNode->right)
			rightDepth = std::max(depth, Depth(startNode->right, rightDepth));

		return depth + std::max(leftDepth, rightDepth);
	}

	Size Depth() const noexcept override
	{
		auto startNode = this->root;

		Size depth = 1;
		Size leftDepth = 0;
		Size rightDepth = 0;

		if (startNode->left)
			leftDepth = std::max(depth, Depth(startNode->left, leftDepth));

		if (startNode->right)
			rightDepth = std::max(depth, Depth(startNode->right, rightDepth));

		return depth + std::max(leftDepth, rightDepth);
	}

	Tree<Tkey, Tvalue>* Add(const Tvalue& value) noexcept override
	{
		if (root == nullptr)
		{
			root = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(value));
			return this;
		}

		NodePtr<Tkey, Tvalue> current = root;

		while (NOT_DONE)
		{
			if (current->left == nullptr)
			{
				current->left = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(value, current->kGen));
				current->left->parent = current;
				return this;
			}

			if (current->right == nullptr)
			{
				current->right = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(value, current->kGen));
				current->right->parent = current;
				return this;
			}

			if (current->left->key < current->right->key)
				current = current->left;
			else
				current = current->right;
		}

		return this;
	}

	Tree<Tkey, Tvalue>* Create() const noexcept override
	{
		return (Tree<Tkey, Tvalue>*) new BinaryTree<Tkey, Tvalue>();
	}

	Tree<Tkey, Tvalue>* Create(TreeNode<Tkey, Tvalue>* root) const noexcept override
	{
		return (Tree<Tkey, Tvalue>*) new BinaryTree<Tkey, Tvalue>(root);
	}

	Tree<Tkey, Tvalue>* Copy() const noexcept override
	{
		return (Tree<Tkey, Tvalue>*) new BinaryTree<Tkey, Tvalue>(*this);
	}

	NodePtr<Tkey, Tvalue> GetRoot() const noexcept override
	{
		return root;
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
				Traverse(startNode->left, first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right, first, second, third, func);
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				Traverse(startNode->left, first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right, first, second, third, func);
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				Traverse(startNode->left, first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right, first, second, third, func);
				break;
			default:
				break;
		}
	}

	Tree<Tkey, Tvalue>*
	Traverse(TraverseOrder first, TraverseOrder second, TraverseOrder third, std::function<void(Tvalue&)> func) const override
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

		NodePtr<Tkey, Tvalue> startNode = this->root;

		if (startNode == nullptr)
			return;

		switch (first)
		{
			case Left:
				Traverse(startNode->left, first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right, first, second, third, func);
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				Traverse(startNode->left, first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right, first, second, third, func);
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				Traverse(startNode->left, first, second, third, func);
				break;
			case Root:
				func(startNode->data);
				break;
			case Right:
				Traverse(startNode->right, first, second, third, func);
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
		tree.Dump(stream, tree.GetRoot(), Left, Root, Right);

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const SharedPtr<BinaryTree<Tkey, Tvalue>>& tree)
	{
		tree->Dump(stream, tree->GetRoot(), Left, Root, Right);

		return stream;
	}
};