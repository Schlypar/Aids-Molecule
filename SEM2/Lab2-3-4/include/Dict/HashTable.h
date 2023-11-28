#pragma once

#include "Dict/IHasher.h"
#include "IDictionary.h"
#include "Pair.h"
#include "STLHasher.h"
#include "Sequence/ArraySequence.h"
#include <initializer_list>

#define DEFAULT_SIZE 16

template <typename Tkey, typename Tvalue>
class HashTable : IDict<Tkey, Tvalue>
{
	using CollisionList = ArraySequence<Pair<Tkey, Tvalue>>;
	using AssociativeArray = Array<CollisionList>;

private:
	AssociativeArray hashTable;
	IHasher<Tkey>* hasher;
	size_t count;

	float multiplier = 0.8;

public:
	HashTable()
	    : hashTable(DEFAULT_SIZE)
	    , hasher(new STLHasher<Tkey>())
	    , count(0)
	{
	}

	HashTable(IHasher<Tkey>* hasher)
	    : hashTable(DEFAULT_SIZE)
	    , hasher(hasher)
	    , count(0)
	{
	}

	HashTable(std::initializer_list<Pair<Tkey, Tvalue>> args)
	    : hashTable(args.size())
	    , hasher(new STLHasher<Tkey>())
	    , count(0)
	{
		for (auto& e : args)
		{
			Add(e);
		}
	}

	HashTable(IHasher<Tkey>* hasher, std::initializer_list<Pair<Tkey, Tvalue>> args)
	    : hashTable(size_t(args.size()))
	    , hasher(hasher)
	    , count(0)
	{
		for (auto& e : args)
		{
			Add(e);
		}
	}

	HashTable(const HashTable<Tkey, Tvalue>& other)
	    : hashTable(other.hashTable)
	    , hasher(other.hasher->Copy())
	    , count(other.count)
	{
	}

	HashTable(HashTable<Tkey, Tvalue>&& other)
	    : hashTable(std::move(other.hashTable))
	    , hasher(other.hasher)
	    , count(other.count)
	{
		other.count = 0;
		other.hasher = nullptr;
	}

	~HashTable()
	{
		delete hasher;
	}

	HashTable<Tkey, Tvalue>& operator=(const HashTable<Tkey, Tvalue>& other);

	HashTable<Tkey, Tvalue>& operator=(HashTable<Tkey, Tvalue>&& other);

	/**
	 * @brief adds record to the hash table and increments member count. Will throw if key already is in table
	 *
	 * @param record contains Key-Value pair
	 * @return pointer to the base class IDict
	 */
	IDict<Tkey, Tvalue>* Add(Pair<Tkey, Tvalue> record) override;

	/**
	 * @brief removes element by its key and decrements member count.
     * Throws if key is not in the table
	 *
	 * @param key key by which we find what to deleted
	 */
	void Remove(const Tkey& key) override;

	/**
	 * @brief Gets element by its key. Throws if there wasnt such a key
	 *
	 * @param key key of the element
	 * @return copy of element
	 */
	Tvalue Get(const Tkey& key) override;

	/**
	 * @brief Gets element by its key. Throws if there wasnt such a key
	 *
	 * @param key key of the element
	 * @return reference of element
	 */
	Tvalue& GetMut(const Tkey& key) override;

	Tvalue operator[](const Tkey& key)
	{
		return Get(key);
	}

	/**
	 * @brief checks if key already exists
	 *
	 * @param key key to look for
	 * @return returns wheter key is in hash table or not
	 */
	bool ContainsKey(const Tkey& key) const noexcept override;

	/**
	 * @brief returns actual number of elements in hash table
	 *
	 * @return number of elements
	 */
	size_t GetCount() const noexcept override;

	/**
	 * @brief return maximum number of elements that can be stored
	 *
	 * @return returns size of member array
	 */
	size_t GetCapacity() const noexcept override;

	friend std::ostream& operator<<(std::ostream& stream, HashTable<Tkey, Tvalue>& hashTable)
	{
		size_t index = 0;
		for (CollisionList& list : hashTable.hashTable)
		{
			(list.isEmpty()) ? stream << index++ << "\t=> [----]\n" : stream << index++ << "\t=> " << list << "\n";
		}

		return stream;
	}

private:
	/**
	 * @brief returns index that corresponds with the key
	 *
	 * @param key key by which hash determines index
	 * @return index in the hash table
	 */
	size_t Hash(const Tkey& key) const noexcept
	{
		return hasher->Hash(key) % hashTable.GetLength();
	}

	/**
	 * @brief Checks if hash table needs to be reconstructed
	 *
	 * @return return true or false
	 */
	bool NeedToReconstruct() const noexcept
	{
		// return (count > multiplier * GetCapacity()) ? true : false;
		if (count > 0.8 * GetCapacity())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

template <typename Tkey, typename Tvalue>
HashTable<Tkey, Tvalue>& HashTable<Tkey, Tvalue>::operator=(const HashTable<Tkey, Tvalue>& other)
{
	this->hashTable = other.hashTable;

	delete this->hasher;
	this->hashTable = other.hasher->Copy();

	this->count = other.count;

	return *this;
}

template <typename Tkey, typename Tvalue>
HashTable<Tkey, Tvalue>& HashTable<Tkey, Tvalue>::operator=(HashTable<Tkey, Tvalue>&& other)
{
	this->hashTable = std::move(other.hashTable);

	this->hashTable = other.hasher->Copy();
	other.hasher = nullptr;

	this->count = other.count;
	other.count = 0;

	return *this;
}

template <typename Tkey, typename Tvalue>
IDict<Tkey, Tvalue>* HashTable<Tkey, Tvalue>::Add(Pair<Tkey, Tvalue> record)
{
	if (hashTable.isEmpty())
	{
		hashTable.Realloc(DEFAULT_SIZE);
	}

	if (ContainsKey(record.GetLeft()))
	{
		throw std::invalid_argument("Key already is in table");
	}

	if (NeedToReconstruct())
	{
		AssociativeArray newTable;
		newTable.Realloc(size_t(hashTable.GetLength() * 2));

		for (CollisionList& list : hashTable)
		{
			for (Pair<Tkey, Tvalue>& record : list)
			{
				newTable[hasher->Hash(record.GetLeft()) % newTable.GetLength()].Append(record);
			}
		}

		this->hashTable = std::move(newTable);
	}

	hashTable[Hash(record.GetLeft())].Append(record);
	this->count++;

	return this;
}

template <typename Tkey, typename Tvalue>
void HashTable<Tkey, Tvalue>::Remove(const Tkey& key)
{
	if (!ContainsKey(key))
	{
		throw std::out_of_range("No key was found");
	}

	CollisionList& list = hashTable[Hash(key)];

	for (int i = 0; i < list.GetLength(); i++)
	{
		if (list[i].GetLeft() == key)
		{
			list.Remove(i);
			break;
		}
	}
	this->count--;
}

template <typename Tkey, typename Tvalue>
Tvalue HashTable<Tkey, Tvalue>::Get(const Tkey& key)
{
	CollisionList& list = hashTable[Hash(key)];

	for (Pair<Tkey, Tvalue>& record : list)
	{
		if (record.GetLeft() == key)
		{
			return record.GetRight();
		}
	}

	throw std::out_of_range("Value was not found");
}

template <typename Tkey, typename Tvalue>
Tvalue& HashTable<Tkey, Tvalue>::GetMut(const Tkey& key)
{
	CollisionList& list = hashTable[Hash(key)];

	for (Pair<Tkey, Tvalue>& record : list)
	{
		if (record.GetLeft() == key)
		{
			// return record.GetRight();
			return record.right;
		}
	}

	throw std::out_of_range("Value was not found");
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey, Tvalue>::ContainsKey(const Tkey& key) const noexcept
{
	CollisionList& list = hashTable[Hash(key)];

	for (Pair<Tkey, Tvalue>& record : list)
	{
		if (record.GetLeft() == key)
		{
			return true;
		}
	}

	return false;
}

template <typename Tkey, typename Tvalue>
size_t HashTable<Tkey, Tvalue>::GetCount() const noexcept
{
	return this->count;
}

template <typename Tkey, typename Tvalue>
size_t HashTable<Tkey, Tvalue>::GetCapacity() const noexcept
{
	return this->hashTable.GetLength();
}
