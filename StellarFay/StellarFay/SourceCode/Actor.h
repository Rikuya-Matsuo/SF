#pragma once
#include <SDL/SDL.h>
#include <list>
#include "BitFlagFunc.h"
#include "Vector.h"

namespace ActorFlagMask
{
	// ビットフラグの型
	typedef Uint8 Type;

	// ビットフラグマスク
	static const Type mSortComponentsFlagMask			= BIT_SHIFT(0);
	static const Type mActiveFlagMask					= BIT_SHIFT(1);

	// ビットフラグの初期化値
	static const Type mInitialFlag = mActiveFlagMask;
}

class Actor
{
public:
	Actor(int priority);
	virtual ~Actor();

	// コンポーネントとアクター自身の更新処理
	virtual void Update() final;

	void SetBelongScene(class SceneBase * scene) { mBelongScene = scene; }

	int GetPriority() const { return mPriority; }

	// コンポーネントのソートを要請する
	// ソートが実行されるのはアクターのUpdate()開始時のみ
	void RequestSortComponents() { BitFlagFunc::Set(mActorFlags, ActorFlagMask::mSortComponentsFlagMask, true); }

	// アクターにコンポーネントを追加
	// コンポーネント基底クラスのコンストラクタにて呼び出すことで、いちいちリストへの追加処理を書かなくてもよいようにする
	void ResisterComponent(class ComponentBase * cmp);

protected:
	// ビットフラグ
	ActorFlagMask::Type mActorFlags;

	// この値が小さいものから処理を行う。
	int mPriority;

	// 位置
	Vector3D mPosition;

	// スケール値
	Vector3D mScales;

	// 回転
	Vector3D mRotation;

	// 所持するコンポーネントのリスト
	std::list<class ComponentBase *> mComponents;

	// 所属するシーン
	class SceneBase * mBelongScene;

	// 継承先の処理
	virtual void UpdateActor();
	// 全コンポーネントの更新とUpdateActor()が終わった後に走る処理
	virtual void UpdateActorLast();
};
