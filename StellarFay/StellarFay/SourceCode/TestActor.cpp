#include "TestActor.h"
#include "ObjMeshShaderWrapper.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "GameSystem.h"
#include "Renderer.h"

TestActor::TestActor() :
	Actor(0)
{
	mShader = new ObjMeshShaderWrapper();

	mShader->UpdateUniformAddress("model", &mModelMat);
	mShader->UpdateUniformAddress("projection", &RENDERER_INSTANCE.GetProjectionMat());

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
