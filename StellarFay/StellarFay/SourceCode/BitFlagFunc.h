#pragma once

namespace BitFlagFunc
{
	// maskが示すビットが１つでも真であれば真を返す
	// ちなみにGetAnd()と比較して、こちらの方が処理が簡易であるため、高速と思われる
	template<typename T>
	inline bool GetOr(T flags, T mask)
	{
		return (flags & mask);
	}

	// maskが示すビットが全て真である場合真を返す
	// GetOr()でもいいケースにおいては、GetOr()を推奨
	template<typename T>
	inline bool GetAnd(T flags, T mask)
	{
		return ((flags & mask) == mask);
	}

	// valueが真であればmaskのビットを立て、偽であれば下す
	template<typename T>
	inline void Set(T & flags, T mask, bool value)
	{
		(value) ?
			(flags |= mask) :
			(flags &= ~mask);
	}
}

#define BIT_SHIFT(maskIdx) 1 << maskIdx

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
