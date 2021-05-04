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

void PhongShaderWrapper::SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const
{
	// ディゾルブの転送
	mShader->SetUniform1f("dissolve", polyGroup->mUsemtl->GetDissolve());

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
