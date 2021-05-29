#include "GameScene.h"
#include "Player.h"
#include "ActorFollowCamera.h"

GameScene::GameScene()
{
	ActorFollowCamera * cam =
		new ActorFollowCamera(nullptr, Vector3D(0.0f, 0.0f, -10.f), Vector3D::UnitY);

	cam->Activate();

	Actor * pl = new Player();

	cam->SetTargetActor(pl);

	mCameras.emplace_back(cam);
}

GameScene::~GameScene()
{
}
