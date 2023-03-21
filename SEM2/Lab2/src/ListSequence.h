#pragma once

#include "ADT.h"


template <typename T>
class ListSequence : public Sequence<T>
{
private:
    SegmentedList<T> container;

public:
    class Iterator : IIterator<T>
    {
        Node<T>* current;
    public:
        Iterator() 
            : current(nullptr) { Logger::Info("Used default constructor of ArraySequence<T>::Iterator"); }

        Iterator(Node<T>* data)
            : current(data) { Logger::Info("Used T* constructor of ArraySequence<T>::Iterator"); }
        
        Iterator(IIterator<T>* other)
            : current(((Iterator*)other)->current) 
            { 
                Logger::Info("Used IIterator* constructor of ArraySequence<T>::Iterator"); 
                Logger::Info("Deleted heap pointer of IIterator<T>*");
                delete other;
            }

        Iterator& operator+ (int n) 
        {
            while (n > 0)
            {
                this->Next();
                n--;
            }

            return *this;
        }

        Iterator& operator- (int n) 
        {
            while (n > 0)
            {
                this->Prev();
                n--;
            }

            return *this;
        }

        Iterator operator++ () { this->_Next() ; return *this; }

        Iterator operator-- () { this->_Prev() ; return *this; }

        bool operator!= (Iterator& other) const { return this->current != other.current; }

		bool operator== (Iterator& other) const { return this->current == other.current; }

        T& operator* () { return this->current->data; }

        IIterator<T>* _Next() override
        {
            current = current->next;
            return this;
        }

        IIterator<T>* _Prev() override
        {
            current = current->prev;
            return this;
        }

        T& _GetCurrent() override
        {
            return this->current->data;
        }

        T* _GetPointer() override
        {
            return &this->current->data;
        }

        bool _isEquals(IIterator<T>* other) override
        {
            return this->current == ((Iterator*)other)->current;
        }
    };

    IIterator<T>* _Begin() override { return (IIterator<T>*) new (Iterator)(container.GetHead()); }
    IIterator<T>* _End() override { return (IIterator<T>*) new (Iterator)(); }

    Iterator begin() { return (Iterator)(this->_Begin()); }
    Iterator end() { return (Iterator)(this->_End()); }

    ListSequence()
        : container() {}

    ListSequence(T* data, Size count)
        : container(data, count) {}
    
    ListSequence(const Sequence<T>& other)
        : container(other) {}

    ListSequence(Sequence<T>&& other)
        : container(other) {}
    
    ~ListSequence() {}

    T& GetFirst() const override
    {
        if (isEmpty())
        {
            Logger::Trace("At Get() at ArraySequence.h");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            exit(EXIT_FAILURE);
        }

        return container.GetFirst();
    }

    T& GetLast() const override
    {
        if (isEmpty())
        {
            Logger::Trace("At Get() at ArraySequence.h");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            exit(EXIT_FAILURE);
        }

        return container.GetLast();
    }

    T& Get(const Index index) const override
    {
        if (isEmpty())
        {
            Logger::Trace("At Get() at ArraySequence.h");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            exit(EXIT_FAILURE);
        }

        return container.Get(index);
    }

    Sequence<T>* Append(T&& data) override
    {
        container.Append(data);

        return this;
    }

    Sequence<T>* Append(const T& data) override
    {
        container.Append(data);

        return this;
    }

    Sequence<T>* Prepend(T&& data) override
    {
        container.Prepend(data);

        return this;
    }

    Sequence<T>* Prepend(const T& data) override
    {
        container.Prepend(data);

        return this;
    }

    void InsertAt(const Index index, const T& data) override
    {
        // if (index >= GetLength())
        // {
        //     Logger::Trace("At InsertAt() at ArraySequence.h");
        //     logException(EXCEPTION_INDEX_OUT_OF_RANGE);
        //     throw EXCEPTION_INDEX_OUT_OF_RANGE;
        // }

        // Node<T>* node = new Node<T>;
        // node->data = data;
        // node->next = nullptr;
        // node->prev = nullptr;

        // Node<T>* pointer = container.GetHead();

        // for (Index i = 0; i < index; i++)
        //     pointer = pointer->next;
        
        // node->next = pointer;
        // node->prev = pointer->prev;
        // pointer->prev = node;
        // node->prev->next = node;

        // container.SetSize(GetLength() + 1);
        container.insertAt(index, data);
    }

    Size GetLength() const noexcept override { return container.GetLength(); }

    bool isEmpty() const noexcept override { return GetLength() == 0; }

    T* GetFirstPointer() override
    {
        return &container.GetFirst();
    }

    T* GetEndPointer() override
    {
        return nullptr;
    }

    Sequence<T>* Create() override
    {
        return (Sequence<T>*) new ListSequence<T>();
    }

    Sequence<T>* Copy() override
    {
        return (Sequence<T>*) new ListSequence<T>(*this);
    }

    T& operator[] (const Index index) { return container[index]; }

    friend std::ostream& operator<< (std::ostream& stream, ListSequence<T>& list)
    {
        if (list.isEmpty())
        {
            Logger::Trace("At operator<< overload at Array.h");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            return stream;
        }

        stream << list.container;

        return stream;
    }

    friend std::ostream& operator<< (std::ostream& stream, ListSequence<T>* list)
    {
        if (list->isEmpty())
        {
            Logger::Trace("At operator<< overload at Array.h");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            return stream;
        }

        stream << list->container;

        return stream;
    }
};

