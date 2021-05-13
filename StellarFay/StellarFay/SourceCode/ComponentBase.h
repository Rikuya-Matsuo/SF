#pragma once
#include <SDL/SDL.h>

class ComponentBase
{
public:
	// ownerにnullptrを入れないこと！
	ComponentBase(class Actor * owner, int priority = 0);
	virtual ~ComponentBase();

	virtual void Update();

	class Actor * GetOwner() const { return mOwner; }

	int GetPriority() const { return mPriority; }

	bool GetActiveFlag() const { return mActiveFlag; }

	bool GetBeforeUpdateActorFlag() const { return (mPriority < 0); }
	bool GetAfterUpdateActorFlag() const { return !GetBeforeUpdateActorFlag(); }

	void SetPriority(int priority);

	void SetActiveFlag(bool value) { mActiveFlag = value; }

protected:
	class Actor * mOwner;

	// この値が小さい順に実行する。０未満のものはUpdateActor()前に実行される。
	int mPriority;

	bool mActiveFlag;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
