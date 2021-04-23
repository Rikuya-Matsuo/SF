#include "Actor.h"
#include "ComponentBase.h"
#include "SceneBase.h"
#include <algorithm>

const Actor::ActorFlagType Actor::mSortComponentsFlagMask = BIT_SHIFT(0);

Actor::Actor(int priority) :
	mPriority(priority)
{
	SceneBase::GetLatestScene(this);
}

Actor::~Actor()
{
	// コンポーネントのメモリ解放
	for (auto itr = mComponents.begin(); itr != mComponents.end(); ++itr)
	{
		delete *itr;
		*itr = nullptr;
	}
	std::list<ComponentBase *>().swap(mComponents);
}

void Actor::Update()
{
	// コンポーネントのソート要請を受けた場合、実行
	if (BitFlagFunc::GetOr(mActorFlags, mSortComponentsFlagMask))
	{
		// ソート順を示すラムダ式
		auto orderLambda = [](const ComponentBase * c1, const ComponentBase * c2)
		{
			return (c1->GetPriority() < c2->GetPriority());
		};

		// 各コンポーネントのmPriorityの値が小さい順にソート
		mComponents.sort(orderLambda);
	}

	// アクター更新前に更新するコンポーネントを更新
	auto itr = mComponents.begin();
	for (; (*itr)->GetBeforeUpdateActorFlag(); ++itr)
	{
		(*itr)->Update();
	}

	// 継承先依存の更新処理
	UpdateActor();

	// アクター更新後に更新するコンポーネントを更新
	for (; itr != mComponents.end(); ++itr)
	{
		(*itr)->Update();
	}

	// 継承先依存の最終更新
	UpdateActorLast();
}

void Actor::ResisterComponent(ComponentBase * cmp)
{
	// mPriorityの値が小さい順になるよう挿入する
	const int priority = cmp->GetPriority();

	for (auto itr = mComponents.begin(); itr != mComponents.end(); ++itr)
	{
		const int itrPriority = (*itr)->GetPriority();

		if (priority < itrPriority)
		{
			mComponents.insert(itr, cmp);
			return;
		}
	}

	// ここまでで挿入できなかった場合、最後尾に追加する
	mComponents.emplace_back(cmp);
}

void Actor::UpdateActor()
{
	// 内容は継承先に依存する
}

void Actor::UpdateActorLast()
{
	// 内容は継承先に依存する
}
