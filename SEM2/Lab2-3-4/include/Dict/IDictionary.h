#pragma once

#include "Pair.h"

template <typename Tkey, typename Tvalue>
class IDict
{
public:
	virtual size_t GetCount() const noexcept = 0;

	virtual size_t GetCapacity() const noexcept = 0;

	virtual Tvalue Get(const Tkey& key) = 0;

	virtual Tvalue& GetMut(const Tkey& key) = 0;

	virtual bool ContainsKey(const Tkey& key) const noexcept = 0;

	virtual IDict<Tkey, Tvalue>* Add(Pair<Tkey, Tvalue> record) = 0;

	virtual void Remove(const Tkey& key) = 0;
};
