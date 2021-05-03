#include "ActorFollowCamera.h"
#include "Actor.h"

// ターゲットにはアクターの情報を代入するので、いったんZeroを代入する
ActorFollowCamera::ActorFollowCamera(const Actor * targetActor, const Vector3D & pos, const Vector3D & up) :
	Camera(pos, Vector3D::Zero, up),
	mTargetActor(targetActor)
{
	if (mTargetActor)
	{
		mFront = mTargetActor->GetPosition();
	}
}

ActorFollowCamera::~ActorFollowCamera()
{
}

void ActorFollowCamera::UpdateCamera()
{
	mFront = mTargetActor->GetPosition();
}
