#pragma once

#include "Logger.h"

#include "Pointer.h"
#include "Logger.h"
#include "Sequence/IContainer.h"
#include <functional>

enum TraversePath
{
    Left, Root, Right
};

template <typename Tkey, typename Tvalue>
using KGen = Tkey (*)(Tvalue);

template <typename Tkey, typename Tvalue>
struct TreeNode
{
    Tkey key;
    Tvalue data;
    KGen<Tkey, Tvalue> kGen = nullptr;

    SharedPtr<TreeNode<Tkey, Tvalue>> parent;
    SharedPtr<TreeNode<Tkey, Tvalue>> left;
    SharedPtr<TreeNode<Tkey, Tvalue>> right;

    TreeNode()
        : key(Tkey()), data(Tvalue()), kGen( [](Tvalue value) -> Tkey {return Tkey(value);} ), parent(nullptr), left(nullptr), right(nullptr)
    {
        key = kGen(data);
    }

    TreeNode(const Tvalue& value)
        : key(Tkey(value)), data(value), kGen( [](Tvalue value) -> Tkey {return Tkey(value);} ), parent(nullptr), left(nullptr), right(nullptr)
    {
        key = kGen(data);
    }

    TreeNode(const Tvalue& value, KGen<Tkey, Tvalue> kGen)
        : key(Tkey(value)), data(value), kGen(kGen), parent(nullptr), left(nullptr), right(nullptr)
    {
        key = kGen(data);
    }

    TreeNode(const Tkey& key, const Tvalue& value)
        : key(key), data(value), kGen( [](Tvalue value) -> Tkey {return Tkey(value);} ), parent(nullptr), left(nullptr), right(nullptr)
    {
        key = kGen(data);
    }

    TreeNode(const Tkey& key, const Tvalue& value, KGen<Tkey, Tvalue> kGen)
        : key(key), data(value), kGen(kGen), parent(nullptr), left(nullptr), right(nullptr)
    {
        key = kGen(data);
    }

    TreeNode(TreeNode<Tkey, Tvalue>&& other)
    {
        key = other.key;
        other.key = Tkey();

        data = other.data;
        other.data = Tvalue();

        kGen = other.kGen;
        other.kGen = nullptr;

        parent = other.parent;
        other.parent = nullptr;

        left = other.left;
        other.left = nullptr;

        right = other.right;
        other.right = nullptr;
    }
};

template <typename Tkey, typename Tvalue>
class Tree
{
public:
    virtual ~Tree()
    {
        Logger::Info("Destroyed Tree<T>");
    }



    virtual Size Depth(SharedPtr<TreeNode<Tkey, Tvalue>> startNode) const noexcept = 0;
    virtual Size Depth(SharedPtr<TreeNode<Tkey, Tvalue>> startNode, Size depth) const noexcept = 0;
    virtual Size Depth() const noexcept = 0;

    virtual Tree<Tkey, Tvalue>* Add(const Tvalue& value) noexcept = 0;
    virtual SharedPtr<TreeNode<Tkey, Tvalue>> GetRoot() const noexcept = 0;

    virtual void Traverse (SharedPtr<TreeNode<Tkey, Tvalue>> startNode, TraversePath first, TraversePath second, TraversePath third, std::function<void(Tvalue)> func) = 0;
    virtual void Traverse (TraversePath first, TraversePath second, TraversePath third, std::function<void(Tvalue)> func) = 0;

    virtual Tree<Tkey, Tvalue>* Create() const noexcept = 0;
    virtual Tree<Tkey, Tvalue>* Copy() const noexcept = 0;

    virtual std::ostream& Dump(std::ostream& stream, const SharedPtr<TreeNode<Tkey, Tvalue>>& startNode, TraversePath first, TraversePath second, TraversePath third) const noexcept = 0;
    virtual void Dump(TraversePath first, TraversePath second, TraversePath third) const noexcept = 0;

    friend std::ostream& operator<< (std::ostream& stream, const Tree<Tkey, Tvalue>& tree)
    {
        tree.Dump(stream, tree.GetRoot(), Left, Root, Right);

        return stream;
    }

    friend std::ostream& operator<< (std::ostream& stream, const SharedPtr<Tree<Tkey, Tvalue>>& tree)
    {
        tree->Dump(stream, tree->GetRoot(), Left, Root, Right);

        return stream;
    }
};