#pragma once
#include "DestroyableActor.h"

class Player : public DestroyableActor
{
public:
	Player();
	~Player();

	void UpdateActor() override;

private:
	class PhongShaderWrapper * mShaderWrapper;
};
