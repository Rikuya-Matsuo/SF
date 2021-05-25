#include "ShaderWrapper.h"
#include "Texture.h"

ShaderWrapper::ShaderWrapper(Shader * shader, Uint8 textureMass) :
	mShader(shader),
	mTextures(nullptr),
	mTextureMass(textureMass),
	mDissolveSetting(DissolveSetting::Default)
{
	// テクスチャポインタ配列を作らない場合はこの関数を抜ける
	if (!mTextureMass)
	{
		return;
	}

	// テクスチャへのポインタ配列を作成
	mTextures = new const Texture*[mTextureMass];

	// 初期化
	for (Uint8 i = 0; i < mTextureMass; ++i)
	{
		mTextures[i] = nullptr;
	}
}

ShaderWrapper::~ShaderWrapper()
{
	// テクスチャポインタ配列のメモリ解放
	if (mTextureMass && mTextures)
	{
		delete[] mTextures;
		mTextures = nullptr;
	}
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

	InputTextureUnits();
}

void ShaderWrapper::InputPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const
{
}

void ShaderWrapper::SetTexture(const Texture * tex, Uint8 unitIndex)
{
	// 指定されたテクスチャユニットのインデックス値が、
	// テクスチャポインタ配列の要素数以上なら関数を抜ける
	if (unitIndex >= mTextureMass)
	{
		return;
	}

	mTextures[unitIndex] = tex;
}

ShaderWrapper::DissolveSetting ShaderWrapper::GetDissolveSetting() const
{
	return mDissolveSetting;
}

void ShaderWrapper::InputTextureUnits() const
{
	for (Uint8 i = 0; i < mTextureMass; ++i)
	{
		if (!mTextures[i])
		{
			break;
		}

		Texture::ActivateTextureUnit(i);
		mTextures[i]->Activate();
	}
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
