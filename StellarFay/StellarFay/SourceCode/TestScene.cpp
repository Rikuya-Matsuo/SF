#include "TestScene.h"
#include "TestActor.h"
#include "ActorFollowCamera.h"
#include "GameSystem.h"

TestScene::TestScene() :
	SceneBase()
{
	mAngle = 0.0f;
	mRadius = 6.0f;

	Vector3D camPos(0.0f, 0.0f, mRadius);
	ActorFollowCamera * camera = new ActorFollowCamera(nullptr, camPos);

	mCameras.emplace_back(camera);
	camera->Activate();

	Actor * a = new TestActor();

	camera->SetTargetActor(a);
}

void TestScene::UpdateScene()
{
	mAngle += MATH_PI / 4 * GAME_SYSTEM_INSTANCE.GetGameWorldDeltaTime();

	Camera * c = mCameras.back();

	Vector3D pos = c->GetPosition();
	pos.x = sinf(mAngle) * mRadius;
	pos.z = cosf(mAngle) * mRadius;

	//c->SetPosition(pos);
}
