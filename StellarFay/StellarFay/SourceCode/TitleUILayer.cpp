#include "TitleUILayer.h"
#include "Sprite.h"
#include "ShaderWrapper.h"
#include "GameSystem.h"
#include "Renderer.h"

TitleUILayer::TitleUILayer() :
	UILayerBase()
{
	Sprite * logo = new Sprite(this);

	Texture * tex = RENDERER_INSTANCE.GetTexture("Assets/stellarFayLogoJPsub.png");
	logo->SetTexture(tex);

	logo->UpdateMatrix();
}

TitleUILayer::~TitleUILayer()
{
}
