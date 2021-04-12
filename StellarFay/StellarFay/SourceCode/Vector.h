#pragma once
#include <cmath>

class Vector2D
{
public:
	float x;
	float y;

	Vector2D() :
		x(0.0f),
		y(0.0f)
	{}
	
	Vector2D(float _x, float _y) :
		x(_x),
		y(_y)
	{}

	Vector2D(const Vector2D & v) :
		x(v.x),
		y(v.y)
	{}

	friend Vector2D operator+(const Vector2D & lhs, const Vector2D & rhs)
	{
		return Vector2D(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	Vector2D & operator+=(const Vector2D & rv)
	{
		x += rv.x;
		y += rv.y;
		return *this;
	}

	friend Vector2D operator-(const Vector2D & lhs, const Vector2D & rhs)
	{
		return Vector2D(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	Vector2D & operator-=(const Vector2D & rv)
	{
		x -= rv.x;
		y -= rv.y;
		return *this;
	}

	friend Vector2D operator*(const Vector2D & lhs, const Vector2D & rhs)
	{
		return Vector2D(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	friend Vector2D operator*(const Vector2D & v, float f)
	{
		return Vector2D(v.x * f, v.y * f);
	}

	friend Vector2D operator*(float f, const Vector2D & v)
	{
		return v * f;
	}

	Vector2D & operator*=(float f)
	{
		x *= f;
		y *= f;
		return *this;
	}

	float LengthSq() const
	{
		return (x * x + y * y);
	}

	float Length() const
	{
		return (sqrt(LengthSq()));
	}

	void Normalize()
	{
		float len = Length();
		x /= len;
		y /= len;
	}

	static Vector2D Normalize(const Vector2D & v)
	{
		Vector2D tmp = v;
		tmp.Normalize();
		return tmp;
	}

	static float Dot(const Vector2D & v0, const Vector2D & v1)
	{
		return (v0.x * v1.x + v0.y * v1.y);
	}

	static Vector2D Lerp(const Vector2D & v, const Vector2D & goal, float rate)
	{
		return Vector2D(v + (goal - v) * rate);
	}

	static Vector2D Reflect(const Vector2D & v, const Vector2D & n)
	{
		return v - 2.0f * Vector2D::Dot(v, n) * n;
	}

	static Vector2D Transform(const Vector2D &v, const class Matrix3 & mat, float w = 1.0f);

	static const Vector2D Zero;
	static const Vector2D UnitX;
	static const Vector2D UnitY;
	static const Vector2D NegUnitX;
	static const Vector2D NegUnitY;
};

class Vector3D
{
public:
	float x;
	float y;
	float z;

	Vector3D() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{}

	Vector3D(float _x, float _y, float _z) :
		x(_x),
		y(_y),
		z(_z)
	{}

	Vector3D(const Vector3D & v) :
		x(v.x),
		y(v.y),
		z(v.z)
	{}

	const float * GetAsFloatPtr() const
	{
		return reinterpret_cast<const float *>(&x);
	}

	friend Vector3D operator+(const Vector3D & lhs, const Vector3D & rhs)
	{
		return Vector3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	Vector3D & operator+=(const Vector3D & rv)
	{
		x += rv.x;
		y += rv.y;
		z += rv.z;
		return *this;
	}

	friend Vector3D operator-(const Vector3D & lhs, const Vector3D & rhs)
	{
		return Vector3D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	Vector3D & operator-=(const Vector3D & rv)
	{
		x -= rv.x;
		y -= rv.y;
		z -= rv.z;
		return *this;
	}

	friend Vector3D operator*(const Vector3D & lhs, const Vector3D & rhs)
	{
		return Vector3D(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	friend Vector3D operator*(const Vector3D & v, float f)
	{
		return Vector3D(v.x * f, v.y * f, v.z * f);
	}

	friend Vector3D operator*(float f, const Vector3D & v)
	{
		return v * f;
	}

	Vector3D & operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	float LengthSq() const
	{
		return (x * x + y * y + z * z);
	}

	float Length() const
	{
		return (sqrt(LengthSq()));
	}

	void Normalize()
	{
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
	}

	static Vector3D Normalize(const Vector3D & v)
	{
		Vector3D tmp = v;
		tmp.Normalize();
		return tmp;
	}

	static float Dot(const Vector3D & v0, const Vector3D & v1)
	{
		return (v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
	}

	static Vector3D Cross(const Vector3D & v0, const Vector3D & v1)
	{
		Vector3D tmp;
		tmp.x = v0.y * v1.z - v0.z * v1.y;
		tmp.y = v0.z * v1.x - v0.x * v1.z;
		tmp.z = v0.x * v1.y - v0.y * v1.x;
		return tmp;
	}

	static Vector3D Lerp(const Vector3D & v, const Vector3D & goal, float rate)
	{
		return Vector3D(v + (goal - v) * rate);
	}

	static Vector3D Reflect(const Vector3D & v, const Vector3D & n)
	{
		return v - 2.0f * Vector3D::Dot(v, n) * n;
	}

	static Vector3D Transform(const Vector3D & v, const class Matrix4 & mat, float w = 1.0f);

	static Vector3D TransformWithPerspDiv(const Vector3D & v, const class Matrix4 & mat, float w = 1.0f);

	//static Vector3D Transform(const Vector3D & v, const class Quaternion & q);

	static const Vector3D Zero;
	static const Vector3D UnitX;
	static const Vector3D UnitY;
	static const Vector3D UnitZ;
	static const Vector3D NegUnitX;
	static const Vector3D NegUnitY;
	static const Vector3D NegUnitZ;

	// 必要になったときに実装する
	//static const Vector3D Infinity;
	//static const Vector3D NegInfinity;
};
