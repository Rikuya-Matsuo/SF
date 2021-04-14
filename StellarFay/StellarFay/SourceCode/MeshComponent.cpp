#include "MeshComponent.h"
#include "Actor.h"
#include "Mesh.h"

MeshComponent::MeshComponent(Actor * owner, Mesh * msh, int drawPriority) :
	ComponentBase(owner, 1000),
	mMesh(msh),
	mDrawPriority(drawPriority)
{
}

MeshComponent::~MeshComponent()
{
}
