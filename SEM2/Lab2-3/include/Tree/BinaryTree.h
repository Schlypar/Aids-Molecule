#pragma once

#include "ITree.h"
#include "Pointer.h"
#include <stdexcept>

#define NOT_DONE true

// template <typename T1, typename T2>
// using Ptr = SharedPtr<TreeNode<T1, T2>>;


template <typename Tkey, typename Tvalue>
class BinaryTree : Tree<Tkey, Tvalue>
{
    void CopyNodes(SharedPtr<TreeNode<Tkey, Tvalue>>& copyNode, const SharedPtr<TreeNode<Tkey, Tvalue>>& originalNode)
    {
        if (originalNode->left)
        {
            copyNode->left = SharedPtr<TreeNode<Tkey, Tvalue>>(new TreeNode<Tkey, Tvalue>(originalNode->left->data));
            CopyNodes(copyNode->left, originalNode->left);
        }
        if (originalNode->right)
        {
            copyNode->right = SharedPtr<TreeNode<Tkey, Tvalue>>(new TreeNode<Tkey, Tvalue>(originalNode->right->data));
            CopyNodes(copyNode->right, originalNode->right);
        }
    }

    std::ostream& printTree(std::ostream& stream, const SharedPtr<TreeNode<Tkey, Tvalue>>& startNode) const noexcept
    {
        if (startNode->left)
            this->printTree(stream, startNode->left);

        stream << startNode->data << " ";      
        
        if (startNode->right)
            this->printTree(stream, startNode->right);

        return stream;
    }

private:
    SharedPtr<TreeNode<Tkey, Tvalue>> root;

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

    BinaryTree(const TreeNode<Tkey, Tvalue>* startRoot)
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

    Size Depth(SharedPtr<TreeNode<Tkey, Tvalue>> startNode) const noexcept override
    {
        Size depth = 0;
        if (startNode->left)
            depth = std::max(depth, Depth(startNode->left, depth));

        if (startNode->right)
            depth = std::max(depth, Depth(startNode->right, depth));

        return depth; 
    }

    Size Depth(SharedPtr<TreeNode<Tkey, Tvalue>> startNode, Size depth) const noexcept override
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
            root = SharedPtr<TreeNode<Tkey, Tvalue>>(new TreeNode<Tkey, Tvalue>(value));
            return this;
        }

        SharedPtr<TreeNode<Tkey, Tvalue>> current = root;

        while (NOT_DONE)
        {
            if (current->left == nullptr)
            {
                current->left = SharedPtr<TreeNode<Tkey, Tvalue>>(new TreeNode<Tkey, Tvalue>(value));
                current->left->parent = current;
                return this;
            }

            if (current->right == nullptr)
            {
                current->right = SharedPtr<TreeNode<Tkey, Tvalue>>(new TreeNode<Tkey, Tvalue>(value));
                current->right->parent = current;
                return this;
            }

            if (Depth(current->left) < Depth(current->right))
                current = current->left;
            else current = current->right;
        }

        return this;
    }

    Tree<Tkey, Tvalue>* Create() const noexcept override
    {
        return (Tree<Tkey, Tvalue>*) new BinaryTree<Tkey, Tvalue>();
    }

    Tree<Tkey, Tvalue>* Copy() const noexcept override
    {
        return (Tree<Tkey, Tvalue>*) new BinaryTree<Tkey, Tvalue>(*this);
    }

    SharedPtr<TreeNode<Tkey, Tvalue>> GetRoot() const noexcept override
    {
        return root;
    }

    friend std::ostream& operator<< (std::ostream& stream, const BinaryTree<Tkey, Tvalue>& tree)
    {
        tree.printTree(stream, tree.GetRoot());

        return stream;
    }

};