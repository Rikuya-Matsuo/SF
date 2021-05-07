#include "ColliderComponentBase.h"
#include "GameSystem.h"
#include "PhysicManager.h"

ColliderComponentBase::ColliderComponentBase(Actor * owner, ColliderAttribute att, int checkOrder) :
	ComponentBase(owner, 1000),
	mAttribute(att),
	mCheckOrder(checkOrder)
{
	PHYSIC_MANAGER_INSTANCE.RegisterCollider(this);
}

ColliderComponentBase::~ColliderComponentBase()
{
	PHYSIC_MANAGER_INSTANCE.DeregisterCollider(this);
}
