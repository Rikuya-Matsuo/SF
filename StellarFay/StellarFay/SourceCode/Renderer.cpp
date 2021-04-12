#include <cstdio>
#include "Renderer.h"


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init(Uint32 windowWidth, Uint32 windowHeight, bool fullScreen)
{
	// ウィンドウサイズを記録
	mWindowWidth = windowWidth;
	mWindowHeight = windowHeight;

	// OpenGL アトリビュートのセット
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// GL version 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// 8bit RGBAチャンネル
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// ダブルバッファリング
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// ハードウェアアクセラレーションを強制
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// ウィンドウ作成
	mWindow = SDL_CreateWindow("StellarFay", 0, 0,
		mWindowWidth, mWindowHeight, SDL_WINDOW_OPENGL);

	// レンダラー作成
	mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mSDLRenderer)
	{
		printf("SDLRenderer作成失敗：\n%s\n", SDL_GetError());
		return false;
	}

	// コンテクスト作成
	mContext = SDL_GL_CreateContext(mWindow);

	return false;
}
