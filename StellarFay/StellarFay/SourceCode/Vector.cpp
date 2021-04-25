#include "Vector.h"
#include "Matrix.h"
#include <limits>
#include <iomanip>

const Vector2D Vector2D::Zero(0.0f, 0.0f);
const Vector2D Vector2D::UnitX(1.0f, 0.0f);
const Vector2D Vector2D::UnitY(0.0f, 1.0f);
const Vector2D Vector2D::NegUnitX(-1.0f, 0.0f);
const Vector2D Vector2D::NegUnitY(0.0f, -1.0f);

const Vector3D Vector3D::Zero(0.0f, 0.0f, 0.0f);
const Vector3D Vector3D::UnitX(1.0f, 0.0f, 0.0f);
const Vector3D Vector3D::UnitY(0.0f, 1.0f, 0.0f);
const Vector3D Vector3D::UnitZ(0.0f, 0.0f, 1.0f);
const Vector3D Vector3D::NegUnitX(-1.0f, 0.0f, 0.0f);
const Vector3D Vector3D::NegUnitY(0.0f, -1.0f, 0.0f);
const Vector3D Vector3D::NegUnitZ(0.0f, 0.0f, -1.0f);

Vector2D Vector2D::Transform(const Vector2D & v, const Matrix3 & mat, float w)
{
	Vector2D ret;
	ret.x = v.x * mat.mat[0][0] + v.y * mat.mat[1][0] + w * mat.mat[2][0];
	ret.y = v.x * mat.mat[0][1] + v.y * mat.mat[1][1] + w * mat.mat[2][1];

	return ret;
}

Vector3D Vector3D::Transform(const Vector3D & v, const Matrix4 & mat, float w)
{
	Vector3D ret;
	ret.x = v.x * mat.mat[0][0] + v.y * mat.mat[1][0] +
		v.z * mat.mat[2][0] + w * mat.mat[3][0];
	ret.y = v.x * mat.mat[0][1] + v.y * mat.mat[1][1] +
		v.z * mat.mat[2][1] + w * mat.mat[3][1];
	ret.z = v.x * mat.mat[0][2] + v.y * mat.mat[1][2] +
		v.z * mat.mat[2][2] + w * mat.mat[3][2];

	return ret;
}

Vector3D Vector3D::TransformWithPerspDiv(const Vector3D & v, const Matrix4 & mat, float w)
{
	Vector3D ret;
	ret.x = v.x * mat.mat[0][0] + v.y * mat.mat[1][0] +
		v.z * mat.mat[2][0] + w * mat.mat[3][0];
	ret.y = v.x * mat.mat[0][1] + v.y * mat.mat[1][1] +
		v.z * mat.mat[2][1] + w * mat.mat[3][1];
	ret.z = v.x * mat.mat[0][2] + v.y * mat.mat[1][2] +
		v.z * mat.mat[2][2] + w * mat.mat[3][2];
	
	float transformedW = v.x * mat.mat[0][3] + v.y * mat.mat[1][3] +
		v.z * mat.mat[2][3] + w * mat.mat[3][3];

	static const float nearZeroLine = 0.001f;
	if (fabsf(transformedW) > nearZeroLine)
	{
		transformedW = 1.0f / transformedW;
		ret *= transformedW;
	}
	
	return ret;
}

std::ostream & operator<<(std::ostream & stream, const Vector2D & v)
{
	stream << std::fixed << std::setprecision(2) <<
		"(" <<
		std::setw(10) << v.x << ", " <<
		std::setw(10) << v.y << ")";

	return stream;
}

std::ostream & operator<<(std::ostream & stream, const Vector3D & v)
{
	stream << std::fixed << std::setprecision(2) <<
		"(" <<
		std::setw(10) << v.x << ", " <<
		std::setw(10) << v.y << ", " <<
		std::setw(10) << v.z << ")";

	return stream;
}
