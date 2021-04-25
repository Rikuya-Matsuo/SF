#include "SceneBase.h"
#include <iostream>
#include "Actor.h"
#include "TestScene.h"

SceneBase * SceneBase::mLatestScene = nullptr;

SceneBase::SceneBase() :
	mGoNextSceneFlag(false),
	mNextScene(SceneEnum::Invalid_SceneEnum)
{
	mLatestScene = this;
}

void SceneBase::GetLatestScene(Actor * actor)
{
	// アクターに最新シーンを設定
	actor->SetBelongScene(mLatestScene);
}

SceneBase * SceneBase::GenerateScene(SceneEnum sceneEnum)
{
	// 返却値
	SceneBase * ret = nullptr;

	// シーン生成
	switch (sceneEnum)
	{
	case Test_SceneEnum:
		ret = new TextScene();
		break;

	case Title_SceneEnum:
		// タイトルシーン生成
		break;

	case Game_SceneEnum:
		// ゲームシーン生成
		break;

	case Base_SceneEnum:
	case Invalid_SceneEnum:
		break;

	default:
		std::cout << "GenerateScene : 挙動未定義のシーン列挙体が渡されました\n";
		break;
	}

	return ret;
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
