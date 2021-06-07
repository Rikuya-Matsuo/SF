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

	// 機首の最大角度
	static const Vector2D mMaxNose;
};
