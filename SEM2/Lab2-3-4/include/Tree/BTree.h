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
void BTree<Tkey, Tvalue>::SplitChild(Node& node, int index) noexcept
{
	Node& fullChild = node.children[index];

	node.children.InsertAt(
		index + 1,
		Node(fullChild.isLeaf, this->sorter->Copy(), this->comparator)
	);
	node.keys.Add(fullChild.keys[t - 1]); // adds the median of a child to its parent

	int i = 0;

	// slice of fullChild keys between t (inclusive) and 2t - 1 (exclusive)
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

	// slice of fullChild keys between 0 (inclusive) and t - 1 (exclusive)
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
		// slice of fullChild children between t (inclusive) and 2t - 1 (exclusive)
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

		// slice of fullChild children between 0 (inclusive) and t - 1 (exclusive)
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
