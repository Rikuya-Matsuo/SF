#include "Matrix.h"
#include "Quaternion.h"
#include <iomanip>

static float mat3Ident[3][3] =
{
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f}
};
const Matrix3 Matrix3::Identity(mat3Ident);

float Matrix3::CalculateDeterminant() const
{
	// サラスの方法より
	float ret;

	ret =
		(mat[0][0] * mat[1][1] * mat[2][2])
		+ (mat[1][0] * mat[2][1] * mat[0][2])
		+ (mat[2][0] * mat[0][1] * mat[1][2])
		- (mat[0][2] * mat[1][1] * mat[2][0])
		- (mat[1][2] * mat[2][1] * mat[0][0])
		- (mat[2][2] * mat[0][1] * mat[1][0]);

	return ret;
}

float Matrix4::CalculateDeterminant() const
{
	// 余因子展開を使う
	float ret = 0;

	Matrix3 bufMat3;

	for (int i = 0; i < 4; ++i)
	{
		// 余因子を作成
		int bufIdxCount = 0;
		for (int bufIdxI = 0; bufIdxI < 4; ++bufIdxI)
		{
			if (bufIdxI == i)
			{
				continue;
			}

			for (int bufIdxJ = 0; bufIdxJ < 4; ++bufIdxJ)
			{
				if (bufIdxJ == 0)
				{
					continue;
				}

				bufMat3.mat[bufIdxCount / 3][bufIdxCount % 3] = mat[bufIdxI][bufIdxJ];
				bufIdxCount++;
			}
		}

		char sign = (i % 2) ? -1 : 1;

		ret += sign * mat[i][0] * bufMat3.CalculateDeterminant();
	}

	return ret;
}

bool Matrix4::Invert()
{
	// 行列式を計算
	float det = CalculateDeterminant();

	// もし行列式が０である場合、逆行列は存在しない
	if (!det)
	{
		return false;
	}

	Matrix3 bufMat3;

	// 余因子行列
	float adjugateMat[4][4];

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			// 余因子行列を作成
			int bufIdxCount = 0;
			for (int bufIdxI = 0; bufIdxI < 4; ++bufIdxI)
			{
				if (bufIdxI == i)
				{
					continue;
				}

				for (int bufIdxJ = 0; bufIdxJ < 4; ++bufIdxJ)
				{
					if (bufIdxJ == j)
					{
						continue;
					}

					bufMat3.mat[bufIdxCount / 3][bufIdxCount % 3] = mat[bufIdxI][bufIdxJ];
					bufIdxCount++;
				}
			}

			char sign = ((i + j) % 2) ? -1 : 1;

			adjugateMat[j][i] = sign * bufMat3.CalculateDeterminant();
		}
	}

	// 逆行列の公式より
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat[i][j] = adjugateMat[i][j] / det;
		}
	}

	return true;
}

static float mat4Ident[4][4] =
{
	{1.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f}
};
const Matrix4 Matrix4::Identity(mat4Ident);

Matrix4 Matrix4::CreateFromQuaternion(const Quaternion & q)
{
	float mat[4][4];

	mat[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	mat[0][1] = 2.0f * q.x * q.y + 2.0f * q.z * q.w;
	mat[0][2] = 2.0f * q.x * q.z - 2.0f * q.y * q.w;
	mat[0][3] = 0.0f;

	mat[1][0] = 2.0f * q.y * q.x - 2.0f * q.z * q.w;
	mat[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	mat[1][2] = 2.0f * q.y * q.z + 2.0f * q.x * q.w;
	mat[1][3] = 0.0f;

	mat[2][0] = 2.0f * q.z * q.x - 2.0f * q.y * q.w;
	mat[2][1] = 2.0f * q.z * q.y - 2.0f * q.x * q.w;
	mat[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
	mat[2][3] = 0.0f;

	mat[3][0] = mat[3][1] = mat[3][2] = 0.0f;
	mat[3][3] = 1.0f;

	return Matrix4(mat);
}

std::ostream & operator<<(std::ostream & stream, const Matrix3 & m)
{
	for (int i = 0; i < 3; ++i)
	{
		if (i)
		{
			stream << '\n';
		}

		for (int j = 0; j < 3; ++j)
		{
			stream << std::fixed << std::setprecision(3) << std::setw(6) << m.mat[i][j];
		}
	}

	return stream;
}

std::ostream & operator<<(std::ostream & stream, const Matrix4 m)
{
	for (int i = 0; i < 4; ++i)
	{
		if (i)
		{
			stream << '\n';
		}

		for (int j = 0; j < 4; ++j)
		{
			stream << std::fixed << std::setprecision(3) << std::setw(6) << m.mat[i][j];

		}
	}

	return stream;
}
