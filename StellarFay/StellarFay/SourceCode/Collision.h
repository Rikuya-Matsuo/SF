#pragma once
#include "Vector.h"

struct AABB
{
	Vector3D mMax;
	Vector3D mMin;

	bool mSetFlag = false;

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
};
