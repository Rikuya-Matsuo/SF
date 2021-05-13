#include "Actor.h"
#include "Vector.h"

class TestActor : public Actor
{
public:
	TestActor();

	~TestActor();

private:
	class PhongShaderWrapper * mShaderWrapper;

	Vector3D mAxis;

	float mAngle;

	void UpdateActor() override;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
