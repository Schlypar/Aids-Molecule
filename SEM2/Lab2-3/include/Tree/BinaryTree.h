#pragma once

#include "ITree.h"



#define NOT_DONE true


template <typename T>
class BinaryTree : Tree<T>
{
    

    std::ostream& printTree(std::ostream& stream, const SharedPtr<TreeNode<T>>& startNode) const noexcept
    {
        if (startNode->left)
            this->printTree(stream, startNode->left);

        stream << startNode->data << " ";      
        
        if (startNode->right)
            this->printTree(stream, startNode->right);

        return stream;
    }

private:
    SharedPtr<TreeNode<T>> root;

public:
    BinaryTree()
        : root(nullptr)
    {
        Logger::Info("Default constructor of BinaryTree<T>");
    }

    BinaryTree(const T& startValue)
        : root(new TreeNode<T>(startValue))
    {
        Logger::Info("Starting value constructor of BinaryTree<T>");
    }

    BinaryTree(const TreeNode<T>* startRoot)
        : root(startRoot)
    {
        Logger::Info("Starting root node constructor of BinaryTree<T>");
    }

    virtual ~BinaryTree()
    {
        // Logger::Info("BinaryTree<T> was destroyed");
    }

    Size Depth(SharedPtr<TreeNode<T>> startNode) const noexcept override
    {
        Size depth = 0;
        if (startNode->left)
            depth = std::max(depth, Depth(startNode->left, depth));

        if (startNode->right)
            depth = std::max(depth, Depth(startNode->right, depth));

        return depth; 
    }

    Size Depth(SharedPtr<TreeNode<T>> startNode, Size depth) const noexcept override
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

    Tree<T>* Insert(const T& value) noexcept override
    {
        if (root == nullptr)
        {
            root = SharedPtr<TreeNode<T>>(new TreeNode<T>(value));
            return this;
        }

        SharedPtr<TreeNode<T>> current = root;

        while (NOT_DONE)
        {
            if (current->left == nullptr)
            {
                current->left = SharedPtr<TreeNode<T>>(new TreeNode<T>(value));
                current->left->parent = current;
                return this;
            }

            if (current->right == nullptr)
            {
                current->right = SharedPtr<TreeNode<T>>(new TreeNode<T>(value));
                current->right->parent = current;
                return this;
            }

            if (Depth(current->left) < Depth(current->right))
                current = current->left;
            else current = current->right;
        }

        return this;
    }

    SharedPtr<TreeNode<T>> GetRoot() const noexcept override
    {
        return root;
    }

    friend std::ostream& operator<< (std::ostream& stream, const BinaryTree<T>& tree)
    {
        tree.printTree(stream, tree.GetRoot());

        return stream;
    }

};