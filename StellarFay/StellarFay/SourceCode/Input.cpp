#include "Input.h"

const size_t Input::mMaxGamePadMass = 1;

Input::Input() :
	mQuitEventFlag(false)
{
	// キー状態にアクセスできるポインタを取得
	mKeyStates = SDL_GetKeyboardState(nullptr);

	// 前フレームのキー状態を初期化
	for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
	{
		mPrevKeyStates[i] = 0;
	}

	// ゲームパッド配列の生成
	mGamePads = new GamePad[mMaxGamePadMass];

	// ゲームパッド接続
	for (size_t i = 0; i < mMaxGamePadMass; ++i)
	{
		// 接続を試みる
		mGamePads[i].Connect(i);
	}
}

Input::~Input()
{
	delete[] mGamePads;
	mGamePads = nullptr;
}

void Input::Update()
{
	// イベント処理
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		// 終了フラグを立てる
		if (sdlEvent.type == SDL_QUIT)
		{
			mQuitEventFlag = true;
		}
	}

	// キーボード入力状態はSDLによって自動更新されている

	// ゲームパッド更新
	SDL_GameControllerUpdate();
	for (size_t i = 0; i < mMaxGamePadMass; ++i)
	{
		if (mGamePads[i].IsValid())
		{
			mGamePads[i].Update();
		}
	}
}

void Input::LastUpdate()
{
	// キー状態保存
	SDL_memcpy(mPrevKeyStates, mKeyStates, SDL_NUM_SCANCODES);

	// ゲームパッド
	for (size_t i = 0; i < mMaxGamePadMass; ++i)
	{
		if (mGamePads[i].IsValid())
		{
			mGamePads[i].LastUpdate();
		}
	}
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
