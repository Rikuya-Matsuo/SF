#pragma once
#include "Vector.h"

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion()
	{
		*this = Identity;
	}

	Quaternion(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	Quaternion(const Vector3D & axis, float angle)
	{
		float scalar = sinf(angle / 2.0f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = cosf(angle / 2.0f);
	}

	static const Quaternion Identity;
};
