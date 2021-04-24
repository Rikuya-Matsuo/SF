#include "SceneBase.h"
#include "Actor.h"

SceneBase * SceneBase::mLatestScene = nullptr;

SceneBase::SceneBase()
{
	mLatestScene = this;
}

void SceneBase::GetLatestScene(Actor * actor)
{
	// アクターに最新シーンを設定
	actor->SetBelongScene(mLatestScene);
}

void SceneBase::ResisterActor(Actor * actor)
{
	// 更新順を取得
	const int priority = actor->GetPriority();

	// 更新順になるよう挿入する
	for (auto itr = mActors.begin(); itr != mActors.end(); ++itr)
	{
		if (priority < (*itr)->GetPriority())
		{
			mActors.insert(itr, actor);
			return;
		}
	}

	// ここまでに挿入できなければ最後尾に挿入する
	mActors.emplace_back(actor);
}

void SceneBase::UpdateActor()
{
	for (auto itr : mActors)
	{
		itr->Update();
	}
}
