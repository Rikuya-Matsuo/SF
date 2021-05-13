#include <SDL/SDL.h>
#include <glad/glad.h>
#include "GameSystem.h"
#include "SceneBase.h"

int main(int argc, char ** argv)
{
	// 初期化
	bool successInit = GAME_SYSTEM_INSTANCE.Init();

	// 最初のシーンを設定
	SceneEnum firstScene = SceneEnum::Test_SceneEnum;
	SceneBase * scenePtr = SceneBase::GenerateScene(firstScene);

	GAME_SYSTEM_INSTANCE.SetFirstScene(scenePtr);

	// 上記２つが上手くいった場合ループ開始
	if (successInit && scenePtr)
	{
		GAME_SYSTEM_INSTANCE.Loop();
	}

	// 後片付け
	GAME_SYSTEM_INSTANCE.Finish();

	return 0;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
