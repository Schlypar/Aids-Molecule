#pragma once

#include "IHasher.h"

template <typename Tkey>
class PtrHasher : public IHasher<Tkey>
{
public:
	size_t Hash(const Tkey& key) const noexcept override;

	IHasher<Tkey>* Copy() const noexcept override;
};

template <typename Tkey>
size_t PtrHasher<Tkey>::Hash(const Tkey& key) const noexcept
{
	return (size_t) (key) >> 3;
}

template <typename Tkey>
IHasher<Tkey>* PtrHasher<Tkey>::Copy() const noexcept
{
	return new PtrHasher<Tkey>();
}
