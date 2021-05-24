#include "UILayerBase.h"

UILayerBase::UILayerBase(int drawOrder, int updateOrder) :
	mDrawPriority(drawOrder),
	mDrawFlag(true)
{
}

void UILayerBase::Update()
{
}

void UILayerBase::Draw() const
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
