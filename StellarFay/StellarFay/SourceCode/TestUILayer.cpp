#include "TestUILayer.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "Texture.h"
#include "SpriteShaderWrapper.h"
#include "Camera.h"

TestUILayer::TestUILayer() :
	mTex(RENDERER_INSTANCE.GetTexture("Assets/stellarFayLogoJPsub.png"))
{
	mShaderWrapper = new SpriteShaderWrapper();

	Vector2D scale = Vector2D(
		static_cast<float>(RENDERER_INSTANCE.GetWidth()) * 2.f,
		static_cast<float>(RENDERER_INSTANCE.GetHeight()) * 2.f
	);
	Matrix4 mat = Matrix4::CreateScale(scale.x, scale.y, 1.0f);
	mShaderWrapper->SetUniformElement("modelMat", mat);

	const Matrix4 & viewProjMat = RENDERER_INSTANCE.GetUIViewProjMat();
	mShaderWrapper->SetUniformAddress("viewProjMat", &viewProjMat);

	mShaderWrapper->SetUniformElement("dissolve", 1.0f);

	mShaderWrapper->SetTexture(mTex, 0);
}

void TestUILayer::Draw(const ElementBuffer * elemBuf) const
{
	mShaderWrapper->Activate();

	mShaderWrapper->InputUniforms();

	elemBuf->DrawElements();
}
