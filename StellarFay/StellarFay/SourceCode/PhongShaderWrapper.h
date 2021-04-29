#pragma once
#include "ShaderWrapper.h"

class PhongShaderWrapper : public ShaderWrapper
{
public:
	// nullptrを渡す場合は、通常のPhongシェーダをラップする
	PhongShaderWrapper(Shader * shader = nullptr);

	void SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const override;

private:
	static Shader * mPhongShader;
};
