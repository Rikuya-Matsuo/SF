#pragma once
#include "Vector.h"

struct AABB
{
	Vector3D mMax;
	Vector3D mMin;

	// 頂点の更新
	void UpdateVertex(const Vector3D & vpos)
	{
		if (!mSetFlag)
		{
			mMax = mMin = vpos;
			mSetFlag = true;
		}
		else
		{
			mMax.x = (vpos.x > mMax.x) ? vpos.x : mMax.x;
			mMax.y = (vpos.y > mMax.y) ? vpos.y : mMax.y;
			mMax.z = (vpos.z > mMax.z) ? vpos.z : mMax.z;

			mMin.x = (vpos.x < mMin.x) ? vpos.x : mMin.x;
			mMin.y = (vpos.y < mMin.y) ? vpos.y : mMin.y;
			mMin.z = (vpos.z < mMin.z) ? vpos.z : mMin.z;
		}
	}

	// 各軸の長さを算出
	Vector3D CalculateSize() const
	{
		return mMax - mMin;
	}

	// 体積を算出
	float CalculateVolume() const
	{
		Vector3D size = CalculateSize();
		return size.x * size.y * size.z;
	}

private:
	bool mSetFlag = false;
};
