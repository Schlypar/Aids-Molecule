#pragma once

#include "BinaryTree.h"
#include "Sequence/List.h"
#include "Tree/ITree.h"

template <Comparible P, typename T>
class BinaryHeap : protected BinaryTree<P, T>
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

	BinaryHeap(TreeNode<P, T>* startRoot)
	    : BinaryTree<P, T>(startRoot)
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

	Tree<P, T>* Add(const T& value) noexcept override
	{
		if (this->root == nullptr)
		{
			this->root = NodePtr<P, T>(new TreeNode<T, P>(value));
			return this;
		}

		P valueKey = this->kGen(value);

		NodePtr<T, P> current = this->root.Get();

		while (NOT_DONE)
		{
			if (current->left == nullptr)
			{
				current->left = new TreeNode<T, P>(value);
				current->left->key = valueKey;
				current->left->parent = current;

				this->Balance();
				return this;
			}
			if (current->right == nullptr)
			{
				current->right = new TreeNode<T, P>(value);
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

	void Delete(const T& value) noexcept override
	{
		auto deleter = [this](NodePtr<T, P> node) -> void {
			while (node->left != nullptr || node->right != nullptr)
			{
				NodePtr<T, P> withMaxValue = nullptr;
				if (node->left != nullptr && node->right != nullptr)
					withMaxValue = (node->left->key > node->right->key) ? node->left.Get()
											    : node->right.Get();
				else if (node->left != nullptr)
					withMaxValue = node->left.Get();
				else
					withMaxValue = node->right.Get();

				this->PushDown(node);
				node = withMaxValue;
			}

			NodePtr<T, P> parent = node->parent;

			if (parent->left.Get() == node)
				parent->left = nullptr;
			else
				parent->right = nullptr;

			node->parent = nullptr;
			delete node;
		};

		this->Traverse(Left, Right, Root, [deleter, value](NodePtr<T, P> node) -> void {
			if (node->data == value)
				deleter(node);
		});
	}

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
	void PushUp(NodePtr<T, P> node) noexcept
	{
		std::swap(node->data, node->parent->data);
		std::swap(node->key, node->parent->key);
	}

	void PushDown(NodePtr<T, P> node) noexcept
	{
		NodePtr<T, P> withMaxValue = nullptr;
		if (node->left != nullptr && node->right != nullptr)
			withMaxValue = (node->left->key > node->right->key) ? node->left.Get() : node->right.Get();
		else if (node->left != nullptr)
			withMaxValue = node->left.Get();
		else
			withMaxValue = node->right.Get();

		std::swap(node->data, withMaxValue->data);
		std::swap(node->key, withMaxValue->key);
	}

	void Balance() noexcept override
	{
		auto pushUp = [this](NodePtr<T, P> node) -> void {
			while (node->parent != nullptr && node->parent->key < node->key)
			{
				this->PushUp(node);
				node = node->parent;
			}
		};

		auto pushDown = [this](NodePtr<T, P> node) -> void {
			while ((node->left != nullptr && node->key < node->left->key)
				|| (node->right != nullptr && node->key < node->right->key))
			{
				NodePtr<T, P> withMaxValue = nullptr;
				if (node->left != nullptr && node->right != nullptr)
					withMaxValue = (node->left->key > node->right->key) ? node->left.Get()
											    : node->right.Get();
				else if (node->left != nullptr)
					withMaxValue = node->left.Get();
				else
					withMaxValue = node->right.Get();

				this->PushDown(node);
				node = withMaxValue;
			}
		};

		this->Traverse(Left, Right, Root, [pushUp, pushDown](NodePtr<T, P> node) -> void {
			pushUp(node);
			pushDown(node);
		});
	}
};