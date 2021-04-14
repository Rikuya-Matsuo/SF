#pragma once
#include <SDL/SDL.h>

class ComponentBase
{
public:
	ComponentBase(class Actor * owner, int priority = 0);
	virtual ~ComponentBase();

	virtual void Update();

	virtual int GetPriority() const final { return mPriority; }

	virtual bool GetBeforeUpdateActorFlag() const final { return (mPriority < 0); }
	virtual bool GetAfterUpdateActorFlag() const final { return !GetBeforeUpdateActorFlag(); }

protected:
	class Actor * mOwner;

	// この値が小さい順に実行する。０未満のものはUpdateActor()前に実行される。
	int mPriority;
};
