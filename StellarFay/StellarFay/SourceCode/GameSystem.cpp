#include <cstdio>
#include <SDL/SDL_image.h>
#include <iostream>
#include "GameSystem.h"
#include "Renderer.h"

GameSystem::GameSystem():
	mPrevTicks(SDL_GetTicks()),
	mTimeSpeedRate(1.0f)
{
}

GameSystem::~GameSystem()
{
}

bool GameSystem::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER)!= 0)
	{
		printf("SDL初期化失敗\n");
		return false;
	}

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

	mRenderer = new Renderer();
	mRenderer->Init(1920, 1080, fullScreenFlag);

	return true;
}

void GameSystem::Loop()
{
	bool runLoopFlag = true;

	while (runLoopFlag)
	{
		runLoopFlag = false;
	}
}

void GameSystem::Finish()
{
	if (mRenderer)
	{
		delete mRenderer;
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
