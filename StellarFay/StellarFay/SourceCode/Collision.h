#pragma once
#include "Vector.h"

// 共通の基底クラスを持たせるためのクラス
struct CollisionShapeBase
{
	virtual bool IsPointInside(const Vector3D & point) const = 0;
};

struct AABB : public CollisionShapeBase
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

	bool IsPointInside(const Vector3D & point) const override;

private:
	bool mSetFlag = false;
};

struct Sphere : public CollisionShapeBase
{
	// 中心の座標
	Vector3D mCenter;
	
	// 半径
	float mRadius;

	bool IsPointInside(const Vector3D & point) const override;
};

bool CheckHit(const AABB & box1, const AABB & box2);

bool CheckHit(const Sphere & s1, const Sphere & s2);
