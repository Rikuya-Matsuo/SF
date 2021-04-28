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
}

void PhongShaderWrapper::SetPolyUniforms(const Mesh::ObjectData::PolyGroup & polyGroup) const
{
	// ディゾルブの転送
	mShader->SetUniform1f("dissolve", polyGroup.mUsemtl->GetDissolve());
}
