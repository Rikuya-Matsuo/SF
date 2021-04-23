#include "MeshComponent.h"
#include "Actor.h"
#include "Mesh.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "Shader.h"

Shader * MeshComponent::mInitialShader = nullptr;

MeshComponent::MeshComponent(Actor * owner, Mesh * msh, int drawPriority) :
	ComponentBase(owner, 1000),
	mMesh(msh),
	mDrawPriority(drawPriority)
{
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::SetInitialShader(const std::string & vertFilePath, const std::string & fragFilePath)
{
	Shader * shd = RENDERER_INSTANCE.GetShader(vertFilePath, fragFilePath);
	SetInitialShader(shd);
}

void MeshComponent::DrawFullDissolveObject(Shader * shader) const
{
	
}

void MeshComponent::DrawUnderCondition(Shader * shader, std::function<bool(Mesh::ObjectData*obj, size_t polyGroupIndex)> condition) const
{
	mMesh->mVertexArray->Activate();
	for (auto objItr : mMesh->mObjects)
	{
		//if (objItr->)
	}
}
