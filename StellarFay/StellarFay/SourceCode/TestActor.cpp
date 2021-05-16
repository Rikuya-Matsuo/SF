#include "TestActor.h"
#include "ObjMeshShaderWrapper.h"
#include "PhongShaderWrapper.h"
#include "MonoColorShaderWrapper.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "Camera.h"

TestActor::TestActor() :
	Actor(0),
	mAxis(Vector3D::UnitY),
	mAngle(0.0f),
	mShaderWrapper(nullptr),
	mShaderWrapper2(nullptr)
{
	ShaderWrapper * swptr = nullptr;

//#define PHONG
#ifdef PHONG

	mShaderWrapper = new PhongShaderWrapper();

	mShaderWrapper->UpdateUniformAddress("modelMat", &mModelMat);
	mShaderWrapper->UpdateUniformAddress("viewMat", &RENDERER_INSTANCE.GetActiveCamera()->GetViewMat());
	mShaderWrapper->UpdateUniformAddress("projectionMat", &RENDERER_INSTANCE.GetActiveCamera()->GetProjectionMat());

	const Vector3D * camPosAddress = &RENDERER_INSTANCE.GetActiveCamera()->GetPosition();
	mShaderWrapper->UpdateCameraPosRefUniform(camPosAddress);


	swptr = mShaderWrapper;

#else

	Shader * sh = RENDERER_INSTANCE.GetShader("Shaders/1color.vert", "Shaders/1color.frag");

	mShaderWrapper2 = new MonoColorShaderWrapper(sh);

	mShaderWrapper2->UpdateUniformElement("color", Vector3D(0.0f, 0.36f, 0.78f));
	mShaderWrapper2->UpdateUniformElement("dissolve", 0.9f);

	mShaderWrapper2->UpdateUniformAddress("modelMat", &mModelMat);
	mShaderWrapper2->UpdateUniformAddress("viewMat", &RENDERER_INSTANCE.GetActiveCamera()->GetViewMat());
	mShaderWrapper2->UpdateUniformAddress("projectionMat", &RENDERER_INSTANCE.GetActiveCamera()->GetProjectionMat());

	swptr = mShaderWrapper2;

#endif

	Mesh * msh = new Mesh();
	bool success = msh->Load("Assets/Handgun/Handgun_obj.obj");

	if (success)
	{
		MeshComponent * meshComp = new MeshComponent(this, msh, 0);

		meshComp->SetShader(swptr);
	}
}

TestActor::~TestActor()
{
	if (mShaderWrapper)
	{
		delete mShaderWrapper;
	}

	if (mShaderWrapper2)
	{
		delete mShaderWrapper2;
	}
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
