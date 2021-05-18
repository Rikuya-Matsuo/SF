#include "MonoColorShaderWrapper.h"
#include "GameSystem.h"
#include "Renderer.h"

Shader * MonoColorShaderWrapper::mMonoColorShader = nullptr;

MonoColorShaderWrapper::MonoColorShaderWrapper(Shader * shader) :
	ShaderWrapper()
{
	if (!mMonoColorShader)
	{
		mMonoColorShader = RENDERER_INSTANCE.GetShader("Shaders/1color.vert", "Shaders/1color.frag");
	}

	mShader = (!shader) ? mMonoColorShader : shader;

	// 例外回避のため、初期値としてディゾルブを設定
	mUniformList1f["dissolve"] = 1.0f;
}

void MonoColorShaderWrapper::InputPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const
{
	const char * dissolveKey = "dissolve";
	float dissolve = mUniformList1f.at(dissolveKey);

	if (!polyGroup->mUsemtl->GetDissolve())
	{
		dissolve = 0.0f;
	}

	mShader->SetUniform1f(dissolveKey, dissolve);
}

ShaderWrapper::DissolveSetting MonoColorShaderWrapper::GetDissolveSetting() const
{
	float dissolve = mUniformList1f.at("dissolve");

	DissolveSetting setting =
		(dissolve == 1.0f) ?
		DissolveSetting::AllFullDissolve :
		DissolveSetting::AllNotFullDissolve;

	return setting;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
