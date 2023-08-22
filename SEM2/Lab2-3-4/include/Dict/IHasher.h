#pragma once

#include <cstddef>

template <typename Tkey>
class IHasher
{
public:
	virtual size_t Hash(const Tkey& key) const noexcept = 0;
};
