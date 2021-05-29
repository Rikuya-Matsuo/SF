#include "Player.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "PhongShaderWrapper.h"

Player::Player() :
	DestroyableActor(0, 100)
{
	Mesh * msh = RENDERER_INSTANCE.GetMesh("Assets/SciFi_Fighter/MK6_OBJ.obj");

	MeshComponent * mshComp = new MeshComponent(this, msh, 0);

	mShaderWrapper = new PhongShaderWrapper();
	mShaderWrapper->SetMatricesReference(&mModelMat, RENDERER_INSTANCE);

	mshComp->SetShader(mShaderWrapper);

	mTransform.mScales.SetAll(0.003f);
}

Player::~Player()
{
	delete mShaderWrapper;
}
