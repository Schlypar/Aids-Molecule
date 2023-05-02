#pragma once

#include "Pointer.h"
#include "Logger.h"
#include "IContainer.h"

#define NOT_DONE true


template <typename T>
class BinaryTree
{
    template <typename U>
    struct Node
    {
        T data;

        SharedPtr<Node<U>> parent;
        SharedPtr<Node<U>> left;
        SharedPtr<Node<U>> right;

        Node()
            : data(U()), parent(nullptr), left(nullptr), right(nullptr)
        {
        }

        Node(const U& value)
            : data(value), parent(nullptr), left(nullptr), right(nullptr)
        {
        }

        Node(const Node<U>& other)
        {
            data = other.data;
            parent = other.parent;
            left = other.left;
            right = other.right;
        }

        Node(Node<U>&& other)
        {
            data = other.data;
            other.data = U();

            parent = other.parent;
            other.parent = nullptr;

            left = other.left;
            other.left = nullptr;

            right = other.right;
            other.right = nullptr;
        }
    };

    std::ostream& printTree(std::ostream& stream, const SharedPtr<Node<T>>& startNode) const noexcept
    {
        if (startNode->left)
            this->printTree(stream, startNode->left);

        stream << startNode->data << " ";      
        
        if (startNode->right)
            this->printTree(stream, startNode->right);

        return stream;
    }

private:
    SharedPtr<Node<T>> root;

public:
    BinaryTree()
        : root(nullptr)
    {
        Logger::Info("Default constructor of BinaryTree<T>");
    }

    BinaryTree(const T& startValue)
        : root(new Node<T>(startValue))
    {
        Logger::Info("Starting value constructor of BinaryTree<T>");
    }

    BinaryTree(const Node<T>* startRoot)
        : root(startRoot)
    {
        Logger::Info("Starting root node constructor of BinaryTree<T>");
    }

    ~BinaryTree()
    {
        Logger::Info("BinaryTree<T> was destroyed");
    }

    Size Depth(SharedPtr<Node<T>> startNode) const noexcept
    {
        Size depth = 0;
        if (startNode->left)
            depth = std::max(depth, Depth(startNode->left, depth));

        if (startNode->right)
            depth = std::max(depth, Depth(startNode->right, depth));

        return depth; 
    }

    Size Depth(SharedPtr<Node<T>> startNode, Size depth) const noexcept
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

    Size Depth() const noexcept
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

    BinaryTree<T>* Insert(const T& value)
    {
        if (root == nullptr)
        {
            root = SharedPtr<Node<T>>(new Node<T>(value));
            return this;
        }

        SharedPtr<Node<T>> current = root;

        while (NOT_DONE)
        {
            if (value < current->data)
            {
                if (current->left == nullptr)
                {
                    current->left = SharedPtr<Node<T>>(new Node<T>(value));
                    current->left->parent = current;
                    return this;
                }
                current = current->left;
            }
            else
            {
                if (current->right == nullptr)
                {
                    current->right = SharedPtr<Node<T>>(new Node<T>(value));
                    current->right->parent = current;
                    return this;
                }
                current = current->right;
            }
        }

        return this;
    }

    SharedPtr<Node<T>> GetRoot() const noexcept
    {
        return root;
    }

    friend std::ostream& operator<< (std::ostream& stream, const BinaryTree<T>& tree)
    {
        tree.printTree(stream, tree.GetRoot());

        return stream;
    }

};