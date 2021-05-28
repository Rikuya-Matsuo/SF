#include "TitleScene.h"
#include "GameSystem.h"
#include "Input.h"
#include "TitleUILayer.h"

TitleScene::TitleScene()
{
	mUI = new TitleUILayer();
}

TitleScene::~TitleScene()
{
	delete mUI;
}

void TitleScene::UpdateScene()
{
	if (INPUT_INSTANCE.GetKeyPressDown(SDL_SCANCODE_RETURN))
	{
		GoNextScene(SceneEnum::Game_SceneEnum);
	}
}
