#pragma once
#define MATH_PI 3.1415926535f
#define DEG_TO_RAD(degrees)		((MATH_PI / 180.0f) * degrees)
#define RAD_TO_DEG(rad)			((rad / MATH_PI) * 180.0f)
#define LERP(a, b, rate)		(a + (b - a) * rate)
#define LARGER(a, b)			((a > b) ? a : b)
#define SMALLER(a, b)			((a < b) ? a : b)

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
