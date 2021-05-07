#include "PhysicManager.h"
#include "ColliderComponentBase.h"
#include <algorithm>

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
	// リストの参照
	ColliderList & list = mColliders[collider->GetAttribute()];

	// 検索
	auto itr = std::find(list.begin(), list.end(), collider);

	// 検索ヒット時は削除
	if (itr != list.end())
	{
		list.erase(itr);
	}
}

void PhysicManager::SortColliders()
{
	// 重複する要素を削除する
	mSortAttributeList.sort();
	mSortAttributeList.unique();

	// 各属性のソートを行う
	for (auto itr : mSortAttributeList)
	{
		// 順番を示すラムダ式
		auto checkOrder = [](const ColliderComponentBase * lhs, const ColliderComponentBase * rhs)
		{
			return (lhs->GetCheckOrder() <= rhs->GetCheckOrder());
		};

		// ソート
		mColliders[itr].sort(checkOrder);
	}

	// 要請リストをクリア
	mSortAttributeList.clear();
}
