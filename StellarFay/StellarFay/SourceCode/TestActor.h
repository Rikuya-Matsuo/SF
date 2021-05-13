﻿#include "Actor.h"
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
