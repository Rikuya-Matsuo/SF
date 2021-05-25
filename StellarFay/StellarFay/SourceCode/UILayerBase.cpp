#include "UILayerBase.h"
#include "GameSystem.h"
#include "UILayerManager.h"
#include "ElementBuffer.h"

UILayerBase::UILayerBase(int drawOrder, int updateOrder) :
	mDrawPriority(drawOrder),
	mDrawFlag(true)
{
	UI_MANAGER_INSTANCE.RegisterUILayer(this);
}

UILayerBase::~UILayerBase()
{
	UI_MANAGER_INSTANCE.DeregisterUILayer(this);
}

void UILayerBase::Update()
{
}

void UILayerBase::Draw(const ElementBuffer * elemBuf) const
{
}

void UILayerBase::UpdateUI()
{
}

void UILayerBase::SetDrawPriority(int drawPriority)
{
	mDrawFlag = drawPriority;

	// マネージャにソートを要請する
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
