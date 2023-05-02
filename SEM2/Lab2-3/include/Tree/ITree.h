#pragma once

#include "Logger.h"

#include "Pointer.h"
#include "Logger.h"
#include "Sequence/IContainer.h"

template <typename U>
struct TreeNode
{
    U data;

    SharedPtr<TreeNode<U>> parent;
    SharedPtr<TreeNode<U>> left;
    SharedPtr<TreeNode<U>> right;

    TreeNode()
        : data(U()), parent(nullptr), left(nullptr), right(nullptr)
    {
    }

    TreeNode(const U& value)
        : data(value), parent(nullptr), left(nullptr), right(nullptr)
    {
    }

    TreeNode(const TreeNode<U>& other)
    {
        data = other.data;
        parent = other.parent;
        left = other.left;
        right = other.right;
    }

    TreeNode(TreeNode<U>&& other)
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

template <typename T>
class Tree
{
public:
    virtual ~Tree()
    {
        Logger::Info("Destroyed Tree<T>");
    }

    virtual Size Depth(SharedPtr<TreeNode<T>> startNode) const noexcept = 0;
    virtual Size Depth(SharedPtr<TreeNode<T>> startNode, Size depth) const noexcept = 0;
    virtual Size Depth() const noexcept = 0;

    virtual Tree<T>* Insert(const T& value) noexcept = 0;
    virtual SharedPtr<TreeNode<T>> GetRoot() const noexcept = 0;
};