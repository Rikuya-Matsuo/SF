#include "RandomGenerator.h"

RandomGenerator::RandomGenerator(Uint32 seed) :
	mRand(seed)
{
}

int RandomGenerator::RangeI(int min, int max)
{
	std::uniform_int_distribution<> range(min, max);
	return range(mRand);
}

float RandomGenerator::RangeF(float min, float max)
{
	std::uniform_real_distribution<float> range(min, max);
	return range(mRand);
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
