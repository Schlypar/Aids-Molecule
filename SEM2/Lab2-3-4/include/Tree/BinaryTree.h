#pragma once

#include "ITree.h"
#include "Logger.h"
#include "Pointer.h"

#define NOT_DONE true

template <typename Tkey, typename Tvalue>
class BinaryTree : public Tree<Tkey, Tvalue>
{
	template <typename T1, typename T2>
	using TreeNode = typename Tree<T1, T2>::TreeNode;

	template <typename T1, typename T2>
	using NodePtr = TreeNode<T1, T2>*;

private:
	WeakPtr<TreeNode<Tkey, Tvalue>> root;

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
			CopyNodes(root.Get(), other.root.Get());
		}
	}

	virtual ~BinaryTree()
	{
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

		Size depth = 1;
		Size leftDepth = 0;
		Size rightDepth = 0;

		if (startNode->left)
			leftDepth = std::max(depth, Depth(startNode->left.Get(), leftDepth));

		if (startNode->right)
			rightDepth = std::max(depth, Depth(startNode->right.Get(), rightDepth));

		return depth + std::max(leftDepth, rightDepth);
	}

	Tree<Tkey, Tvalue>* Add(const Tvalue& value) noexcept override
	{
		if (root == nullptr)
		{
			root = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(value));
			return this;
		}

		Tkey valueKey = root->kGen(value);

		NodePtr<Tkey, Tvalue> current = this->root.Get();

		while (NOT_DONE)
		{
			if (current->left == nullptr && valueKey < current->data)
			{
				current->left = new TreeNode<Tkey, Tvalue>(value, current->kGen);
				current->left->parent = current;
				auto curDepth = Depth();

				auto rootBF = this->BalanceFactor(this->GetRoot());
				auto leftBF = this->BalanceFactor(this->GetRoot()->left.Get());
				auto rightBF = this->BalanceFactor(this->GetRoot()->right.Get());

				print("VALUE: ", value, "\nDEPTH: ", curDepth, "\nROOT: ", rootBF, "\nLEFT: ", leftBF,
					"\nRIGHT: ", rightBF, "\n========================\n");
				Balance();
				return this;
			}

			if (current->right == nullptr && valueKey >= current->data)
			{
				current->right = new TreeNode<Tkey, Tvalue>(value, current->kGen);
				current->right->parent = current;
				auto curDepth = Depth();

				auto rootBF = this->BalanceFactor(this->GetRoot());
				auto leftBF = this->BalanceFactor(this->GetRoot()->left.Get());
				auto rightBF = this->BalanceFactor(this->GetRoot()->right.Get());

				print("VALUE: ", value, "\nDEPTH: ", curDepth, "\nROOT: ", rootBF, "\nLEFT: ", leftBF,
					"\nRIGHT: ", rightBF, "\n========================\n");
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
		return root.Get();
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
		tree.Dump(stream, tree.GetRoot(), Left, Root, Right);

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const SharedPtr<BinaryTree<Tkey, Tvalue>>& tree)
	{
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

private:
	void CopyNodes(NodePtr<Tkey, Tvalue> copyNode, const NodePtr<Tkey, Tvalue> originalNode) const noexcept override
	{
		if (originalNode->left)
		{
			copyNode->left = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->left->data,
				originalNode->left->kGen));

			copyNode->left->parent = copyNode;
			CopyNodes(copyNode->left.Get(), originalNode->left.Get());
		}
		if (originalNode->right)
		{
			copyNode->right = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->right->data,
				originalNode->right->kGen));

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
				copyNode->left = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->left->data,
					originalNode->left->kGen));

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
				copyNode->right = NodePtr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->right->data,
					originalNode->right->kGen));

				copyNode->right->parent = copyNode;
				CopyNodes(copyNode->right.Get(), originalNode->right.Get(), filter);
			}
			else
				CopyNodes(copyNode, originalNode->right.Get(), filter);
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
				printTree(stream, startNode->left.Get(), first, second, third);
				break;
			case Root:
				stream << startNode->data << " ";
				break;
			case Right:
				printTree(stream, startNode->right.Get(), first, second, third);
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				printTree(stream, startNode->left.Get(), first, second, third);
				break;
			case Root:
				stream << startNode->data << " ";
				break;
			case Right:
				printTree(stream, startNode->right.Get(), first, second, third);
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				printTree(stream, startNode->left.Get(), first, second, third);
				break;
			case Root:
				stream << startNode->data << " ";
				break;
			case Right:
				printTree(stream, startNode->right.Get(), first, second, third);
				break;
			default:
				break;
		}
	}

	std::ostream& Dump(std::ostream& stream, const NodePtr<Tkey, Tvalue> startNode, TraverseOrder first, TraverseOrder second,
		TraverseOrder third) const noexcept override
	{
		switch (first)
		{
			case Left:
				stream << "{ ";
				printTree(stream, GetRoot()->left.Get(), first, second, third);
				stream << "}";
				break;
			case Root:
				stream << "(";
				stream << GetRoot()->data;
				stream << ")";
				break;
			case Right:
				stream << "[ ";
				printTree(stream, GetRoot()->right.Get(), first, second, third);
				stream << "]";
				break;
			default:
				break;
		}

		switch (second)
		{
			case Left:
				stream << "{ ";
				printTree(stream, GetRoot()->left.Get(), first, second, third);
				stream << "}";
				break;
			case Root:
				stream << "(";
				stream << GetRoot()->data;
				stream << ")";
				break;
			case Right:
				stream << "[ ";
				printTree(stream, GetRoot()->right.Get(), first, second, third);
				stream << "]";
				break;
			default:
				break;
		}

		switch (third)
		{
			case Left:
				stream << "{ ";
				printTree(stream, GetRoot()->left.Get(), first, second, third);
				stream << "}";
				break;
			case Root:
				stream << "(";
				stream << GetRoot()->data;
				stream << ")";
				break;
			case Right:
				stream << "[ ";
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
			Traverse(
				root->left.Get(), Left, Right, Root,
				[this](NodePtr<Tkey, Tvalue> nodePtr) -> void { this->Balance(nodePtr); });
		}

		if (BalanceFactor(root->right.Get()) <= -2)
		{
			Traverse(
				root->right.Get(), Right, Left, Root,
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