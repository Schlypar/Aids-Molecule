#pragma once

#include "ITree.h"
#include "Logger.h"
#include "Pointer.h"

#define NOT_DONE true

template <typename T1, typename T2>
using Ptr = SharedPtr<TreeNode<T1, T2>>;


template <typename Tkey, typename Tvalue>
class BinaryTree : Tree<Tkey, Tvalue>
{
    void CopyNodes(Ptr<Tkey, Tvalue>& copyNode, const Ptr<Tkey, Tvalue>& originalNode)
    {
        if (originalNode->left)
        {
            copyNode->left = Ptr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->left->data, originalNode->left->kGen));
            CopyNodes(copyNode->left, originalNode->left);
        }
        if (originalNode->right)
        {
            copyNode->right = Ptr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(originalNode->right->data, originalNode->right->kGen));
            CopyNodes(copyNode->right, originalNode->right);
        }
    }

    std::ostream& printTree(std::ostream& stream, const Ptr<Tkey, Tvalue>& startNode) const noexcept
    {
        if (startNode->left)
            this->printTree(stream, startNode->left);

        stream << startNode->data << " ";      
        
        if (startNode->right)
            this->printTree(stream, startNode->right);

        return stream;
    }

    std::ostream& printTree(std::ostream& stream, const Ptr<Tkey, Tvalue>& startNode, TraversePath first, TraversePath second, TraversePath third) const
    {
        if (first == second || first == third || second == third)
        {
            Logger::Info("At Traverse(TraversePath, TraversePath, TraversePath) at BinaryTree<T>");
            logException(EXCEPTION_BAD_LOGIC);
            throw EXCEPTION_BAD_LOGIC;
        }

        if (startNode == nullptr)
            return stream;
        
        
        if (first == Left)
            printTree(stream, startNode->left, first, second, third);
        else if (first == Root)
            stream << startNode->data << " ";
        else if (first == Right)
            printTree(stream, startNode->right, first, second, third);
            
        if (second == Left)
            printTree(stream, startNode->left, first, second, third);
        else if (second == Root)
            stream << startNode->data << " ";
        else if (second == Right)
            printTree(stream, startNode->right, first, second, third);

        if (third == Left)
            printTree(stream, startNode->left, first, second, third);
        else if (third == Root)
            stream << startNode->data << " ";
        else if (third == Right)
            printTree(stream, startNode->right, first, second, third);
    }

    std::ostream& Dump(std::ostream& stream, const Ptr<Tkey, Tvalue>& startNode, TraversePath first, TraversePath second, TraversePath third) const noexcept override
    {
        if (first == Left)
        {
            std::cout << "[ ";
            printTree(std::cout, GetRoot()->left, first, second, third);
            std::cout << "]";
        }
        if (first == Root)
        {
            std::cout << "(";
            std::cout << GetRoot()->data;
            std::cout << ")";
        }
        if (first == Right)
        {
            std::cout << "[ ";
            printTree(std::cout, GetRoot()->right, first, second, third);
            std::cout << "]";
        }


        if (second == Left)
        {
            std::cout << "[ ";
            printTree(std::cout, GetRoot()->left, first, second, third);
            std::cout << "]";
        }
        if (second == Root)
        {
            std::cout << "(";
            std::cout << GetRoot()->data;
            std::cout << ")";
        }
        if (second == Right)
        {
            std::cout << "[ ";
            printTree(std::cout, GetRoot()->right, first, second, third);
            std::cout << "]";
        }


        if (third == Left)
        {
            std::cout << "[ ";
            printTree(std::cout, GetRoot()->left, first, second, third);
            std::cout << "]";
        }
        if (third == Root)
        {
            std::cout << "(";
            std::cout << GetRoot()->data;
            std::cout << ")";
        }
        if (third == Right)
        {
            std::cout << "[ ";
            printTree(std::cout, GetRoot()->right, first, second, third);
            std::cout << "]";
        }

        return stream;
    }

private:
    Ptr<Tkey, Tvalue> root;

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

    Size Depth(Ptr<Tkey, Tvalue> startNode) const noexcept override
    {
        Size depth = 0;
        if (startNode->left)
            depth = std::max(depth, Depth(startNode->left, depth));

        if (startNode->right)
            depth = std::max(depth, Depth(startNode->right, depth));

        return depth; 
    }

    Size Depth(Ptr<Tkey, Tvalue> startNode, Size depth) const noexcept override
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
            root = Ptr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(value));
            return this;
        }

        Ptr<Tkey, Tvalue> current = root;

        while (NOT_DONE)
        {
            if (current->left == nullptr)
            {
                current->left = Ptr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(value, current->kGen));
                current->left->parent = current;
                return this;
            }

            if (current->right == nullptr)
            {
                current->right = Ptr<Tkey, Tvalue>(new TreeNode<Tkey, Tvalue>(value, current->kGen));
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

    Ptr<Tkey, Tvalue> GetRoot() const noexcept override
    {
        return root;
    }

    void Traverse (Ptr<Tkey, Tvalue> startNode, TraversePath first, TraversePath second, TraversePath third, std::function<void(Tvalue)> func) override
    {
        if (first == second || first == third || second == third)
        {
            Logger::Info("At Traverse(TraversePath, TraversePath, TraversePath) at BinaryTree<T>");
            logException(EXCEPTION_BAD_LOGIC);
            throw EXCEPTION_BAD_LOGIC;
        }

        if (startNode == nullptr)
            return;
        
        if (first == Left)
            Traverse(startNode->left, first, second, third, func);
        if (first == Root)
            func(startNode->data);
        if (first == Right)
            Traverse(startNode->right, first, second, third, func);
            
        if (second == Left)
            Traverse(startNode->left, first, second, third, func);
        if (second == Root)
            func(startNode->data);
        if (second == Right)
            Traverse(startNode->right, first, second, third, func);

        if (third == Left)
            Traverse(startNode->left, first, second, third, func);
        if (third == Root)
            func(startNode->data);
        if (third == Right)
            Traverse(startNode->right, first, second, third, func);

    }

    void Traverse (TraversePath first, TraversePath second, TraversePath third, std::function<void(Tvalue)> func) override
    {
        if (first == second || first == third || second == third)
        {
            Logger::Info("At Traverse(TraversePath, TraversePath, TraversePath) at BinaryTree<T>");
            logException(EXCEPTION_BAD_LOGIC);
            throw EXCEPTION_BAD_LOGIC;
        }

        if (root == nullptr)
            return;
        
        if (first == Left)
            Traverse(root->left, first, second, third, func);
        else if (first == Root)
            func(root->data);
        else if (first == Right)
            Traverse(root->right, first, second, third, func);
            
        if (second == Left)
            Traverse(root->left, first, second, third, func);
        else if (second == Root)
            func(root->data);
        else if (second == Right)
            Traverse(root->right, first, second, third, func);

        if (third == Left)
            Traverse(root->left, first, second, third, func);
        else if (third == Root)
            func(root->data);
        else if (third == Right)
            Traverse(root->right, first, second, third, func);

    }

    void Dump(TraversePath first, TraversePath second, TraversePath third) const noexcept override
    {
        Dump(std::cout, GetRoot(), first, second, third);
    }

    friend std::ostream& operator<< (std::ostream& stream, const BinaryTree<Tkey, Tvalue>& tree)
    {
        tree.Dump(stream, tree.GetRoot(), Left, Root, Right);

        return stream;
    }

    friend std::ostream& operator<< (std::ostream& stream, const SharedPtr<BinaryTree<Tkey, Tvalue>>& tree)
    {
        tree->Dump(stream, tree->GetRoot(), Left, Root, Right);

        return stream;
    }
};