#include "Input.h"

Input::Input() :
	mQuitEventFlag(false)
{
	// キー状態にアクセスできるポインタを取得
	mKeyStates = SDL_GetKeyboardState(nullptr);

	// 前フレームのキー状態を初期化
	for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
	{
		mPrevKeyStates[i] = 0;
	}
}

Input::~Input()
{
}

void Input::Update()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		// 終了フラグを立てる
		if (sdlEvent.type == SDL_QUIT)
		{
			mQuitEventFlag = true;
		}
	}
}

void Input::LastUpdate()
{
	// キー状態保存
	SDL_memcpy(mPrevKeyStates, mKeyStates, SDL_NUM_SCANCODES);
}
