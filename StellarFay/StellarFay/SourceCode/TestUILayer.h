#pragma once
#include "UILayerBase.h"

class TestUILayer : public UILayerBase
{
public:
	TestUILayer();

	void Draw(const class ElementBuffer * elemBuf) const override;

private:

};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
