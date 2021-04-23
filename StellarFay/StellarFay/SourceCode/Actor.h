#pragma once
#include <SDL/SDL.h>
#include <list>
#include "BitFlagFunc.h"
#include "Vector.h"

class Actor
{
public:
	Actor(int priority);
	virtual ~Actor();

	// コンポーネントとアクター自身の更新処理
	virtual void Update() final;

	void SetBelongScene(class SceneBase * scene) { mBelongScene = scene; }

	// コンポーネントのソートを要請する
	// ソートが実行されるのはアクターのUpdate()開始時のみ
	void RequestSortComponents() { BitFlagFunc::Set(mActorFlags, mSortComponentsFlagMask, true); }

	// アクターにコンポーネントを追加
	// コンポーネント基底クラスのコンストラクタにて呼び出すことで、いちいちリストへの追加処理を書かなくてもよいようにする
	void ResisterComponent(class ComponentBase * cmp);

protected:
	// ビットフラグの型
	typedef Uint8 ActorFlagType;

	// ビットフラグマスク
	static const ActorFlagType mSortComponentsFlagMask;

	// ビットフラグ
	ActorFlagType mActorFlags;

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
