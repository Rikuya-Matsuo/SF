#pragma once
#include <SDL/SDL.h>

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Init(Uint32 windowWidth, Uint32 windowHeight, bool fullScreen);

private:
	Uint32 mWindowWidth;
	Uint32 mWindowHeight;

	SDL_Window * mWindow;

	SDL_Renderer * mSDLRenderer;

	SDL_GLContext mContext;
};
