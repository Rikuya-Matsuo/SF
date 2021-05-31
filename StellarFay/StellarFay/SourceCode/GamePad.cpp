#include "GamePad.h"

float GamePad::mStickDeadZone = 0.2f;

GamePad::GamePad(int index) :
	mButtons(0),
	mPrevButtons(0)
{
	mGamePad = SDL_GameControllerOpen(index);

	if (mGamePad)
	{
		mMapping = SDL_GameControllerMapping(mGamePad);
	}
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

void GamePad::Update()
{

}

void GamePad::LastUpdate()
{
	mPrevButtons = mButtons;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
