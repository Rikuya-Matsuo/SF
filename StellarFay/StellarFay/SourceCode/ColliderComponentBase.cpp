#include "ColliderComponentBase.h"

ColliderComponentBase::ColliderComponentBase(Actor * owner, ColliderAttribute att) :
	ComponentBase(owner, 1000),
	mAttribute(att)
{
}

ColliderComponentBase::~ColliderComponentBase()
{
}
