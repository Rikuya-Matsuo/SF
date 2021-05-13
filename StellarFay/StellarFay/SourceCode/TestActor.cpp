#include "TestActor.h"
#include "ObjMeshShaderWrapper.h"
#include "PhongShaderWrapper.h"
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
	mShaderWrapper = new PhongShaderWrapper();

	mShaderWrapper->UpdateUniformAddress("modelMat", &mModelMat);
	mShaderWrapper->UpdateUniformAddress("viewMat", &RENDERER_INSTANCE.GetActiveCamera()->GetViewMat());
	mShaderWrapper->UpdateUniformAddress("projectionMat", &RENDERER_INSTANCE.GetActiveCamera()->GetProjectionMat());

	const Vector3D * camPosAddress = &RENDERER_INSTANCE.GetActiveCamera()->GetPosition();
	mShaderWrapper->UpdateCameraPosRefUniform(camPosAddress);

	Mesh * msh = new Mesh();
	bool success = msh->Load("Assets/Handgun/Handgun_obj.obj");

	if (success)
	{
		MeshComponent * meshComp = new MeshComponent(this, msh, 0);

		meshComp->SetShader(mShaderWrapper);
	}
}

TestActor::~TestActor()
{
	delete mShaderWrapper;
}

void TestActor::UpdateActor()
{
	mAngle += MATH_PI * 0.5f * GAME_SYSTEM_INSTANCE.GetGameWorldDeltaTime();

	Quaternion q(mAxis, mAngle);

	mTransform.mRotation = q;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
