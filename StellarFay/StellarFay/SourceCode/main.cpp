#include <SDL/SDL.h>
#include <glad/glad.h>
#include "GameSystem.h"

int main(int argc, char ** argv)
{
	bool success = GAME_SYSTEM_INSTANCE.Init();

	if (success)
	{
		GAME_SYSTEM_INSTANCE.Loop();
	}

	GAME_SYSTEM_INSTANCE.Finish();

	return 0;
}
