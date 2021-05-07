#pragma once
#include "ColliderAttribute.h"
#include <list>
#include <unordered_map>

// 物理挙動を管理するクラス
class PhysicManager final
{
public:
	PhysicManager();

	// 当たり判定
	void CheckHit();

	// コライダーの登録
	// コライダー側で呼ぶので、外部から呼んでやる必要はない
	void RegisterCollider(class ColliderComponentBase * collider);

	// コライダーの登録解除
	// コライダー側で呼ぶので、外部から呼んでやる必要はない
	void DeregisterCollider(class ColliderComponentBase * collider);

	// コライダーソートの要請
	void RequestSortCollider(ColliderAttribute att) { mSortAttributeList.emplace_back(att); }

private:
	// コライダーのリストの型
	typedef std::list<class ColliderComponentBase *> ColliderList;

	// ColliderAttributeごとに分けられたコライダーのリスト
	std::unordered_map<ColliderAttribute, ColliderList> mColliders;

	// ソート要請が行われた属性を示すリスト
	std::list<ColliderAttribute> mSortAttributeList;

	// リストに示された属性のコライダーをソートする
	void SortColliders();
};
