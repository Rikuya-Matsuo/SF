#include <cstdio>
#include <SDL/SDL_image.h>
#include <iostream>
#include "GameSystem.h"
#include "Renderer.h"
#include "PhysicManager.h"
#include "Input.h"
#include "SceneBase.h"

GameSystem::GameSystem():
	mPrevTicks(SDL_GetTicks()),
	mTimeSpeedRate(1.0f),
	mCurrentScene(nullptr)
{
}

GameSystem::~GameSystem()
{
}

bool GameSystem::Init()
{
	// SDL機能の初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER)!= 0)
	{
		printf("SDL初期化失敗\n");
		return false;
	}

	// 画像読み込み機能の初期化
	int imgInitFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (IMG_Init(imgInitFlags) != imgInitFlags)
	{
		printf("画像ライブラリ初期化失敗\n");
		return false;
	}

	// デバッグ時のみフルスクリーンではない
	bool fullScreenFlag = true;
#ifdef DEBUG_SF
	fullScreenFlag = false;
#endif

	// レンダラーの生成・初期化
	mRenderer = new Renderer();
	bool successRendererInit = mRenderer->Init(1920, 1080, fullScreenFlag);
	if (!successRendererInit)
	{
		printf("Rendererクラス初期化失敗\n");
		return false;
	}

	// レンダラーの設定
	// 背景色
	Vector3D BGcolor = Vector3D::Zero;
	mRenderer->SetBGColor(BGcolor);

	// ライト情報
	LightInfo & light = mRenderer->GetLightInfo();

	Vector3D lightDir = Vector3D(1.0f, -1.0f, 0.0f);
	light.mDirection = Vector3D::Normalize(lightDir);

	light.mDiffuseColor = Vector3D(1.0f);
	light.mAmbientColor = Vector3D(0.8f);
	light.mSpecularColor = Vector3D(1.0f);

	// 物理クラス生成
	mPhysicManager = new PhysicManager();

	return true;
}

void GameSystem::Loop()
{
	bool runLoopFlag = true;

	while (runLoopFlag)
	{
		// 時間情報更新
		UpdateDeltaTimes();

		// 入力状態の更新
		INPUT_INSTANCE.Update();

		// escキーの入力 or 終了イベントの発生でループを終了
		if (INPUT_INSTANCE.GetKey(SDL_SCANCODE_ESCAPE) || INPUT_INSTANCE.GetQuitEventFlag())
		{
			runLoopFlag = false;
		}

		// シーン更新
		mCurrentScene->Update();

		// シーン移動判定
		if (mCurrentScene->GetGoNextSceneFlag())
		{
			// 次シーンへ移動
			bool success = GoNextScene();

			// 失敗時、ゲームを終了する
			if (!success)
			{
				runLoopFlag = false;
			}
		}

		// 描画
		mRenderer->Draw();

		// 入力状態の最終更新
		INPUT_INSTANCE.LastUpdate();
	}
}

void GameSystem::Finish()
{
	if (mCurrentScene)
	{
		delete mCurrentScene;
	}

	if (mRenderer)
	{
		delete mRenderer;
	}

	if (mPhysicManager)
	{
		delete mPhysicManager;
	}
}

void GameSystem::UpdateDeltaTimes()
{
	// このフレームのSDL_GetTicks()の値
	Uint32 ticks;

	// フレームレート調整
	do
	{
		ticks = SDL_GetTicks();

		mRealDeltaTime = (ticks - mPrevTicks) / 1000.0f;
	} while (mRealDeltaTime < 1.00f / 30);

	// Ticks値記録
	mPrevTicks = ticks;

	// ゲーム内世界におけるデルタタイム計算
	mGameWorldDetaTime = mRealDeltaTime * mTimeSpeedRate;
}

bool GameSystem::GoNextScene()
{
	// 次シーンの種類取得
	SceneEnum next = mCurrentScene->GetNextScene();

	// 生成
	SceneBase * nextPtr = SceneBase::GenerateScene(next);

	// 生成の成否に関わらず、現在のシーンは削除する
	delete mCurrentScene;
	mCurrentScene = nullptr;

	// 生成失敗時、偽を返す
	if (!nextPtr)
	{
		return false;
	}

	// 成功時は、現在のシーンとして新しいシーンを設定する
	mCurrentScene = nextPtr;

	// 生成に成功したとして、真を返す
	return true;
}
