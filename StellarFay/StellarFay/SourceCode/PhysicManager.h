#pragma once
#include "ColliderAttribute.h"
#include "Hash.h"
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
	void RegisterCollider(const class ColliderComponentBase * collider);

	// コライダーの登録解除
	// コライダー側で呼ぶので、外部から呼んでやる必要はない
	void DeregisterCollider(const class ColliderComponentBase * collider);

	// コライダーソートの要請
	void RequestSortCollider(ColliderAttribute att) { mSortAttributeList.emplace_back(att); }

private:
	// コライダーのリストの型
	typedef std::list<const class ColliderComponentBase *> ColliderList;

	// ColliderAttributeごとに分けられたコライダーのリスト
	std::unordered_map<ColliderAttribute, ColliderList> mColliders;

	// ソート要請が行われた属性を示すリスト
	std::list<ColliderAttribute> mSortAttributeList;

	// コライダーのペア
	typedef std::pair<const class ColliderComponentBase *, const class ColliderComponentBase *> ColliderPair;

	// コライダーのペアのハッシュファンクタ
	struct ColPairHash
	{
		size_t operator()(const ColliderPair & pair) const
		{
			size_t arr[2] = { std::hash<const ColliderComponentBase*>()(pair.first), std::hash<const ColliderComponentBase*>()(pair.second) };
			return Hash::Fnv1(arr, 2);
		}
	};

	// 接触状況の列挙体
	enum HitState : Uint8
	{
		NoTouch_HitState = 0,
		Hit_HitState,
		BeTouching_HitState,
		Invalid_HitState
	};

	// コライダーコンポーネント同士の接触状況を記録するマップ
	std::unordered_map<ColliderPair, HitState, ColPairHash> mHitState;

	// コライダーペアの発行（コライダーペアを作成する場合はこの関数を必ず用いるようにする）
	void MakeColliderPair(const class ColliderComponentBase * col1, const class ColliderComponentBase * col2, ColliderPair & ret);

	// リストに示された属性のコライダーをソートする
	void SortColliders();

	// 引数に指定された属性組み合わせのチェックを行う
	// ポインタから頭2つの組み合わせのチェックを行うものとする
	void CollisionCheckLoop(const ColliderAttribute * attCombi);

	// 2つのコライダーが接触しているかを検証する。
	bool Judge(const class ColliderComponentBase ** col);

	// コライダーの所有者であるアクターの当たり判定反応関数を呼ぶ
	void CallReaction(const class ColliderComponentBase ** col, bool hitFlag);
};
