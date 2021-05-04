#include "TestActor.h"
#include "ObjMeshShaderWrapper.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "Camera.h"

TestActor::TestActor() :
	Actor(0),
	mAxis(Vector3D::UnitY),
	mAngle(0.0f)
{
	mShader = new ObjMeshShaderWrapper();

	mShader->UpdateUniformAddress("modelMat", &mModelMat);
	mShader->UpdateUniformAddress("viewMat", &RENDERER_INSTANCE.GetActiveCamera()->GetViewMat());
	mShader->UpdateUniformAddress("projectionMat", &RENDERER_INSTANCE.GetActiveCamera()->GetProjectionMat());

	Mesh * msh = new Mesh();
	bool success = msh->Load("Assets/Handgun/Handgun_obj.obj");

	if (success)
	{
		MeshComponent * meshComp = new MeshComponent(this, msh, 0);

		meshComp->SetShader(mShader);
	}
}

TestActor::~TestActor()
{
}

void TestActor::UpdateActor()
{
	mAngle += MATH_PI * 1 * GAME_SYSTEM_INSTANCE.GetGameWorldDeltaTime();

	Quaternion q(mAxis, mAngle);

	mRotation = q;
}
