#pragma once
#include "Vector.h"
#include "CommonMath.h"

// 四元数
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

	bool operator==(const Quaternion & rhs) const
	{
		return
			(this->x == rhs.x) &&
			(this->y == rhs.y) &&
			(this->z == rhs.z) &&
			(this->w == rhs.w);
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
		// 回転を示す四元数の長さは１なので、内積からcosθを求める
		float rawCos = Quaternion::Dot(a, b);

		// rawCosの絶対値を記録
		// 回転を示す四元数は符号が反転していても同じ回転を示す。
		// rawCosが負の場合は片方の符号が反転しているため、cosの方を合わせる
		float cos = fabsf(rawCos);

		// 球面線形補間の式における、各四元数にかかる係数
		float scaleA, scaleB;

		// 係数の計算
		// 2つの回転が同一直線上にない場合
		if (cos < 1.0f)
		{
			const float theta = acosf(cos);
			const float invSin = 1.0f / sinf(theta);

			scaleA = sinf((1.0f - rate) * theta) * invSin;
			scaleB = sinf(rate * theta) * invSin;
		}
		// 同一直線上にある場合
		else
		{
			// 線形補完を使う
			scaleA = 1.0f - rate;
			scaleB = rate;
		}

		if (rawCos < 0.0f)
		{
			scaleB *= -1;
		}

		Quaternion ret;
		ret.x = scaleA * a.x + scaleB * b.x;
		ret.y = scaleA * a.y + scaleB * b.y;
		ret.z = scaleA * a.z + scaleB * b.z;
		ret.w = scaleA * a.w + scaleB * b.w;
		ret.Normalize();
		return ret;
	}

	static const Quaternion Identity;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
