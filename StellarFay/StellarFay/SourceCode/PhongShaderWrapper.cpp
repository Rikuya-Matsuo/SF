#include "PhongShaderWrapper.h"
#include "GameSystem.h"
#include "Renderer.h"

Shader * PhongShaderWrapper::mPhongShader = nullptr;

PhongShaderWrapper::PhongShaderWrapper(Shader * shader) :
	ShaderWrapper()
{
	// Phongシェーダをまだ取得していなければ取得
	if (!mPhongShader)
	{
		mPhongShader = RENDERER_INSTANCE.GetShader("Shaders/Phong.vert", "Shaders/Phong.frag");
	}

	// 引数がnullptrならPhongシェーダを使う
	mShader = (!shader) ? mPhongShader : shader;

	// ライト情報の参照を予め設定しておく
	LightInfo & lightInfoRef = RENDERER_INSTANCE.GetLightInfo();
	mUniformAddressList3f["light.direction"] = &lightInfoRef.mDirection;
	mUniformAddressList3f["light.diffuse"] = &lightInfoRef.mDiffuseColor;
	mUniformAddressList3f["light.ambient"] = &lightInfoRef.mAmbientColor;
	mUniformAddressList3f["light.specular"] = &lightInfoRef.mSpecularColor;
}

void PhongShaderWrapper::InputUniforms() const
{
	// 基底クラスの同関数を呼ぶ
	ShaderWrapper::InputUniforms();

	// 逆行列のセット
	Matrix4 invMat;
	static const char * modelKey = "modelMat";

	// モデル行列を探す
	// 参照から
	auto itrRef = mUniformAddressList4m.find(modelKey);
	if (itrRef != mUniformAddressList4m.end())
	{
		invMat = *itrRef->second;
	}

	// 見つからなかった場合行列型のマップから
	else
	{
		auto itr = mUniformList4m.find(modelKey);
		if (itr != mUniformList4m.end())
		{
			invMat = itr->second;
		}
	}

	// ここで逆行列化
	invMat.Invert();

	// ユニフォーム転送
	mShader->SetUniform4m("invertedModelMat", invMat.GetAsFloatPtr());
}

void PhongShaderWrapper::InputPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const
{
	// ディゾルブの転送
	mShader->SetUniform1f("dissolve", polyGroup->mUsemtl->GetDissolve());

	// スペキュラーの光沢度の転送
	mShader->SetUniform1f("material.shininess", polyGroup->mUsemtl->GetShininess());

	// ディフューズマップの設定
	mShader->SetUniform1i("material.diffuseTex", 0);
	const Texture * dtex = polyGroup->mUsemtl->GetDiffuseTexture();
	if (dtex)
	{
		Texture::ActivateTextureUnit(0);
		dtex->Activate();
	}

	// スペキュラーマップの設定
	mShader->SetUniform1i("material.specular", 1);
	const Texture * stex = polyGroup->mUsemtl->GetSpecularTexture();
	if (stex)
	{
		Texture::ActivateTextureUnit(1);
		stex->Activate();
	}
}

void PhongShaderWrapper::UpdateMatricesReference(const Matrix4 * modelMatAddress, const Renderer & renderer)
{
}

void PhongShaderWrapper::UpdateMatricesReference(const Matrix4 * modelMatAddress, const Matrix4 * viewMat, const Matrix4 * projMat)
{
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
