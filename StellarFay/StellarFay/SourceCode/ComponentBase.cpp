#include "ComponentBase.h"
#include "Actor.h"

ComponentBase::ComponentBase(Actor * owner, int priority) :
	mOwner(owner),
	mPriority(priority)
{
	owner->ResisterComponent(this);
}

ComponentBase::~ComponentBase()
{
}

void ComponentBase::Update()
{
	// 処理内容は継承先に依存する
}
