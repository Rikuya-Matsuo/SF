#include "PhysicManager.h"
#include "ColliderComponentBase.h"

PhysicManager::PhysicManager()
{
}

void PhysicManager::CheckHit()
{
}

void PhysicManager::RegisterCollider(ColliderComponentBase * collider)
{
	// コライダーが入るリストの参照
	ColliderList & list = mColliders[collider->GetAttribute()];

	// 判定順
	const int order = collider->GetCheckOrder();

	// 判定順になるように挿入する
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		if (order <= (*itr)->GetCheckOrder())
		{
			list.insert(itr, collider);

			// 挿入し終えたら関数を抜ける
			return;
		}
	}

	// ここまでに挿入されてなければ、最後尾に追加する
	list.emplace_back(collider);
}

void PhysicManager::DeregisterCollider(ColliderComponentBase * collider)
{

}
