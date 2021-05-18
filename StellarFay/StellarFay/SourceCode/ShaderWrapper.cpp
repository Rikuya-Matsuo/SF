#include "ShaderWrapper.h"

ShaderWrapper::ShaderWrapper(Shader * shader) :
	mShader(shader),
	mDissolveSetting(DissolveSetting::Default)
{
}

ShaderWrapper::~ShaderWrapper()
{
}

void ShaderWrapper::InputUniforms() const
{
	for (auto itr : mUniformList1f)
	{
		mShader->SetUniform1f(itr.first, itr.second);
	}

	for (auto itr : mUniformAddressList1f)
	{
		mShader->SetUniform1f(itr.first, *itr.second);
	}

	for (auto itr : mUniformList3f)
	{
		mShader->SetUniform3fv(itr.first, itr.second.GetAsFloatPtr());
	}

	for (auto itr : mUniformAddressList3f)
	{
		mShader->SetUniform3fv(itr.first, itr.second->GetAsFloatPtr());
	}

	for (auto itr : mUniformList1i)
	{
		mShader->SetUniform1i(itr.first, itr.second);
	}

	for (auto itr : mUniformAddressList1i)
	{
		mShader->SetUniform1i(itr.first, *itr.second);
	}

	for (auto itr : mUniformList4m)
	{
		mShader->SetUniform4m(itr.first, itr.second.GetAsFloatPtr());
	}

	for (auto itr : mUniformAddressList4m)
	{
		mShader->SetUniform4m(itr.first, itr.second->GetAsFloatPtr());
	}
}

void ShaderWrapper::InputPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const
{
}

ShaderWrapper::DissolveSetting ShaderWrapper::GetDissolveSetting() const
{
	return mDissolveSetting;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
