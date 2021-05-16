#pragma once
#include "ShaderWrapper.h"

class MonoColorShaderWrapper : public ShaderWrapper
{
public:
	MonoColorShaderWrapper(Shader * shader = nullptr);

	void UpdateColorUniformElement(const Vector3D & color) { mUniformList3f["color"] = color; }
	void UpdateDissolveUniformElement(float dissolve) { mUniformList1f["dissolve"] = dissolve; }

	void SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const override;

	DissolveSetting GetDissolveSetting() const override;

private:
	static Shader * mMonoColorShader;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
