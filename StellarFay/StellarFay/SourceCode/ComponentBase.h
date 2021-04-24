#pragma once
#include <SDL/SDL.h>

class ComponentBase
{
public:
	// ownerにnullptrを入れないこと！
	ComponentBase(class Actor * owner, int priority = 0);
	virtual ~ComponentBase();

	virtual void Update();

	virtual class Actor * GetOwner() const final { return mOwner; }

	virtual int GetPriority() const final { return mPriority; }

	virtual bool GetBeforeUpdateActorFlag() const final { return (mPriority < 0); }
	virtual bool GetAfterUpdateActorFlag() const final { return !GetBeforeUpdateActorFlag(); }

	void SetPriority(int priority);

protected:
	class Actor * mOwner;

	// この値が小さい順に実行する。０未満のものはUpdateActor()前に実行される。
	int mPriority;
};
