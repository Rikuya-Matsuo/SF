#pragma once
#include "DestroyableActor.h"

class Player : public DestroyableActor
{
public:
	Player();
	~Player();

private:
	class PhongShaderWrapper * mShaderWrapper;
};
