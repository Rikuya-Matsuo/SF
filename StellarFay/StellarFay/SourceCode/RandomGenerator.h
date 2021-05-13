#pragma once
#include "SDL/SDL.h"
#include <random>
#include <climits>

class RandomGenerator
{
public:
	// 種をランダムで設定
	RandomGenerator() : RandomGenerator(std::random_device()()) {}

	// 種を指定
	RandomGenerator(Uint32 seed);

	// 乱数の生成
	Uint32 Gen() { return mRand(); }

	// 範囲(int)
	int RangeI(int min = 0, int max = INT_MAX);

	// 範囲(float)
	float RangeF(float min = 0.0f, float max = 100.0f);

private:
	// 乱数生成機本体
	std::mt19937 mRand;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
