#pragma once
#include <cstdint>

namespace Hash
{
	static const uint32_t FnvBasis32 = 2166136261u;
	static const uint64_t FnvBasis64 = 14695981039346656037u;

	static const uint32_t FnvPrime32 = 16777619u;
	static const uint64_t FnvPrime64 = 1099511628211ull;

	size_t _Fnv1a(void * bytes, size_t sizeOf, size_t length);

	size_t Fnv1a(uint8_t * bytes, size_t length);

	size_t Fnv1a(uint16_t * bytes, size_t length);

	size_t Fnv1a(uint32_t * bytes, size_t length);

	size_t Fnv1a(uint64_t * bytes, size_t length);

	size_t _Fnv1(void * bytes, size_t sizeOf, size_t length);

	size_t Fnv1(uint8_t * bytes, size_t length);

	size_t Fnv1(uint16_t * bytes, size_t length);

	size_t Fnv1(uint32_t * bytes, size_t length);

	size_t Fnv1(uint64_t * bytes, size_t length);
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
