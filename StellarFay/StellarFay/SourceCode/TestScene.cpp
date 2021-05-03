#include "TestScene.h"
#include "TestActor.h"
#include "ActorFollowCamera.h"

TestScene::TestScene() :
	SceneBase()
{
	Vector3D camPos(0.0f, 0.0f, 6.0f);
	ActorFollowCamera * camera = new ActorFollowCamera(nullptr, camPos);

	mCameras.emplace_back(camera);
	camera->Activate();

	Actor * a = new TestActor();

	camera->SetTargetActor(a);
}
