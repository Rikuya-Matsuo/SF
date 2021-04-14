#pragma once
#include <SDL/SDL.h>
#include <list>
#include "BitFlagFunc.h"
#include "ComponentBase.h"

class Actor
{
public:
	Actor(int priority);
	virtual ~Actor();

	// コンポーネントとアクター自身の更新処理
	virtual void Update() final;

	// コンポーネントのソートを要請する
	// ソートが実行されるのはアクターのUpdate()開始時のみ
	void RequestSortComponents() { BitFlagFunc::Set(mActorFlags, mSortComponentsFlagMask, true); }

protected:
	// ビットフラグの型
	typedef Uint8 ActorFlagType;

	// ビットフラグマスク
	static const ActorFlagType mSortComponentsFlagMask;

	// ビットフラグ
	ActorFlagType mActorFlags;

	// この値が小さいものから処理を行う。
	int mPriority;

	// 所持するコンポーネントのリスト
	std::list<ComponentBase *> mComponents;

	// 継承先の処理
	virtual void UpdateActor();
	// 全コンポーネントの更新とUpdateActor()が終わった後に走る処理
	virtual void UpdateActorLast();

};
