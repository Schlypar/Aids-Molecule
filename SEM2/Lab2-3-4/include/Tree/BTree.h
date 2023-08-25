#pragma once

#include "Logger.h"
#include "Pair.h"
#include "Sequence/ArraySequence.h"
#include "Sequence/ListSequence.h"
#include "Sequence/SortedSequence.h"
#include "Sorter/HeapSort.h"
#include "concepts.h"
#include <functional>
#include <stdexcept>

template <Comparable Tkey, typename Tvalue>
class BTree
{
protected:
	using KeyValue = Pair<Tkey, Tvalue>;
	using KeysList = SortedSequence<KeyValue>;
	using CompFunc = std::function<int(const Tkey&, const Tkey&)>;
	using Sorter = ISorter<KeyValue>*;

	struct Node
	{
		using ChildrensList = List<Node>;

		KeysList keys;
		ChildrensList children;
		bool isLeaf = false;

		Node()
		    : keys(new ArraySequence<KeyValue>(), new HeapSorter<KeyValue>(), [](const KeyValue& a, const KeyValue& b) -> int
			   { return 1; })
		    , children()
		    , isLeaf(false)
		{
		}

		Node(Sorter sorter, CompFunc comparator)
		    : keys(new ArraySequence<KeyValue>(), sorter, [comparator](const KeyValue& a, const KeyValue& b) -> int
			   { return comparator(a.GetLeft(), b.GetLeft()); })
		    , children()
		    , isLeaf(false)
		{
		}

		Node(bool isLeaf, Sorter sorter, CompFunc comparator)
		    : keys(new ArraySequence<KeyValue>(), sorter, [comparator](const KeyValue& a, const KeyValue& b) -> int
			   { return comparator(a.GetLeft(), b.GetLeft()); })
		    , children()
		    , isLeaf(isLeaf)
		{
		}

		Node(const Node& other)
		    : keys(other.keys)
		    , children(other.children)
		    , isLeaf(other.isLeaf)
		{
		}

		Node(Node&& other)
		    : keys(std::move(other.keys))
		    , children(std::move(other.children))
		    , isLeaf(other.isLeaf)
		{
		}

		~Node()
		{
		}

		Node& operator=(const Node& other)
		{
			this->keys = other.keys;
			this->children = other.children;
			this->isLeaf = other.isLeaf;

			return *this;
		}

		Node& operator=(Node&& other)
		{
			this->keys = std::move(other.keys);
			this->children = std::move(other.children);
			this->isLeaf = std::move(other.isLeaf);

			return *this;
		}

		bool operator==(const Node& other) const
		{
			bool result = this->keys == other.keys;
			result = result && this->children == other.children;
			result = result && this->isLeaf == other.isLeaf;

			return result;
		}
	};

private:
	Node root = Node(true);
	size_t t = 2; // minimum degree of tree (t >= 2) or branching factor

	Sorter sorter = nullptr;       // sorter for SortedSequence. That's in the Node struct
	CompFunc comparator = nullptr; // how to compare two Tkey values. Should be able to return 0 if elements are the same

public:
	BTree(ISorter<KeyValue>* sorter, std::function<int(const Tkey&, const Tkey&)> comparator)
	    : root(true, sorter->Copy(), comparator)
	    , t(2)
	    , sorter(sorter)
	    , comparator(comparator)
	{
	}

	BTree(size_t t, ISorter<KeyValue>* sorter, std::function<int(const Tkey&, const Tkey&)> comparator)
	    : root(true, sorter->Copy(), comparator)
	    , t(t)
	    , sorter(sorter)
	    , comparator(comparator)
	{
	}

	BTree(const BTree<Tkey, Tvalue>& other)
	    : root(other.root)
	    , t(other.t)
	    , sorter(other.sorter->Copy())
	    , comparator(other.comparator)
	{
	}

	BTree(BTree<Tkey, Tvalue>&& other)
	    : root(std::move(other.root))
	    , t(std::move(other.t))
	    , sorter(other.comparator)
	    , comparator(other.comparator)
	{
		other.sorter = nullptr;
	}

	~BTree()
	{
		delete sorter;
	}

	/**
	 * @brief finds element by its key with O(log(n)) complexity. Throws if element was not found
	 *
	 * @param key by which key to find value
	 * @return copy of the found element
	 */
	Tvalue Search(const Tkey& key) const;

	/**
	 * @brief will insert record at the B tree and will reconstruct it if needed. 
     * Does nothing if same key already appears in the tree
	 *
	 * @param record what to insert
	 */
	BTree<Tkey, Tvalue>* Insert(const Pair<Tkey, Tvalue>& record) noexcept;

	/**
     * @brief will delete node by its key. Will throw if key is not present in the tree
     *
     * @param key what to look for
     */
	void Delete(const Tkey& key);

	friend std::ostream& operator<<(std::ostream& stream, const BTree<Tkey, Tvalue>& tree)
	{
		tree.print(stream);

		return stream;
	}

private:
	BTree()
	    : root(true)
	    , t(2)
	    , sorter(nullptr)
	    , comparator(nullptr)
	{
	}

	/**
	 * @brief recursive helper function that searches Tvalue by key given
	 *
	 * @param key by which key to find value
	 * @param node starting node
	 * @return copy of the found element
	 */
	Tvalue Search(const Tkey& key, Node& node) const;

	/**
     * @brief recursive function that searches by key node, starting from node
     *
     * @param key for what to look for
     * @param node starting node
     * @return pair of found node (reference to it) and index where key will be
     */
	Pair<Node&, int> SearchForNode(const Tkey& key, Node& node) const;

	/**
	 * @brief takes non-full internal node and index of a full child and splits him
	 *
	 * @param node parent non-full node
	 * @param index index of a full child
	 */
	void SplitChild(Node& node, int index) noexcept;

	/**
	 * @brief inserts in the node that is not full
	 *
	 * @param node to which node insert
	 * @param record what to insert
	 */
	void InsertNonFull(Node& node, const KeyValue& record) noexcept;

	/**
     * @brief recursive helper function for delete
     *
     * @param node starting node
     * @param key which key to delete
     */
	void Delete(Node& node, const Tkey& key);

	/**
     * @brief if node is a leaf then tries to delete from it, otherwise branches off to different delete methods
     *
     * @param node from which node to delete (or from whose children to look for)
     * @param key what to delete
     * @param index index of key (or child)
     */
	void DeleteInternalNode(Node& node, const Tkey& key, int index);

	/**
     * @brief if node is a leaf then deletes from him, otherwise deletes from child
     *
     * @param node from which node to delete
     */
	Node& DeletePredecessor(Node& node);

	/**
     * @brief if node is a leaf then deletes from him, otherwise deletes from child
     *
     * @param node from which node to delete
     */
	Node& DeleteSuccessor(Node& node);

	/**
     * @brief merges children[i] and children[j] of node into one.
     *
     * @param node what node's children needs to be merged
     * @param i index of first child
     * @param j index of second child
     */
	void DeleteMerge(Node& node, int i, int j);

	/**
     * @brief like DeleteMerge, but different
     *
     * @param node from what node to look for
     * @param i index of first child
     * @param j index of second child
     */
	void DeleteSibling(Node& node, int i, int j);

	/**
     * @brief prints tree to the standard output
     *
     * @param stream reference to the standard output
     */
	void print(std::ostream& stream) const;

	/**
     * @brief helper function for print
     *
     * @param stream reference to the standard output
     * @param node from which node to start
     * @param level level of that node
     */
	void print(std::ostream& stream, Node& node, int level) const;
};

template <Comparable Tkey, typename Tvalue>
Tvalue BTree<Tkey, Tvalue>::Search(const Tkey& key) const
{
	const Node& node = this->root;
	int index = 0;

	while (index < node.keys.GetLength() && comparator(key, node.keys[index].GetLeft()) > 0)
	{
		index++;
	}

	if (index < node.keys.GetLength() && comparator(key, node.keys[index].GetLeft()) == 0)
	{
		return node.keys[index].GetRight();
	}
	else if (node.isLeaf)
	{
		throw std::out_of_range("Couldnt find element by this key");
	}
	else
	{
		return Search(key, node.children[index]);
	}
}

template <Comparable Tkey, typename Tvalue>
Tvalue BTree<Tkey, Tvalue>::Search(const Tkey& key, Node& node) const
{
	int index = 0;

	while (index < node.keys.GetLength() && comparator(key, node.keys[index].GetLeft()) > 0)
	{
		index++;
	}

	if (index < node.keys.GetLength() && comparator(key, node.keys[index].GetLeft()) == 0)
	{
		return node.keys[index].GetRight();
	}
	else if (node.isLeaf)
	{
		throw std::out_of_range("Couldnt find element by this key");
	}
	else
	{
		return Search(key, node.children[index]);
	}
}

template <Comparable Tkey, typename Tvalue>
Pair<typename BTree<Tkey, Tvalue>::Node&, int> BTree<Tkey, Tvalue>::SearchForNode(const Tkey& key, Node& node) const
{
	int index = 0;

	while (index < node.keys.GetLength() && comparator(key, node.keys[index].GetLeft()) > 0)
	{
		index++;
	}

	if (index < node.keys.GetLength() && comparator(key, node.keys[index].GetLeft()) == 0)
	{
		return { node, index };
	}
	else if (node.isLeaf)
	{
		throw std::out_of_range("Couldnt find element by this key");
	}
	else
	{
		return SearchForNode(key, node.children[index]);
	}
}

template <Comparable Tkey, typename Tvalue>
void BTree<Tkey, Tvalue>::SplitChild(Node& node, int index) noexcept
{
	Node& fullChild = node.children[index];

	node.children.InsertAt(
		index + 1,
		Node(fullChild.isLeaf, this->sorter->Copy(), this->comparator)
	);
	node.keys.Add(fullChild.keys[t - 1]); // adds the median of a child to its parent

	int i = 0;

	// slice of fullChild keys between t and 2t - 1
	node.children[index + 1].keys = fullChild.keys
		| fn::filter<KeyValue>(
						[&i, this](KeyValue& pair) -> bool
						{
							bool result = (i >= t) && (i < (2 * t) - 1);
							++i;
							return result;
						}
		);

	i = 0;

	// slice of fullChild keys between 0 and t - 1
	node.children[index].keys = fullChild.keys
		| fn::filter<KeyValue>(
					    [&i, this](KeyValue& pair) -> bool
					    {
						    bool result = (i >= 0) && (i < t - 1);
						    ++i;
						    return result;
					    }
		);

	i = 0;

	if (!fullChild.isLeaf)
	{
		// slice of fullChild children between t and 2t - 1
		node.children[index + 1].children = fullChild.children
			| fn::filter<Node>(
							    [&i, this](Node& node) -> bool
							    {
								    bool result = (i >= t) && (i <= (2 * t) - 1);
								    ++i;
								    return result;
							    }
			);

		i = 0;

		// slice of fullChild children between 0 and t - 1
		node.children[index].children = fullChild.children
			| fn::filter<Node>(
							[&i, this](Node& node) -> bool
							{
								bool result = (i >= 0) && (i <= t - 1);
								++i;
								return result;
							}
			);
	}
}

template <Comparable Tkey, typename Tvalue>
BTree<Tkey, Tvalue>* BTree<Tkey, Tvalue>::Insert(const Pair<Tkey, Tvalue>& record) noexcept
{
	try
	{
		Search(record.GetLeft());
		return this;
	}
	catch (...)
	{
		if (root.keys.GetLength() == (t * 2) - 1)
		{
			Node newRoot = Node(this->sorter->Copy(), this->comparator);
			Node& oldRoot = this->root;

			newRoot.children.InsertAt(0, oldRoot);

			SplitChild(newRoot, 0);
			InsertNonFull(newRoot, record);

			this->root = std::move(newRoot);

			return this;
		}
		else
		{
			InsertNonFull(root, record);
			return this;
		}
	}
}

template <Comparable Tkey, typename Tvalue>
void BTree<Tkey, Tvalue>::InsertNonFull(Node& node, const KeyValue& record) noexcept
{
	int index = node.keys.GetLength() - 1;

	if (node.isLeaf)
	{
		node.keys.Add(record);
	}
	else
	{
		while (index >= 0 && comparator(record.GetLeft(), node.keys[index].GetLeft()) < 0)
		{
			index--;
		}

		index++;

		if (node.children[index].keys.GetLength() == (t * 2) - 1)
		{
			SplitChild(node, index);

			if (comparator(record.GetLeft(), node.keys[index].GetLeft()) > 0)
			{
				index++;
			}
		}

		InsertNonFull(node.children[index], record);
	}
}

template <Comparable Tkey, typename Tvalue>
void BTree<Tkey, Tvalue>::Delete(const Tkey& key)
{
	Pair<Node&, int> pair = SearchForNode(key, root);

	Node& node = pair.GetLeft();
	int deletionIndex = pair.GetRight();

	Pair<Tkey, Tvalue> value = node.keys[deletionIndex];

	int index = 0;

	while (index < node.keys.GetLength() && comparator(key, node.keys[index].GetLeft()) > 0)
	{
		index++;
	}

	if (node.isLeaf)
	{
		if (index < node.keys.GetLength() && comparator(node.keys[index].GetLeft(), key) == 0)
		{
			node.keys.Remove(index);
		}

		return;
	}

	if (index < node.keys.GetLength() && comparator(node.keys[index].GetLeft(), key) == 0)
	{
		DeleteInternalNode(node, key, index);
		node.keys.RemoveByValue(value);

		return;
	}
	else if (node.children[index].keys.GetLength() >= t)
	{
		Delete(node.children[index], key);
	}
	else
	{
		if (index != 0 && index + 2 < node.children.GetLength())
		{

			if (node.children[index - 1].keys.GetLength() >= t)
			{
				DeleteSibling(node, index, index - 1);
			}
			else if (node.children[index + 1].keys.GetLength() >= t)
			{
				DeleteSibling(node, index, index + 1);
			}
			else
			{
				DeleteMerge(node, index, index + 1);
			}
		}
		else if (index == 0)
		{
			if (node.children[index + 1].keys.GetLength() >= t)
			{
				DeleteSibling(node, index, index + 1);
			}
			else
			{
				DeleteMerge(node, index, index + 1);
			}
		}
		else if (index + 1 == node.children.GetLength())
		{

			if (node.children[index - 1].keys.GetLength() >= t)
			{
				DeleteSibling(node, index, index - 1);
			}
			else
			{
				DeleteMerge(node, index, index - 1);
			}
		}

		Delete(node.children[index], key);
	}
}

template <Comparable Tkey, typename Tvalue>
void BTree<Tkey, Tvalue>::Delete(Node& node, const Tkey& key)
{
	int index = 0;

	while (index < node.keys.GetLength() && comparator(key, node.keys[index].GetLeft()) > 0)
	{
		index++;
	}

	if (node.isLeaf)
	{
		if (index < node.keys.GetLength() && comparator(node.keys[index].GetLeft(), key) == 0)
		{
			node.keys.Remove(index);
		}

		return;
	}

	if (index < node.keys.GetLength() && comparator(node.keys[index].GetLeft(), key) == 0)
	{
		DeleteInternalNode(node, key, index);
		return;
	}
	else if (node.children[index].keys.GetLength() >= t)
	{
		Delete(node.children[index], key);
	}
	else
	{
		if (index != 0 && index + 2 < node.children.GetLength())
		{
			if (node.children[index - 1].keys.GetLength() >= t)
			{
				DeleteSibling(node, index, index - 1);
			}
			else if (node.children[index + 1].keys.GetLength() >= t)
			{
				DeleteSibling(node, index, index + 1);
			}
			else
			{
				DeleteMerge(node, index, index + 1);
			}
		}
		else if (index == 0)
		{
			if (node.children[index + 1].keys.GetLength() >= t)
			{
				DeleteSibling(node, index, index + 1);
			}
			else
			{
				DeleteMerge(node, index, index + 1);
			}
		}
		else if (index + 1 == node.children.GetLength())
		{
			if (node.children[index - 1].keys.GetLength() >= t)
			{
				DeleteSibling(node, index, index - 1);
			}
			else
			{
				DeleteMerge(node, index, index - 1);
			}
		}

		Delete(node.children[index], key);
	}
}

template <Comparable Tkey, typename Tvalue>
void BTree<Tkey, Tvalue>::DeleteInternalNode(Node& node, const Tkey& key, int index)
{
	if (node.isLeaf)
	{
		if (comparator(node.keys[index].GetLeft(), key) == 0)
		{
			node.keys.Remove(index);
		}

		return;
	}

	if (node.children[index].keys.GetLength() >= t)
	{
		Node& predecessor = DeletePredecessor(node.children[index]);

		node.keys.Add(predecessor.keys.GetLast());
		predecessor.keys.RemoveByValue(predecessor.keys.GetLast());

		return;
	}
	else if (node.children[index + 1].keys.GetLength() >= t)
	{
		Node& successor = DeleteSuccessor(node.children[index + 1]);

		node.keys.Add(successor.keys.GetFirst());
		successor.keys.Remove(0);

		return;
	}
	else
	{
		DeleteMerge(node, index, index + 1);
		DeleteInternalNode(node.children[index], key, t - 1);
	}
}

template <Comparable Tkey, typename Tvalue>
BTree<Tkey, Tvalue>::Node& BTree<Tkey, Tvalue>::DeletePredecessor(Node& node)
{
	if (node.isLeaf)
	{
		return node;
	}

	int n = node.keys.GetLength() - 1;

	if (node.children[n].keys.GetLength() >= t)
	{
		DeleteSibling(node, n + 1, n);
	}
	else
	{
		DeleteMerge(node, n, n + 1);
	}

	return DeletePredecessor(node.children[n]);
}

template <Comparable Tkey, typename Tvalue>
BTree<Tkey, Tvalue>::Node& BTree<Tkey, Tvalue>::DeleteSuccessor(Node& node)
{
	if (node.isLeaf)
	{
		return node;
	}

	if (node.children[1].keys.GetLength() >= t)
	{
		DeleteSibling(node, 0, 1);
	}
	else
	{
		DeleteMerge(node, 0, 1);
	}

	return DeleteSuccessor(node.children[0]);
}

template <Comparable Tkey, typename Tvalue>
void BTree<Tkey, Tvalue>::DeleteMerge(Node& node, int i, int j)
{
	Node& currentNode = node.children[i];

	Node& newNode = currentNode;

	if (j > i)
	{
		Node& rightSideNode = node.children[j];
		currentNode.keys.Add(node.keys[i]);

		for (int k = 0; k < rightSideNode.keys.GetLength(); k++)
		{
			currentNode.keys.Add(rightSideNode.keys[k]);

			if (rightSideNode.children.GetLength() > 0)
			{
				currentNode.children.Append(rightSideNode.children[k]);
			}
		}

		if (rightSideNode.children.GetLength() > 0)
		{
			currentNode.children.Append(rightSideNode.children.GetLast());
			rightSideNode.children.Remove(rightSideNode.children.GetLength() - 1);
		}

		node.keys.Remove(i);
		node.children.Remove(j);
	}
	else
	{
		Node& leftSideNode = node.children[j];
		leftSideNode.keys.Add(node.keys[j]);

		for (int k = 0; k < currentNode.keys.GetLength(); k++)
		{
			leftSideNode.keys.Add(currentNode.keys[k]);

			if (leftSideNode.children.GetLength() > 0)
			{
				leftSideNode.children.Append(currentNode.children[k]);
			}
		}

		if (leftSideNode.children.GetLength() > 0)
		{
			leftSideNode.children.Append(currentNode.children.GetLast());
			currentNode.children.Remove(currentNode.children.GetLength() - 1);
		}

		newNode = leftSideNode;

		node.keys.Remove(i);
		node.children.Remove(j);
	}

	if (newNode == root && node.keys.GetLength() == 0)
	{
		root = newNode;
	}
}

template <Comparable Tkey, typename Tvalue>
void BTree<Tkey, Tvalue>::DeleteSibling(Node& node, int i, int j)
{
	Node& currentNode = node.children[i];

	if (i < j)
	{
		Node& rightSideNode = node.children[j];
		currentNode.keys.Add(node.keys[i]);

		node.keys[i] = rightSideNode.keys[0];

		if (rightSideNode.children.GetLength() > 0)
		{
			currentNode.children.Append(rightSideNode.children[0]);
			rightSideNode.children.Remove(0);
		}

		rightSideNode.children.Remove(0);
	}
	else
	{
		Node& leftSideNode = node.children[j];
		currentNode.keys.Add(node.keys[i - 1]);

		node.keys[i - 1] = leftSideNode.keys.GetLast();
		leftSideNode.keys.RemoveByValue(leftSideNode.keys.GetLast());

		if (leftSideNode.children.GetLength() > 0)
		{
			currentNode.children.InsertAt(0, leftSideNode.children.GetFirst());
			leftSideNode.children.Remove(0);
		}
	}
}

template <Comparable Tkey, typename Tvalue>
void BTree<Tkey, Tvalue>::print(std::ostream& stream) const
{
	stream << "LEVEL 0: ";
	stream << root.keys << "\n";

	if (root.children.GetLength() > 0)
	{
		for (auto& e : root.children)
		{
			print(stream, e, 1);
		}
	}
}

template <Comparable Tkey, typename Tvalue>
void BTree<Tkey, Tvalue>::print(std::ostream& stream, Node& node, int level) const
{
	stream << "LEVEL " << level << ": ";
	stream << node.keys << "\n";

	if (node.children.GetLength() > 0)
	{
		for (auto& e : node.children)
		{
			print(stream, e, level + 1);
		}
	}
}
