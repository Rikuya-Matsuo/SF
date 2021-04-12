#include "Matrix.h"

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
	return Matrix4();
}
