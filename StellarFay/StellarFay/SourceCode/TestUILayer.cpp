#include "TestUILayer.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "Texture.h"
#include "Sprite.h"
#include "ShaderWrapper.h"

TestUILayer::TestUILayer()
{
	Sprite * spr = new Sprite(this);

	const Texture * tex = RENDERER_INSTANCE.GetTexture("Assets/stellarFayLogoJPsub.png");
	spr->SetTexture(tex);

	ShaderWrapper * sw = spr->GetShaderWrapper();
	sw->SetUniformElement("dissolve", 1.0f);

	spr->UpdateMatrix();
}

void TestUILayer::Draw(const ElementBuffer * elemBuf) const
{
	UILayerBase::Draw(elemBuf);
}
