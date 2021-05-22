#pragma once
#include "ShaderWrapper.h"

class SpriteShaderWrapper : public ShaderWrapper
{
public:
	SpriteShaderWrapper(Shader * shader = nullptr);

private:
	static Shader * mSpriteShader;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
