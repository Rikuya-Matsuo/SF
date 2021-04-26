#include "DestroyableActor.h"

DestroyableActor::DestroyableActor(int priority, HitPoint hp, HitPoint maxHp) :
	Actor(priority),
	mHitPoint(hp),
	mMaxHitPoint(maxHp),
	mAliveFlag(true)
{
}

DestroyableActor::~DestroyableActor()
{
}

void DestroyableActor::Damage(HitPoint damage)
{
	// ヒットポイントを減らす
	mHitPoint -= damage;

	// （回復に使われることもあるため）ヒットポイントのクランプ
	if (mHitPoint > mMaxHitPoint)
	{
		mHitPoint = mMaxHitPoint;
	}
}
