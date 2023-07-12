#pragma once

#include "BinaryTree.h"
#include "Sequence/List.h"
#include "Tree/ITree.h"

template <Comparible P, typename T>
class BinaryHeap : public BinaryTree<P, T>
{
	template <typename T1, typename T2>
	using TreeNode = typename Tree<T1, T2>::TreeNode;

	template <typename T1, typename T2>
	using NodePtr = TreeNode<T1, T2>*;

public:
	BinaryHeap()
	    : BinaryTree<P, T>()
	{
	}

	BinaryHeap(const T& startValue)
	    : BinaryTree<P, T>(startValue)
	{
	}

	BinaryHeap(const T& startValue, KGen<P, T> kGen)
	    : BinaryTree<P, T>(startValue, kGen)
	{
	}

	BinaryHeap(TreeNode<P, T>* startRoot, KGen<P, T> kGen)
	    : BinaryTree<P, T>(startRoot, kGen)
	{
	}

	BinaryHeap(const BinaryTree<P, T>& other)
	    : BinaryTree<P, T>(other)
	{
	}

	BinaryHeap(const BinaryHeap<P, T>& other)
	    : BinaryTree<P, T>((BinaryTree<P, T>*) (&other))
	{
	}

	BinaryHeap(const BinaryHeap<P, T>* other)
	    : BinaryTree<P, T>((BinaryTree<P, T>*) other)
	{
	}

	friend std::ostream& operator<<(std::ostream& stream, SharedPtr<BinaryHeap<P, T>> tree)
	{
		stream << (BinaryTree<P, T>*) tree.Get();

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const BinaryHeap<P, T>& tree)
	{
		stream << ((BinaryTree<P, T>*) (&tree));

		return stream;
	}

	Tree<P, T>* Add(const T& value) noexcept override;

	void Delete(const T& value) noexcept override;

	Tree<P, T>* Create() const noexcept override
	{
		return (Tree<P, T>*) new BinaryHeap<P, T>(this->kGen);
	}

	Tree<P, T>* Create(TreeNode<P, T>* root) const noexcept override
	{
		return (Tree<P, T>*) new BinaryHeap<P, T>(root, this->kGen);
	}

	Tree<P, T>* Copy() const noexcept override
	{
		return (Tree<P, T>*) new BinaryHeap<P, T>(*this);
	}

private:
	// swaps values with parent of the 'node'
	void PushUp(NodePtr<P, T> node) noexcept;

	// swaps values with 'bigger' child node
	void PushDown(NodePtr<P, T> node) noexcept;

	void Balance() noexcept override;
};

template <Comparible P, typename T>
Tree<P, T>* BinaryHeap<P, T>::Add(const T& value) noexcept
{
	if (this->root == nullptr)
	{
		this->root = NodePtr<P, T>(new TreeNode<P, T>(value));
		return this;
	}

	P valueKey = this->kGen(value);

	NodePtr<P, T> current = this->root.Get();

	while (NOT_DONE)
	{
		if (current->left == nullptr)
		{
			current->left = new TreeNode<P, T>(value);
			current->left->key = valueKey;
			current->left->parent = current;

			this->Balance();
			return this;
		}
		if (current->right == nullptr)
		{
			current->right = new TreeNode<P, T>(value);
			current->right->key = valueKey;
			current->right->parent = current;

			this->Balance();
			return this;
		}

		if (this->Depth(current->left.Get()) < this->Depth(current->right.Get()))
			current = current->left.Get();
		else
			current = current->right.Get();
	}

	this->Balance();
	return this;
}

template <Comparible P, typename T>
void BinaryHeap<P, T>::Delete(const T& value) noexcept
{
	auto deleter = [this](NodePtr<P, T> node) -> void {
		while (node->left != nullptr || node->right != nullptr)
		{
			NodePtr<P, T> withMaxValue = nullptr;
			if (node->left != nullptr && node->right != nullptr)
				withMaxValue = (node->left->key > node->right->key) ? node->left.Get() : node->right.Get();
			else if (node->left != nullptr)
				withMaxValue = node->left.Get();
			else
				withMaxValue = node->right.Get();

			this->PushDown(node);
			node = withMaxValue;
		}

		NodePtr<P, T> parent = node->parent;

		if (parent->left.Get() == node)
			parent->left = nullptr;
		else
			parent->right = nullptr;

		node->parent = nullptr;
		delete node;
	};

	this->Traverse(Left, Right, Root, [deleter, value](NodePtr<P, T> node) -> void {
		if (node->data == value)
			deleter(node);
	});
}

template <Comparible P, typename T>
void BinaryHeap<P, T>::PushUp(NodePtr<P, T> node) noexcept
{
	std::swap(node->data, node->parent->data);
	std::swap(node->key, node->parent->key);
}

template <Comparible P, typename T>
void BinaryHeap<P, T>::PushDown(NodePtr<P, T> node) noexcept
{
	NodePtr<P, T> withMaxValue = nullptr;
	if (node->left != nullptr && node->right != nullptr)
		withMaxValue = (node->left->key > node->right->key) ? node->left.Get() : node->right.Get();
	else if (node->left != nullptr)
		withMaxValue = node->left.Get();
	else
		withMaxValue = node->right.Get();

	std::swap(node->data, withMaxValue->data);
	std::swap(node->key, withMaxValue->key);
}

template <Comparible P, typename T>
void BinaryHeap<P, T>::Balance() noexcept
{
	auto pushUp = [this](NodePtr<P, T> node) -> void {
		while (node->parent != nullptr && node->parent->key < node->key)
		{
			this->PushUp(node);
			node = node->parent;
		}
	};

	auto pushDown = [this](NodePtr<P, T> node) -> void {
		while ((node->left != nullptr && node->key < node->left->key)
			|| (node->right != nullptr && node->key < node->right->key))
		{
			NodePtr<P, T> withMaxValue = nullptr;
			if (node->left != nullptr && node->right != nullptr)
				withMaxValue = (node->left->key > node->right->key) ? node->left.Get() : node->right.Get();
			else if (node->left != nullptr)
				withMaxValue = node->left.Get();
			else
				withMaxValue = node->right.Get();

			this->PushDown(node);
			node = withMaxValue;
		}
	};

	this->Traverse(Left, Right, Root, [pushUp, pushDown](NodePtr<P, T> node) -> void {
		pushUp(node);
		pushDown(node);
	});
}
