#pragma once
#include <SDL/SDL.h>

// デバッグマクロ
// リリース時はこれをコメントアウトする
#define DEBUG_SF

// ゲームのシステム面の流れを管理するシングルトンクラス
class GameSystem final
{
public:
	~GameSystem();

	static GameSystem & GetInstance()
	{
		static GameSystem instance;
		return instance;
	}

	bool Init();

	void Loop();

	void Finish();

	float GetDeltaTime() const { return mRealDeltaTime; }

	float GetGameWorldDeltaTime() const { return mGameWorldDetaTime; }

	void SetTimeSpeedRate(float rate) { mTimeSpeedRate = rate; }

private:
	GameSystem();

	////////////////
	// 時間関係
	////////////////
	// 前フレームのSDL_GetTicks()の値
	Uint32 mPrevTicks;

	// 現実時間のデルタタイム
	float mRealDeltaTime;
	
	// スローモーションなどを加味した、ゲームオブジェクト目線のデルタタイム
	float mGameWorldDetaTime;

	// ゲーム内世界の時間の経過スピード倍率
	// スローモーション演出などを加える場合はここをいじればいい
	float mTimeSpeedRate;

	void UpdateDeltaTimes();
};

#define GAME_SYSTEM_INSTANCE GameSystem::GetInstance()
