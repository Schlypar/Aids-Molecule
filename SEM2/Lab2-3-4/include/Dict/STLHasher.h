#pragma once

#include "IHasher.h"
#include <functional>

template <typename Tkey>
class STLHasher : public IHasher<Tkey>
{
public:
	size_t Hash(const Tkey& key) const noexcept override;

	IHasher<Tkey>* Copy() const noexcept override;
};

template <typename Tkey>
size_t STLHasher<Tkey>::Hash(const Tkey& key) const noexcept
{
	return std::hash<Tkey> {}(key);
}

template <typename Tkey>
IHasher<Tkey>* STLHasher<Tkey>::Copy() const noexcept
{
	return new STLHasher<Tkey>();
}
