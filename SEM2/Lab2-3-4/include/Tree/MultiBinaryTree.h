#pragma once

#include "BinaryTree.h"

/*

tempalte <typename K, C<T>>\

C<T> -> single<T> or multi<Tvalue>

*/

template <Comparible Tkey, typename Tvalue>
class MultiBinaryTree : public BinaryTree<Tkey, Multi<Tvalue>>
{
	template <typename T1, typename T2>
	using TreeNode = typename Tree<T1, T2>::TreeNode;

	template <typename T1, typename T2>
	using NodePtr = TreeNode<T1, T2>*;

public:
	MultiBinaryTree()
	    : BinaryTree<Tkey, Multi<Tvalue>>()
	{
	}

	MultiBinaryTree(const Tvalue& startValue)
	    : BinaryTree<Tkey, Multi<Tvalue>>(Stack<Tvalue>(startValue))
	{
	}

	MultiBinaryTree(const Tvalue& startValue, KGen<Tkey, Multi<Tvalue>> kGenMulti)
	    : BinaryTree<Tkey, Multi<Tvalue>>(Stack<Tvalue>(startValue), kGenMulti)
	{
	}

	MultiBinaryTree(TreeNode<Tkey, Multi<Tvalue>>* startRoot)
	    : BinaryTree<Tkey, Multi<Tvalue>>(startRoot)
	{
	}

	// MultiBinaryTree(const BinaryTree<Tkey, Multi<Tvalue>>& other)
	//     : BinaryTree<Tkey, Multi<Tvalue>>(other)
	// {
	// }

	MultiBinaryTree(const MultiBinaryTree<Tkey, Tvalue>& other)
	    : BinaryTree<Tkey, Multi<Tvalue>>((BinaryTree<Tkey, Multi<Tvalue>>*) (&other))
	{
	}

	MultiBinaryTree(const MultiBinaryTree<Tkey, Tvalue>* other)
	    : BinaryTree<Tkey, Multi<Tvalue>>((BinaryTree<Tkey, Multi<Tvalue>>*) other)
	{
	}

	MultiBinaryTree(MultiBinaryTree<Tkey, Tvalue>* other)
	    : BinaryTree<Tkey, Multi<Tvalue>>((BinaryTree<Tkey, Multi<Tvalue>>*) other)
	{
	}

	MultiBinaryTree<Tkey, Tvalue>* Add(const Tvalue& val) noexcept
	{
		Multi<Tvalue> value = val;
		if (this->root == nullptr)
		{
			this->root = NodePtr<Tkey, Multi<Tvalue>>(new TreeNode<Tkey, Multi<Tvalue>>(value));
			this->root->key = this->kGen(value);
			return this;
		}

		Tkey valueKey = this->kGen(value);

		NodePtr<Tkey, Multi<Tvalue>> current = this->root.Get();

		while (NOT_DONE)
		{
			if (current->key == valueKey)
			{
				current->data.Push(val);
				return this;
			}

			if (current->left == nullptr && valueKey < current->key)
			{
				current->left = new TreeNode<Tkey, Multi<Tvalue>>(value);
				current->left->key = this->kGen(value);
				current->left->parent = current;

				this->Balance();
				return this;
			}

			if (current->right == nullptr && valueKey >= current->key)
			{

				current->right = new TreeNode<Tkey, Multi<Tvalue>>(value);
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

	NodePtr<Tkey, Multi<Tvalue>> Find(const Tvalue& val) const noexcept
	{
		Multi<Tvalue> value = Stack<Tvalue>(val);

		if (this->root == nullptr)
			return nullptr;

		Tkey keyValue = this->kGen(value);

		NodePtr<Tkey, Multi<Tvalue>> current = this->root.Get();

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

	void Delete(const Multi<Tvalue>& value) noexcept override
	{
		if (this->root == nullptr)
			return;

		Tkey keyValue = this->kGen(value);

		if (keyValue == this->root->key)
		{
			this->DeleteRoot();
			this->Balance();
			return;
		}

		NodePtr<Tkey, Multi<Tvalue>> current = this->GetRoot();

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
					UniquePtr<TreeNode<Tkey, Multi<Tvalue>>> nodeToBeDeleted = MostLeft(current->right.Get());

					nodeToBeDeleted->parent->left = nodeToBeDeleted->right;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					this->Balance();
					return;
				}
				else if (MostRight(current->left.Get()) != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Multi<Tvalue>>> nodeToBeDeleted = MostRight(current->left.Get());

					nodeToBeDeleted->parent->right = nodeToBeDeleted->left;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					this->Balance();
					return;
				}

				current->parent = nullptr;

				NodePtr<Tkey, Multi<Tvalue>> parent = current->parent;
				(parent->left.Get() == current) ? parent->left = nullptr : parent->right = nullptr;
				current->parent = nullptr;

				this->Balance();
				return;
			}
			else
			{
				if (MostRight(current->left.Get()) != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Multi<Tvalue>>> nodeToBeDeleted = MostRight(current->left.Get());

					nodeToBeDeleted->parent->right = nodeToBeDeleted->left;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					this->Balance();
					return;
				}
				else if (MostLeft(current->right.Get()) != nullptr)
				{
					UniquePtr<TreeNode<Tkey, Multi<Tvalue>>> nodeToBeDeleted = MostLeft(current->right.Get());

					nodeToBeDeleted->parent->left = nodeToBeDeleted->right;

					current->data = std::move(nodeToBeDeleted->data);
					current->key = std::move(nodeToBeDeleted->key);

					this->Balance();
					return;
				}

				NodePtr<Tkey, Multi<Tvalue>> parent = current->parent;
				(parent->left.Get() == current) ? parent->left = nullptr : parent->right = nullptr;
				current->parent = nullptr;

				delete current;
				this->Balance();
				return;
			}
		}
	}

	// will erase only if node->data length is more than 1
	void Erase(const Tvalue& value) noexcept
	{
		Multi<Tvalue> val = Stack<Tvalue>(value);
		Tkey key = this->kGen(val);

		auto erase = [this, &key, &value](NodePtr<Tkey, Multi<Tvalue>> node) -> void {
			if (node->key == key)
			{
				if (node->data.Count() > 1)
					node->data.Pop();
			}
		};

		this->Traverse(Left, Root, Right, erase);
	}

	friend std::ostream& operator<<(std::ostream& stream, SharedPtr<MultiBinaryTree<Tkey, Tvalue>> tree)
	{
		stream << (BinaryTree<Tkey, Multi<Tvalue>>*) tree.Get();

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const MultiBinaryTree<Tkey, Tvalue>& tree)
	{
		stream << ((BinaryTree<Tkey, Multi<Tvalue>>*) (&tree));

		return stream;
	}
};
