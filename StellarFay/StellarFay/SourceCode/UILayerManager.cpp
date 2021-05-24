#include "UILayerManager.h"
#include "UILayerBase.h"
#include <algorithm>

UILayerManager::UILayerManager() :
	mSortDrawListFlag(false)
{
}

UILayerManager::~UILayerManager()
{
}

void UILayerManager::Update()
{
	// ソート要請を受けていればソートを行う
	if (mSortDrawListFlag)
	{
		// 描画順を比較するラムダ式
		auto priorityOrder = [](const UILayerBase * lhs, const UILayerBase * rhs)
		{
			return (lhs->GetDrawPriority() < rhs->GetDrawPriority());
		};

		// 描画順でソート
		mDrawList.sort(priorityOrder);

		// ソート要請フラグを偽に
		mSortDrawListFlag = false;
	}

	// 一番手前に描画されるものを取得
	// 反対側から走査した際に、最も早く描画フラグが真だったもの
	auto itr = mDrawList.rbegin();
	for (; itr != mDrawList.rend(); ++itr)
	{
		if ((*itr)->GetDrawFlag())
		{
			break;
		}
	}

	// 描画フラグが真のものがなかった場合、関数を終了
	if (itr == mDrawList.rend())
	{
		return;
	}

	// 更新
	(*itr)->Update();
}

void UILayerManager::Draw()
{
	// リストの順番に従って描画していく
	for (auto itr : mDrawList)
	{
		if (itr->GetDrawFlag())
		{
			itr->Draw();
		}
	}
}

void UILayerManager::RegisterUILayer(UILayerBase * ui)
{
	// 描画リストへの登録
	RegisterIntoDrawList(ui);
}

void UILayerManager::DeregisterUILayer(UILayerBase * ui)
{
	// リスト内から同じものを検索する
	auto itr = std::find(mDrawList.begin(), mDrawList.end(), ui);

	// 検索にヒットした場合、リストから抹消する
	if (itr != mDrawList.end())
	{
		mDrawList.erase(itr);
	}
}

void UILayerManager::RegisterIntoDrawList(UILayerBase * ui)
{
	// 描画順を取得
	const int drawPriority = ui->GetDrawPriority();

	// 描画順の値が小さい順になるように挿入
	auto itr = mDrawList.begin();
	for (; itr != mDrawList.end(); ++itr)
	{
		if (drawPriority < (*itr)->GetDrawPriority())
		{
			mDrawList.insert(itr, ui);
			return;
		}
	}

	// ここまでに挿入できていなければ最後尾に追加する
	mDrawList.emplace_back(ui);
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
