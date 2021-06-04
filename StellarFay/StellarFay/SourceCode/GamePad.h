#pragma once
#include <SDL/SDL.h>
#include "Vector.h"

class GamePad final
{
public:
	GamePad();
	GamePad(int index);
	~GamePad();

	// 接続する
	void Connect(int index);

	// 接続解除
	void Disconnect();

	// 有効フラグ計算
	bool IsValid() const { return static_cast<bool>(mGamePad); }

	// スティックのデッドゾーンの設定
	// 各スティック各方向への倒し具合の絶対値がこの値に満たない場合、値を0.0fとしてクランプする
	static void SetStickDeadZone(float deadZone) { mStickDeadZone = deadZone; }

	// スティックのデッドゾーン取得
	static float GetStickDeadZone() { return mStickDeadZone; }

	// このフレームでボタンが押されたかを調べる
	bool GetButtonPressDown(SDL_GameControllerButton button) const { return GetButtonFlag(button, mButtons) && !GetButtonFlag(button, mPrevButtons); }

	// 前フレーム以前から継続してボタンが押されているかを調べる
	bool GetButtonPressed(SDL_GameControllerButton button) const { return GetButtonFlag(button, mButtons) && GetButtonFlag(button, mPrevButtons); }

	// このフレームでボタンが離されたかを調べる
	bool GetButtonReleas(SDL_GameControllerButton button) const { return !GetButtonFlag(button, mButtons) && GetButtonFlag(button, mPrevButtons); }

	// 左スティックの入力取得
	const Vector2D & GetLeftStick() const { return mLeftStick; }

	// 右スティックの入力取得
	const Vector2D & GetRightStick() const { return mRightStick; }

	// 更新
	void Update();

	// フレーム終了時の更新
	void LastUpdate();

private:
	// スティックのデッドゾーン
	static float mStickDeadZone;

	// ポインタ
	SDL_GameController * mGamePad;

	// マッピング
	char * mMapping;

	// 左右のスティック
	Vector2D mLeftStick;
	Vector2D mRightStick;

	// ボタン状態のビットフラグの型
	typedef Uint32 ButtonFlags;

	// ボタン状態のビットフラグ
	// マスクは1をSDL_GameControllerButtonの値分左シフトさせたもの
	ButtonFlags mButtons;

	// 前フレームのボタン状態ビットフラグ
	ButtonFlags mPrevButtons;

	// ボタン状態ビットフラグから、ボタンが押されているか否かを得る内部関数
	bool GetButtonFlag(SDL_GameControllerButton button, ButtonFlags flags) const { return static_cast<bool>(flags & (1 << button)); }

	// ボタン状態、スティック状態の初期化
	void InitState();
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
