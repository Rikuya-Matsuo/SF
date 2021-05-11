#pragma once
#include "Vector.h"
#include <SDL/SDL.h>

enum CollisionShapeEnum : Uint8
{
	AABB_CollisionShapeEnum = 0,
	Sphere_CollisionShapeEnum,
	Invalid_CollisionShapeEnum
};

// コリジョン形状の基底クラス
struct CollisionShapeBase
{
	CollisionShapeBase(CollisionShapeEnum shape) :
		mShape(shape)
	{}

	const CollisionShapeEnum mShape;

	virtual bool IsPointInside(const Vector3D & point) const = 0;
};

struct AABB : public CollisionShapeBase
{
	AABB() :
		CollisionShapeBase(CollisionShapeEnum::AABB_CollisionShapeEnum) {}

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

	float MinDistanceSq(const Vector3D & point) const;

private:
	bool mSetFlag = false;
};

struct Sphere : public CollisionShapeBase
{
	Sphere() :
		CollisionShapeBase(CollisionShapeEnum::Sphere_CollisionShapeEnum) {}

	// 中心の座標
	Vector3D mCenter;
	
	// 半径
	float mRadius;

	bool IsPointInside(const Vector3D & point) const override;
};

namespace CollisionFunc
{
	bool CheckHit(const AABB & box1, const AABB & box2);

	bool CheckHit(const AABB & box, const Sphere & s);

	inline bool CheckHit(const Sphere & s, const AABB & box)
	{
		return CheckHit(box, s);
	}

	bool CheckHit(const Sphere & s1, const Sphere & s2);
}
