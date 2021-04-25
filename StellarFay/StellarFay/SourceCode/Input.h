#include <SDL/SDL.h>

// 入力を管理するシングルトンクラス
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

	// 更新処理
	void Update();

	// フレームの最後に呼ぶ。主に入力状態の記録が目的
	void LastUpdate();

private:
	Input();

	// キー状態
	const Uint8 * mKeyStates;

	// 前フレームのキー状態
	Uint8 mPrevKeyStates[SDL_NUM_SCANCODES];

	// 終了イベントが発生したフラグ
	bool mQuitEventFlag;
};

#define INPUT_INSTANCE Input::GetInstance()
