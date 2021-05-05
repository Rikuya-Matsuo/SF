#pragma once
#include "ComponentBase.h"
#include "ColliderAttribute.h"

class ColliderComponentBase : public ComponentBase
{
public:
	ColliderComponentBase(class Actor * owner, ColliderAttribute att);

	virtual ~ColliderComponentBase();

	ColliderAttribute GetAttribute() const { return mAttribute; }

protected:
	const ColliderAttribute mAttribute;

};
