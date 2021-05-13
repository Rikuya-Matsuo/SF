#pragma once
#include <memory.h>
#include "SDL/SDL.h"
#include "Vector.h"

class Matrix3
{
public:
	float mat[3][3];

	Matrix3()
	{
		*this = Matrix3::Identity;
	}

	Matrix3(float _mat[3][3])
	{
		rsize_t size = 3 * 3 * sizeof(float);
		memcpy_s(mat, size, _mat, size);
	}

	const float * GetAsFloatPtr() const
	{
		return reinterpret_cast<const float *>(&mat[0][0]);
	}

	friend Matrix3 operator*(const Matrix3 & lhs, const Matrix3 & rhs)
	{
		Matrix3 ret;

		auto calcElem = [&ret, &lhs, &rhs](Uint8 xIndex, Uint8 yIndex)
		{
			ret.mat[yIndex][xIndex] =
				(lhs.mat[yIndex][0] * rhs.mat[0][xIndex]) +
				(lhs.mat[yIndex][1] * rhs.mat[1][xIndex]) +
				(lhs.mat[yIndex][2] * rhs.mat[2][xIndex]);
		};

		for (Uint8 y = 0; y < 3; ++y)
		{
			for (Uint8 x = 0; x < 3; ++x)
			{
				calcElem(x, y);
			}
		}

		return ret;
	}

	Matrix3 & operator*=(const Matrix3 & rm)
	{
		return (*this = (*this * rm));
	}

	friend std::ostream & operator<<(std::ostream & stream, const Matrix3 & m);

	float CalculateDeterminant() const;

	static Matrix3 CreateScale(float xScale, float yScale)
	{
		float tmp[3][3] =
		{
			{xScale, 0.0f, 0.0f},
			{0.0f, yScale, 0.0f},
			{0.0f, 0.0f, 1.0f}
		};

		return Matrix3(tmp);
	}

	static Matrix3 CreateScale(const Vector2D & scaleVec)
	{
		return CreateScale(scaleVec.x, scaleVec.y);
	}

	static Matrix3 CreateScale(float scale)
	{
		return CreateScale(scale, scale);
	}

	static Matrix3 CreateRotation(float theta)
	{
		float tmp[3][3] =
		{
			{cosf(theta), sinf(theta), 0.0f},
			{-sinf(theta), cosf(theta), 0.0f},
			{0.0f, 0.0f, 1.0f}
		};

		return Matrix3(tmp);
	}

	static Matrix3 CreateTranslation(const Vector2D & trans)
	{
		float tmp[3][3] =
		{
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{trans.x, trans.y, 1.0f}
		};

		return Matrix3(tmp);
	}

	static const Matrix3 Identity;
};

class Matrix4
{
public:
	float mat[4][4];

	Matrix4()
	{
		*this = Matrix4::Identity;
	}

	Matrix4(float _mat[4][4])
	{
		rsize_t size = 4 * 4 * sizeof(float);
		memcpy_s(mat, size, _mat, size);
	}

	const float * GetAsFloatPtr() const
	{
		return reinterpret_cast<const float *>(&mat[0][0]);
	}

	friend Matrix4 operator*(const Matrix4 & lhs, const Matrix4 & rhs)
	{
		Matrix4 ret;

		auto calcElem = [&ret, &lhs, &rhs](Uint8 xIndex, Uint8 yIndex)
		{
			ret.mat[yIndex][xIndex] =
				(lhs.mat[yIndex][0] * rhs.mat[0][xIndex]) +
				(lhs.mat[yIndex][1] * rhs.mat[1][xIndex]) +
				(lhs.mat[yIndex][2] * rhs.mat[2][xIndex]) +
				(lhs.mat[yIndex][3] * rhs.mat[3][xIndex]);
		};

		for (Uint8 y = 0; y < 4; ++y)
		{
			for (Uint8 x = 0; x < 4; ++x)
			{
				calcElem(x, y);
			}
		}

		return ret;
	}

	Matrix4 & operator*=(const Matrix4 & rm)
	{
		return (*this = (*this * rm));
	}

	friend std::ostream & operator<<(std::ostream & stream, const Matrix4 m);

	// 挙動重いかも。
	float CalculateDeterminant() const;

	// 挙動重いかも。
	bool Invert();

	Vector3D GetTranslation() const
	{
		return Vector3D(mat[3][0], mat[3][1], mat[3][2]);
	}

	Vector3D GetXAxis() const
	{
		return Vector3D(mat[0][0], mat[0][1], mat[0][2]);
	}

	Vector3D GetYAxis() const
	{
		return Vector3D(mat[1][0], mat[1][1], mat[1][2]);
	}

	Vector3D GetZAxis() const
	{
		return Vector3D(mat[2][0], mat[2][1], mat[2][2]);
	}

	static Matrix4 CreateScale(float xScale, float yScale, float zScale)
	{
		float tmp[4][4] =
		{
			{xScale, 0.0f, 0.0f, 0.0f},
			{0.0f, yScale, 0.0f, 0.0f},
			{0.0f, 0.0f, zScale, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};

		return Matrix4(tmp);
	}

	static Matrix4 CreateScale(const Vector3D &	scaleVec)
	{
		return CreateScale(scaleVec.x, scaleVec.y, scaleVec.z);
	}

	static Matrix4 CreateScale(float scale)
	{
		return CreateScale(scale, scale, scale);
	}

	static Matrix4 CreateRotationX(float theta)
	{
		float tmp[4][4] =
		{
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, cosf(theta), sinf(theta), 0.0f},
			{0.0f, -sinf(theta), cosf(theta), 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};

		return Matrix4(tmp);
	}

	static Matrix4 CreateRotationY(float theta)
	{
		float tmp[4][4] =
		{
			{cosf(theta), 0.0f, -sinf(theta), 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{sinf(theta), 0.0f, cosf(theta), 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};

		return Matrix4(tmp);
	}

	static Matrix4 CreateRotationZ(float theta)
	{
		float tmp[4][4] =
		{
			{cosf(theta), sinf(theta), 0.0f, 0.0f},
			{-sinf(theta), cosf(theta), 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};

		return Matrix4(tmp);
	}

	static Matrix4 CreateFromQuaternion(const class Quaternion & q);

	static Matrix4 CreateTranslation(const Vector3D & trans)
	{
		float tmp[4][4] =
		{
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{trans.x, trans.y, trans.z, 1.0f}
		};

		return Matrix4(tmp);
	}

	static Matrix4 CreateLookAt(const Vector3D & eye, const Vector3D & target, const Vector3D & up)
	{
		Vector3D zAxis = Vector3D::Normalize(target - eye);
		Vector3D xAxis = Vector3D::Normalize(Vector3D::Cross(up, zAxis));
		Vector3D yAxis = Vector3D::Normalize(Vector3D::Cross(zAxis, xAxis));
		Vector3D trans;
		trans.x = -Vector3D::Dot(xAxis, eye);
		trans.y = -Vector3D::Dot(yAxis, eye);
		trans.z = -Vector3D::Dot(zAxis, eye);

		float tmp[4][4] =
		{
			{xAxis.x, yAxis.x, zAxis.x, 0.0f},
			{xAxis.y, yAxis.y, zAxis.y, 0.0f},
			{xAxis.z, yAxis.z, zAxis.z, 0.0f},
			{trans.x, trans.y, trans.z, 1.0f}
		};
		return Matrix4(tmp);
	}

	static Matrix4 CreateOrtho(float width, float height, float near, float far)
	{
		float tmp[4][4] =
		{
			{2.0f / width, 0.0f, 0.0f, 0.0f},
			{0.0f, 2.0f / height, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f / (far - near), 0.0f},
			{0.0f, 0.0f, near / (near - far), 1.0f}
		};

		return Matrix4(tmp);
	}

	static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
	{
		float yScale = 1.0f / tanf(fovY / 2.0f);
		float xScale = yScale * height / width;

		float tmp[4][4] =
		{
			{xScale, 0.0f, 0.0f, 0.0f},
			{0.0f, yScale, 0.0f, 0.0f},
			{0.0f, 0.0f, far / (far - near), 1.0f},
			{0.0f, 0.0f, -near * far / (far - near), 0.0f}
		};

		return Matrix4(tmp);
	}

	static Matrix4 CreateSimpleViewProj(float width, float height)
	{
		float tmp[4][4] =
		{
			{2.0f / width, 0.0f, 0.0f, 0.0f},
			{0.0f, 2.0f / height, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 1.0f}
		};

		return Matrix4(tmp);
	}

	static const Matrix4 Identity;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
