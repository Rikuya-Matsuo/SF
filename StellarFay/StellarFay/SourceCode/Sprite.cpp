#include "Sprite.h"
#include "SpriteShaderWrapper.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "UILayerBase.h"

Sprite::Sprite(UILayerBase * uiLayer, ShaderWrapper * shaderWrapper) :
	mPosition(Vector2D::Zero),
	mScale(1.0f)
{
	// 引数がnullptrであった場合、スプライトシェーダのラッパーを作成
	if (!shaderWrapper)
	{
		mShaderWrapper = new SpriteShaderWrapper();
	}
	// そうでない場合は指定のシェーダラッパーを使う
	else
	{
		mShaderWrapper = shaderWrapper;
	}

	mShaderWrapper->SetUniformAddress("modelMat", &mModelMat);
	SetDissolve(1.0f);

	uiLayer->RegisterSprite(this);
}

Sprite::~Sprite()
{
}

void Sprite::SetTexture(const Texture * tex, Uint8 texUnitIndex)
{
	if (mShaderWrapper)
	{
		mShaderWrapper->SetTexture(tex, texUnitIndex);
	}
}

void Sprite::SetDissolve(float value)
{
	mShaderWrapper->SetUniformElement("dissolve", value);
}

void Sprite::UpdateMatrix()
{
	mModelMat = Matrix4::CreateScale(
		static_cast<float>(RENDERER_INSTANCE.GetWidth()) * mScale.x,
		static_cast<float>(RENDERER_INSTANCE.GetHeight()) * mScale.y,
		1.0f);

	mModelMat *= Matrix4::CreateTranslation(Vector3D(mPosition.x, mPosition.y, 0.0f));
}

void Sprite::Draw(const ElementBuffer * elemBuf) const
{
	mShaderWrapper->Activate();

	mShaderWrapper->InputUniforms();

	elemBuf->DrawElements();
}
