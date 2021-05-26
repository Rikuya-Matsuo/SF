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

	for (auto itr : mSprites)
	{
		delete itr;
		itr = nullptr;
	}
	mSprites.clear();
}

void UILayerBase::Update()
{
}

void UILayerBase::Draw(const ElementBuffer * elemBuf) const
{
	for (auto itr : mSprites)
	{
		itr->Draw(elemBuf);
	}
}

void UILayerBase::UpdateUI()
{
}

void UILayerBase::SetDrawPriority(int drawPriority)
{
	mDrawFlag = drawPriority;

	// マネージャにソートを要請する
	UI_MANAGER_INSTANCE.RequestSortDrawList();
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
