#include "SpriteShaderWrapper.h"
#include "GameSystem.h"
#include "Renderer.h"

Shader * SpriteShaderWrapper::mSpriteShader = nullptr;

SpriteShaderWrapper::SpriteShaderWrapper(Shader * shader, Uint8 textureMass) :
	ShaderWrapper(nullptr, textureMass)
{
	if (!mSpriteShader)
	{
		mSpriteShader = RENDERER_INSTANCE.GetShader("Shaders/Sprite.vert", "Shaders/Sprite.frag");
	}

	mShader = (!shader) ? mSpriteShader : shader;

	mUniformList1i["tex"] = 0;

	mUniformAddressList4m["viewProjMat"] = &RENDERER_INSTANCE.GetUIViewProjMat();
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
