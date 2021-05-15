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
}

void MonoColorShaderWrapper::SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const
{
	const char * dissolveKey = "dissolve";
	float dissolve = mUniformList1f.at(dissolveKey);

	if (!polyGroup->mUsemtl->GetDissolve())
	{
		dissolve = 0.0f;
	}

	mShader->SetUniform1f(dissolveKey, dissolve);
}
