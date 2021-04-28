#include "ShaderWrapper.h"

ShaderWrapper::ShaderWrapper(Shader * shader) :
	mShader(shader)
{
}

ShaderWrapper::~ShaderWrapper()
{
}

void ShaderWrapper::SetUniforms() const
{
	for (auto itr : mUniformList1f)
	{
		mShader->SetUniform1f(itr.first, itr.second);
	}

	for (auto itr : mUniformList3f)
	{
		mShader->SetUniform3fv(itr.first, itr.second.GetAsFloatPtr());
	}

	for (auto itr : mUniformList1i)
	{
		mShader->SetUniform1i(itr.first, itr.second);
	}
}

void ShaderWrapper::SetPolyUniforms(const Mesh::ObjectData::PolyGroup & polyGroup) const
{
}
