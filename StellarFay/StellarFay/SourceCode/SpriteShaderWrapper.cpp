#include "SpriteShaderWrapper.h"
#include "GameSystem.h"
#include "Renderer.h"

Shader * SpriteShaderWrapper::mSpriteShader = nullptr;

SpriteShaderWrapper::SpriteShaderWrapper(Shader * shader) :
	ShaderWrapper()
{
	if (!mSpriteShader)
	{
		mSpriteShader = RENDERER_INSTANCE.GetShader("Shaders/Sprite.vert", "Shaders/Sprite.frag");
	}

	mShader = (!shader) ? mSpriteShader : shader;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
