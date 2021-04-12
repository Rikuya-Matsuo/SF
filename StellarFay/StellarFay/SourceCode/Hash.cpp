#include "Hash.h"

size_t Hash::_Fnv1a(void * bytes, size_t sizeOf, size_t length)
{
	const uint8_t sizetSize = sizeof(size_t);
	size_t hash = (sizetSize == 4) ? FnvBasis32 : FnvBasis64;
	const size_t prime = (sizetSize == 4) ? FnvPrime32 : FnvPrime64;

	for (size_t i = 0; i < length; ++i)
	{
		size_t v =
			(sizeOf == 1) ? reinterpret_cast<uint8_t*>(bytes)[i] :
			(sizeOf == 2) ? reinterpret_cast<uint16_t*>(bytes)[i] :
			(sizeOf == 4) ? reinterpret_cast<uint32_t*>(bytes)[i] :
			reinterpret_cast<uint64_t*>(bytes)[i];
		hash = hash ^ v;
		hash = hash * prime;
	}

	return hash;
}

size_t Hash::Fnv1a(uint8_t * bytes, size_t length)
{
	return _Fnv1a(bytes, sizeof(uint8_t), length);
}

size_t Hash::Fnv1a(uint16_t * bytes, size_t length)
{
	return _Fnv1a(bytes, sizeof(uint16_t), length);
}

size_t Hash::Fnv1a(uint32_t * bytes, size_t length)
{
	return _Fnv1a(bytes, sizeof(uint32_t), length);
}

size_t Hash::Fnv1a(uint64_t * bytes, size_t length)
{
	return _Fnv1a(bytes, sizeof(uint64_t), length);
}

size_t Hash::_Fnv1(void * bytes, size_t sizeOf, size_t length)
{
	const uint8_t sizetSize = sizeof(size_t);
	size_t hash = (sizetSize == 4) ? FnvBasis32 : FnvBasis64;
	const size_t prime = (sizetSize == 4) ? FnvPrime32 : FnvPrime64;

	for (size_t i = 0; i < length; ++i)
	{
		size_t v =
			(sizeOf == 1) ? reinterpret_cast<uint8_t*>(bytes)[i] :
			(sizeOf == 2) ? reinterpret_cast<uint16_t*>(bytes)[i] :
			(sizeOf == 4) ? reinterpret_cast<uint32_t*>(bytes)[i] :
			reinterpret_cast<uint64_t*>(bytes)[i];
		hash = hash * prime;
		hash = hash ^ v;
	}

	return hash;
}

size_t Hash::Fnv1(uint8_t * bytes, size_t length)
{
	return _Fnv1(bytes, sizeof(uint8_t), length);
}

size_t Hash::Fnv1(uint16_t * bytes, size_t length)
{
	return _Fnv1(bytes, sizeof(uint16_t), length);
}

size_t Hash::Fnv1(uint32_t * bytes, size_t length)
{
	return _Fnv1(bytes, sizeof(uint32_t), length);
}

size_t Hash::Fnv1(uint64_t * bytes, size_t length)
{
	return _Fnv1(bytes, sizeof(uint64_t), length);
}
