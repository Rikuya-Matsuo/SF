#include "ObjMeshShaderWrapper.h"
#include "GameSystem.h"
#include "Renderer.h"

Shader * ObjMeshShaderWrapper::mObjMeshShader = nullptr;

ObjMeshShaderWrapper::ObjMeshShaderWrapper(Shader * shader)
{
	if (!mObjMeshShader)
	{
		mObjMeshShader = RENDERER_INSTANCE.GetShader("Shaders/ObjMesh.vert", "Shaders/ObjMesh.frag");
	}

	mShader = (!shader) ? mObjMeshShader : shader;
}

ObjMeshShaderWrapper::~ObjMeshShaderWrapper()
{
}

void ObjMeshShaderWrapper::SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const
{
	const Mesh::MtlData * mtl = polyGroup->mUsemtl;

	// ディゾルブ転送
	mShader->SetUniform1f("dissolve", mtl->GetDissolve());

	// テクスチャ転送
	mShader->SetUniform1i("tex0", 0);
	Texture::ActivateTextureUnit(0);

	const Texture * tex = mtl->GetDiffuseTexture();
	if (tex)
	{
		tex->Activate();
	}
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
