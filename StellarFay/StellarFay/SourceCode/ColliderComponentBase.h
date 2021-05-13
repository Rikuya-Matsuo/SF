#pragma once
#include "ComponentBase.h"
#include "ColliderAttribute.h"
#include "Collision.h"

class ColliderComponentBase : public ComponentBase
{
public:
	ColliderComponentBase(class Actor * owner, ColliderAttribute att, int checkOrder = 0);

	virtual ~ColliderComponentBase();

	ColliderAttribute GetAttribute() const { return mAttribute; }

	int GetCheckOrder() const { return mCheckOrder; }

	// コリジョン基底データの取得。継承先によって形状が異なる。
	// 基底クラスではnullptrを返す。
	virtual const CollisionShapeBase * GetCollision() const { return nullptr; }

	// AABBの取得
	// AABBをコリジョンとして持たないコンポーネントからはnullptrを返す
	virtual const AABB * GetWorldBox() const { return nullptr; }

	// 球の取得
	// 球をコリジョンとして持たないコンポーネントからはnullptrを返す
	virtual const Sphere * GetWorldSphere() const { return nullptr; }

protected:
	const ColliderAttribute mAttribute;

	// 当たり判定を行う順番を示す変数
	// この値が小さい順に判定を行う
	int mCheckOrder;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
