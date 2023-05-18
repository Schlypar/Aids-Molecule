#pragma once

#include "BinaryTree.h"
#include "Tree/ITree.h"

template <Comparible T>
class MultiSearchTree : public BinaryTree<T, Multi<T>>
{
	template <typename T1, typename T2>
	using TreeNode = typename Tree<T1, T2>::TreeNode;

	// template <typename T1, typename T2>
	// using TreeNode = typename BinaryTree<T1, T2>::template Tree<T1, T2>::TreeNode;

	template <typename T1, typename T2>
	using NodePtr = TreeNode<T1, T2>*;

public:
	MultiSearchTree()
	    : BinaryTree<T, Multi<T>>()
	{
	}

	MultiSearchTree(const T& startValue)
	    : BinaryTree<T, Multi<T>>(Stack<T>(startValue))
	{
	}

	// MultiSearchTree(const T& startValue, T (*kGenMulti)(const Stack<T>&))
	//     : BinaryTree<T, Multi<T>>(Stack<T>(startValue), kGenMulti)
	// {
	// }

	MultiSearchTree(const T& startValue, KGen<T, Multi<T>> kGenMulti)
	    : BinaryTree<T, Multi<T>>(Stack<T>(startValue), kGenMulti)
	{
	}

	MultiSearchTree(TreeNode<T, Multi<T>>* startRoot)
	    : BinaryTree<T, Multi<T>>(startRoot)
	{
	}

	MultiSearchTree(const BinaryTree<T, Multi<T>>& other)
	    : BinaryTree<T, Multi<T>>(other)
	{
	}

	MultiSearchTree(const MultiSearchTree<T>& other)
	    : BinaryTree<T, Multi<T>>((BinaryTree<T, Multi<T>>*) (&other))
	{
	}

	MultiSearchTree(const MultiSearchTree<T>* other)
	    : BinaryTree<T, Multi<T>>((BinaryTree<T, Multi<T>>*) other)
	{
	}

	MultiSearchTree(MultiSearchTree<T>* other)
	    : BinaryTree<T, Multi<T>>((BinaryTree<T, Multi<T>>*) other)
	{
	}

	MultiSearchTree<T>* Add(const T& val) noexcept
	{
		Multi<T> value = val;
		if (this->root == nullptr)
		{
			this->root = NodePtr<T, Multi<T>>(new TreeNode<T, Multi<T>>(value));
			this->root->key = this->kGen(value);
			return this;
		}

		T valueKey = this->kGen(value);

		NodePtr<T, Multi<T>> current = this->root.Get();

		while (NOT_DONE)
		{
			if (current->key == valueKey)
			{
				current->data.Push(val);
				return this;
			}

			if (current->left == nullptr && valueKey < current->key)
			{
				current->left = new TreeNode<T, Multi<T>>(value);
				current->left->key = this->kGen(value);
				current->left->parent = current;

				this->Balance();
				return this;
			}

			if (current->right == nullptr && valueKey >= current->key)
			{

				current->right = new TreeNode<T, Multi<T>>(value);
				current->right->key = this->kGen(value);
				current->right->parent = current;

				this->Balance();
				return this;
			}

			if (valueKey < current->key)
				current = current->left.Get();
			else if (valueKey > current->key)
				current = current->right.Get();
		}

		this->Balance();
		return this;
	}

	NodePtr<T, Multi<T>> Find(const T& val) const noexcept
	{
		Multi<T> value = Stack<T>(val);

		if (this->root == nullptr)
			return nullptr;

		T keyValue = this->kGen(value);

		NodePtr<T, Multi<T>> current = this->root.Get();

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

	void Delete(const Multi<T>& value) noexcept override
	{
		if (this->root == nullptr)
			return;

		T keyValue = this->kGen(value);

		if (keyValue == this->root->key)
		{
			this->DeleteRoot();
			this->Balance();
			return;
		}

		NodePtr<T, Multi<T>> current = this->GetRoot();

		while (NOT_DONE)
		{
			if (keyValue < current->key)
				current = current->left.Get();
			else if (keyValue > current->key)
				current = current->right.Get();
			else if (this->Depth(current->left.Get()) < this->Depth(current->right.Get()))
			{
				if (MostLeft(current->right.Get()) != nullptr)
				{
					UniquePtr<TreeNode<T, Multi<T>>> nodeToBeDeleted = MostLeft(current->right.Get());

					nodeToBeDeleted->parent->left = nodeToBeDeleted->right;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					this->Balance();
					return;
				}
				else if (MostRight(current->left.Get()) != nullptr)
				{
					UniquePtr<TreeNode<T, Multi<T>>> nodeToBeDeleted = MostRight(current->left.Get());

					nodeToBeDeleted->parent->right = nodeToBeDeleted->left;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					this->Balance();
					return;
				}

				current->parent = nullptr;

				NodePtr<T, Multi<T>> parent = current->parent;
				(parent->left.Get() == current) ? parent->left = nullptr : parent->right = nullptr;
				current->parent = nullptr;

				this->Balance();
				return;
			}
			else
			{
				if (MostRight(current->left.Get()) != nullptr)
				{
					UniquePtr<TreeNode<T, Multi<T>>> nodeToBeDeleted = MostRight(current->left.Get());

					nodeToBeDeleted->parent->right = nodeToBeDeleted->left;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					this->Balance();
					return;
				}
				else if (MostLeft(current->right.Get()) != nullptr)
				{
					UniquePtr<TreeNode<T, Multi<T>>> nodeToBeDeleted = MostLeft(current->right.Get());

					nodeToBeDeleted->parent->left = nodeToBeDeleted->right;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					this->Balance();
					return;
				}

				NodePtr<T, Multi<T>> parent = current->parent;
				(parent->left.Get() == current) ? parent->left = nullptr : parent->right = nullptr;
				current->parent = nullptr;

				delete current;
				this->Balance();
				return;
			}
		}
	}

	// will erase only if node->data length is more than 1
	void Erase(const T& value) noexcept
	{
		Multi<T> val = Stack<T>(value);
		T key = this->kGen(val);

		auto erase = [this, &key, &value](NodePtr<T, Multi<T>> node) -> void {
			if (node->key == key)
			{
				if (node->data.Count() > 1)
					node->data.Pop();
			}
		};

		this->Traverse(Left, Root, Right, erase);
	}

	friend std::ostream& operator<<(std::ostream& stream, SharedPtr<MultiSearchTree<T>> tree)
	{
		stream << (BinaryTree<T, T>*) tree.Get();

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const MultiSearchTree<T>& tree)
	{
		stream << ((BinaryTree<T, T>*) (&tree));

		return stream;
	}
};