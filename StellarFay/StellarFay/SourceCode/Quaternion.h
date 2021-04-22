#pragma once
#include "Vector.h"
#include "CommonMath.h"

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

	float LengthSq() const
	{
		return (x * x + y * y + z * z + w * w);
	}

	float Length() const
	{
		return sqrtf(LengthSq());
	}

	void Normalize()
	{
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}

	static Quaternion Normalize(const Quaternion & q)
	{
		Quaternion ret = q;
		ret.Normalize();
		return ret;
	}

	// 線形補完
	static Quaternion Lerp(const Quaternion & a, const Quaternion & b, float rate)
	{
		Quaternion ret;
		ret.x = LERP(a.x, b.x, rate);
		ret.y = LERP(a.y, b.y, rate);
		ret.z = LERP(a.z, b.z, rate);
		ret.w = LERP(a.w, b.w, rate);
		ret.Normalize();

		return ret;
	}

	static float Dot(const Quaternion & q0, const Quaternion & q1)
	{
		return (q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w);
	}

	// 球面線形補完
	static Quaternion Slerp(const Quaternion & a, const Quaternion & b, float rate)
	{
		return Quaternion();
	}

	static const Quaternion Identity;
};
