#include "SceneBase.h"
#include "Actor.h"

SceneBase * SceneBase::mLatestScene = nullptr;

SceneBase::SceneBase()
{
	mLatestScene = this;
}

void SceneBase::GetLatestScene(Actor * actor)
{
	actor->SetBelongScene(mLatestScene);
}
