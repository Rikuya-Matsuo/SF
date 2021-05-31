#include <SDL/SDL.h>
#include "GamePad.h"

// 入力を管理するクラス
class Input final
{
public:
	~Input();

	static Input & GetInstance()
	{
		static Input instance;
		return instance;
	}

	// 現フレームでキーが押されているか
	bool GetKey(int scanCode) const { return mKeyStates[scanCode]; }

	// このフレームでキーが押されたなら真を返す
	bool GetKeyPressDown(int scanCode) const { return (mKeyStates[scanCode] && !mPrevKeyStates[scanCode]); }

	// このフレームでキーが離されたなら真を返す
	bool GetKeyRelease(int scanCode) const { return (!mKeyStates[scanCode] && mPrevKeyStates[scanCode]); }

	// 終了イベントフラグの取得
	bool GetQuitEventFlag() const { return mQuitEventFlag; }

	// ゲームパッドを取得する
	const GamePad & GetGamePad(int index) const { return mGamePads[index]; }

	// 更新処理
	void Update();

	// フレームの最後に呼ぶ。主に入力状態の記録が目的
	void LastUpdate();

private:
	Input();

	// 用意するゲームパッドクラスの数
	static const size_t mMaxGamePadMass;

	// ゲームパッド配列
	GamePad * mGamePads;

	// キー状態
	const Uint8 * mKeyStates;

	// 前フレームのキー状態
	Uint8 mPrevKeyStates[SDL_NUM_SCANCODES];

	// 終了イベントが発生したフラグ
	bool mQuitEventFlag;
};

#define INPUT_INSTANCE Input::GetInstance()

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
