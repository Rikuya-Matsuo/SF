﻿#pragma once
#include "ComponentBase.h"
#include "ColliderAttribute.h"

class ColliderComponentBase : public ComponentBase
{
public:
	ColliderComponentBase(class Actor * owner, ColliderAttribute att, int checkOrder = 0);

	virtual ~ColliderComponentBase();

	ColliderAttribute GetAttribute() const { return mAttribute; }

	int GetCheckOrder() const { return mCheckOrder; }

protected:
	const ColliderAttribute mAttribute;

	// 当たり判定を行う順番を示す変数
	// この値が小さい順に判定を行う
	int mCheckOrder;
};
