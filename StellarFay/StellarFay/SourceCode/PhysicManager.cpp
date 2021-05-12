#include "PhysicManager.h"
#include "ColliderComponentBase.h"
#include "Actor.h"
#include <algorithm>

PhysicManager::PhysicManager()
{
}

PhysicManager::~PhysicManager()
{
	// コライダーリストのメモリ解放
	for (auto itr = mColliders.begin(); itr != mColliders.end(); ++itr)
	{
		itr->second.clear();

		ColliderList().swap(itr->second);
	}
	mColliders.clear();
	std::unordered_map<ColliderAttribute, ColliderList>().swap(mColliders);

	// ソート要請受け付けリストのメモリ解放
	mSortAttributeList.clear();
	std::list<ColliderAttribute>().swap(mSortAttributeList);

	// 接触状況記録マップのメモリ解放
	mHitState.clear();
	std::unordered_map<ColliderPair, HitState, ColPairHash>().swap(mHitState);
}

void PhysicManager::CheckHit()
{
	// 接触可能な全ての属性の組み合わせで判定を行う
	const size_t tacIMax = CalculateTouchableAttributeCombinationMass();
	for (size_t i = 0; i < tacIMax; ++i)
	{
		const ColliderAttribute * attCombi = TouchableAttributeCombinations[i];

		CollisionCheckLoop(attCombi);
	}
}

void PhysicManager::CollisionCheckLoop(const ColliderAttribute * attCombi)
{
	// 指定属性のリストのエイリアス取得
	ColliderList * colList[2] = { &mColliders[attCombi[0]], &mColliders[attCombi[1]] };

	// 片方のリストが空なら関数を抜ける
	if (colList[0]->empty() || colList[1]->empty())
	{
		return;
	}

	// 判定を行う2つのコライダーコンポーネントへのポインタをエイリアスとして持つ配列
	const ColliderComponentBase * colliders[2] = { nullptr };

	// 判定の前準備をするラムダ式
	auto prepareForJudge = [&colliders](size_t index, const ColliderComponentBase * col)
	{
		// 判定が可能でなければ偽を返す
		bool skipJudge = !col->GetActiveFlag() || !col->GetOwner()->GetActiveFlag();
		if (skipJudge)
		{
			return false;
		}

		// エイリアス取得
		colliders[index] = col;

		return true;
	};

	// 全ての組み合わせについて検証
	for (auto list0itr = colList[0]->begin(); list0itr != colList[0]->end(); ++list0itr)
	{
		// 準備
		bool allowed = prepareForJudge(0, *list0itr);

		// 判定が可能でなければスキップ
		if (!allowed)
		{
			continue;
		}

		for (auto list1itr = colList[1]->begin(); list1itr != colList[1]->end(); ++list1itr)
		{
			// 準備
			allowed = prepareForJudge(1, *list1itr);

			// 判定が可能でなければスキップ
			if (!allowed)
			{
				continue;
			}

			// （同じ属性同士での判定時）同じコライダー同士での判定は行わない
			bool sameCollider = (colliders[0] == colliders[1]);
			if (sameCollider)
			{
				continue;
			}

			// 判定
			bool hitFlag = Judge(colliders);

			// アクターから判定に対する反応を引き出す
			CallReaction(colliders, hitFlag);
		}
	}
}

bool PhysicManager::Judge(const ColliderComponentBase ** col)
{
	// 形状enumを取得
	CollisionShapeEnum shape[2] = { col[0]->GetCollision()->mShape, col[1]->GetCollision()->mShape };

	// 当たったかどうかのフラグ
	bool hitFlag = false;

	// 形状の組み合わせごとに分岐
	if (shape[0] == CollisionShapeEnum::AABB_CollisionShapeEnum)
	{
		// col[0]のボックス取得
		const AABB * box1 = col[0]->GetWorldBox();

		// col[1]の形状によって判定を分岐
		if (shape[1] == CollisionShapeEnum::AABB_CollisionShapeEnum)
		{
			const AABB * box2 = col[1]->GetWorldBox();
			hitFlag = CollisionFunc::CheckHit(*box1, *box2);
		}
		else if (shape[1] == CollisionShapeEnum::Sphere_CollisionShapeEnum)
		{
			const Sphere * s = col[1]->GetWorldSphere();
			hitFlag = CollisionFunc::CheckHit(*box1, *s);
		}
	}
	else if (shape[0] == CollisionShapeEnum::Sphere_CollisionShapeEnum)
	{
		// col[0]の球を取得
		const Sphere * s1 = col[0]->GetWorldSphere();

		// col[1]の形状によって判定を分岐
		if (shape[1] == CollisionShapeEnum::AABB_CollisionShapeEnum)
		{
			const AABB * box = col[1]->GetWorldBox();
			hitFlag = CollisionFunc::CheckHit(*s1, *box);
		}
		else if (shape[1] == CollisionShapeEnum::Sphere_CollisionShapeEnum)
		{
			const Sphere * s2 = col[1]->GetWorldSphere();
			hitFlag = CollisionFunc::CheckHit(*s1, *s2);
		}
	}

	return hitFlag;
}

void PhysicManager::CallReaction(const ColliderComponentBase ** col, bool hitFlag)
{
	// ペアとしての変数を作成
	ColliderPair pair;
	MakeColliderPair(col[0], col[1], pair);

	// 元々の接触状況を取得
	auto itr = mHitState.find(pair);
	HitState prevState = HitState::Invalid_HitState;
	if (itr != mHitState.end())
	{
		prevState = itr->second;
	}

	// 前のフレームの段階で触れていることを示す
	bool touchSincePrevFrame = (prevState == HitState::Hit_HitState || prevState == HitState::BeTouching_HitState);

	// 2つのコライダーがこのフレームでも前のフレームでも触れていない場合、この関数でするべきことはない
	if (!hitFlag && !touchSincePrevFrame)
	{
		return;
	}

	// Ownerを得るラムダ式
	auto getActor = [&col](Uint8 index) -> Actor *
	{
		return col[index]->GetOwner();
	};

	// このフレームで触れている場合
	if (hitFlag)
	{
		// 前のフレームで触れていない（あるいはそれまでに触れたことがない）場合
		if (!touchSincePrevFrame)
		{
			getActor(0)->OnHit(col[0], col[1]);
			getActor(1)->OnHit(col[1], col[0]);

			mHitState[pair] = HitState::Hit_HitState;
		}
		// 前のフレームから触れている場合
		else if (touchSincePrevFrame)
		{
			getActor(0)->OnBeTouching(col[0], col[1]);
			getActor(1)->OnBeTouching(col[1], col[0]);

			mHitState[pair] = HitState::BeTouching_HitState;
		}
	}
	// このフレームで触れておらず、かつ前のフレームまで触れていた場合
	else if (touchSincePrevFrame)
	{
		getActor(0)->OnPart(col[0], col[1]);
		getActor(1)->OnPart(col[1], col[0]);

		mHitState[pair] = HitState::NoTouch_HitState;
	}
}

void PhysicManager::RegisterCollider(const ColliderComponentBase * collider)
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

void PhysicManager::DeregisterCollider(const ColliderComponentBase * collider)
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

void PhysicManager::MakeColliderPair(const ColliderComponentBase * col1, const ColliderComponentBase * col2, ColliderPair & ret)
{
	// ハッシュ値が小さい方をcol1とするので、
	// col1の方がハッシュ値が大きい場合入れ替える
	if (std::hash<const ColliderComponentBase*>()(col1) > std::hash<const ColliderComponentBase*>()(col2))
	{
		const ColliderComponentBase * tmp = col1;
		col1 = col2;
		col2 = tmp;
	}

	// ペアを作成
	ret = std::make_pair(col1, col2);
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
