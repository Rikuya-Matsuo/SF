#include "TitleScene.h"
#include "GameSystem.h"
#include "Input.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::UpdateScene()
{
	if (INPUT_INSTANCE.GetKeyPressDown(SDL_SCANCODE_RETURN))
	{
		GoNextScene();
	}
}
