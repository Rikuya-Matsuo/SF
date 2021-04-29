#pragma once
#include "ShaderWrapper.h"

class ObjMeshShaderWrapper : public ShaderWrapper
{
public:
	ObjMeshShaderWrapper(Shader * shader = nullptr);

	~ObjMeshShaderWrapper();

	void SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const override;

private:
	static Shader * mObjMeshShader;
};
