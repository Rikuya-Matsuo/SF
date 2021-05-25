#pragma once
#include "UILayerBase.h"

class TestUILayer : public UILayerBase
{
public:
	TestUILayer();

	void Draw(const class ElementBuffer * elemBuf) const override;

private:
	class Texture * mTex;

	class ShaderWrapper * mShaderWrapper;
};
