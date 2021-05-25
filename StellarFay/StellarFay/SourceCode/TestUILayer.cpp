#include "TestUILayer.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "Texture.h"

TestUILayer::TestUILayer() :
	mTex(RENDERER_INSTANCE.GetTexture("Assets/stellarFayLogoJPsub.png"))
{
}

void TestUILayer::Draw(const ElementBuffer * elemBuf) const
{
	
}
