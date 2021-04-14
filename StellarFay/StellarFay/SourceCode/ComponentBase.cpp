#include "ComponentBase.h"

ComponentBase::ComponentBase(Actor * owner, int priority) :
	mOwner(owner),
	mPriority(priority)
{
}

ComponentBase::~ComponentBase()
{
}

void ComponentBase::Update()
{
	// 処理内容は継承先に依存する
}
