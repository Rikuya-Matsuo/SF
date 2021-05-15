#pragma once
#include "ShaderWrapper.h"

class MonoColorShaderWrapper : public ShaderWrapper
{
public:
	MonoColorShaderWrapper(Shader * shader = nullptr);

	void UpdateColorUniformElement(const Vector3D & color) { mUniformList3f["color"] = color; }
	void UpdateDissolveUniformElement(float dissolve) { mUniformList1f["dissolve"] = dissolve; }

	void SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const override;

private:
	static Shader * mMonoColorShader;
};
