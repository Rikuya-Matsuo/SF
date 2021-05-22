#pragma once
#include <SDL/SDL.h>
#include <list>
#include "Vector.h"
#include "Matrix.h"
#include "RandomGenerator.h"

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

	class Renderer * GetRenderer() { return mRenderer; }

	class PhysicManager * GetPhysicManager() { return mPhysicManager; }

	// システムが所有する乱数生成機の参照
	RandomGenerator & GetRand() { return mRand; }

	void SetFirstScene(class SceneBase * scene) { mCurrentScene = scene; }

	void SetTimeSpeedRate(float rate) { mTimeSpeedRate = rate; }

	// UIの登録
	void RegisterUI(class UILayerBase * ui);

	// UIの登録解除
	void DeregisterUI(class UILayerBase * ui);

private:
	GameSystem();

	/////////////////////
	// レンダラー
	/////////////////////
	class Renderer * mRenderer;

	///////////////////////
	// 物理
	///////////////////////
	class PhysicManager * mPhysicManager;

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

	///////////////////
	// シーン
	///////////////////
	// 現在のシーン
	class SceneBase * mCurrentScene;

	// 次のシーンに移動する
	bool GoNextScene();

	//////////////////
	// UI
	//////////////////
	// UIコンテナ
	std::list<class UILayerBase *> mUIs;

	//////////////////
	// 乱数
	//////////////////
	// 乱数生成機
	RandomGenerator mRand;
};

#define GAME_SYSTEM_INSTANCE GameSystem::GetInstance()
#define RENDERER_INSTANCE (*(GAME_SYSTEM_INSTANCE.GetRenderer()))
#define PHYSIC_MANAGER_INSTANCE (*(GAME_SYSTEM_INSTANCE.GetPhysicManager()))
#define RANDOM_GENERATOR (GAME_SYSTEM_INSTANCE.GetRand())

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
