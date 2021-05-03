#pragma once
#include "Camera.h"

class ActorFollowCamera : public Camera
{
public:
	ActorFollowCamera(const class Actor * targetActor, const Vector3D & pos = Vector3D::Zero, const Vector3D & up = Vector3D::UnitY);

	virtual ~ActorFollowCamera();

	void SetTargetActor(const class Actor * targetActor) { mTargetActor = targetActor; }

protected:
	const class Actor * mTargetActor;

	// アクターからターゲットベクトルの情報を設定する
	void UpdateCamera() override;
};
