#pragma once
#include "SceneBase.h"

class TestScene : public SceneBase
{
public:
	TestScene();

private:
	float mRadius;

	float mAngle;

	void UpdateScene() override;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
