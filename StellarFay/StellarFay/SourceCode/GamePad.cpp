#include "GamePad.h"

float GamePad::mStickDeadZone = 0.0f;

GamePad::GamePad() :
	mButtons(0),
	mPrevButtons(0),
	mGamePad(nullptr),
	mMapping(nullptr)
{
}

GamePad::GamePad(int index) :
	GamePad()
{
	Connect(index);
}

GamePad::~GamePad()
{
	if (mMapping)
	{
		SDL_free(mMapping);
		mMapping = nullptr;
	}

	if (mGamePad)
	{
		SDL_GameControllerClose(mGamePad);
		mGamePad = nullptr;
	}
}

void GamePad::Connect(int index)
{
	mGamePad = SDL_GameControllerOpen(index);

	if (mGamePad)
	{
		mMapping = SDL_GameControllerMapping(mGamePad);
	}
}

void GamePad::Update()
{
	// スティックのSint16の値をfloat型-1.0f～1.0fの相対的な値に変換するラムダ式
	auto relativeValue = [](Sint16 v)
	{
		// float型に変換した絶対値
		float vfAbs = fabsf(v);

		// vが正の数ならSint16の最大値、負の数ならSint16の最小値を代入
		Sint16 maxOrMin = ((v >= 0) ? SDL_MAX_SINT16 : SDL_MIN_SINT16);

		return vfAbs / maxOrMin;
	};

	// スティック値がデッドゾーンに満たない場合、0.0fを代入するラムダ式
	auto calcDeadZone = [](float & stickValue)
	{
		if (fabsf(stickValue) < GamePad::GetStickDeadZone())
		{
			stickValue = 0.0f;
		}
	};

	// 左スティック更新
	Sint16 lx = SDL_GameControllerGetAxis(mGamePad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX);
	Sint16 ly = SDL_GameControllerGetAxis(mGamePad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY);

	mLeftStick.x = relativeValue(lx);
	mLeftStick.y = relativeValue(ly);

	// デッドゾーン考慮
	calcDeadZone(mLeftStick.x);
	calcDeadZone(mLeftStick.y);

	// 右スティック更新
	Sint16 rx = SDL_GameControllerGetAxis(mGamePad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX);
	Sint16 ry = SDL_GameControllerGetAxis(mGamePad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY);

	mRightStick.x = relativeValue(rx);
	mRightStick.y = relativeValue(ry);

	// デッドゾーン考慮
	calcDeadZone(mRightStick.x);
	calcDeadZone(mRightStick.y);

	// ボタン状態初期化
	mButtons = 0;

	// ボタン状態取得
	for (size_t button = 0; button < SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX; ++button)
	{
		// フラグマスク作成
		const ButtonFlags mask = 1 << button;

		// ボタンが押されていればフラグを立てる
		if (SDL_GameControllerGetButton(mGamePad, static_cast<SDL_GameControllerButton>(button)))
		{
			mButtons |= mask;
		}
	}
}

void GamePad::LastUpdate()
{
	mPrevButtons = mButtons;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
