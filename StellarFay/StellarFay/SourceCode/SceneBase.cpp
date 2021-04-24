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

void SceneBase::RegisterActor(Actor * actor)
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

	// ここまでに挿入できなければ最後尾に追加する
	mActors.emplace_back(actor);
}

void SceneBase::Update()
{
	// フラグの値に従い、アクターをソート
	if (mSortActorFlag)
	{
		// ソート順を示すラムダ式
		auto order = [](const Actor * lhs, const Actor * rhs)
		{
			return lhs->GetPriority() < rhs->GetPriority();
		};

		// ソート
		mActors.sort(order);

		// フラグを下す
		mSortActorFlag = false;
	}

	// アクターの更新
	UpdateActors();

	// シーンの更新
	UpdateScene();
}

void SceneBase::UpdateScene()
{
	// 処理内容は継承先に依存
}

void SceneBase::UpdateActors()
{
	// 全アクターの更新処理
	for (auto itr : mActors)
	{
		// アクティブなもののみ更新を行う
		if (itr->GetActiveFlag())
		{
			itr->Update();
		}
	}
}
