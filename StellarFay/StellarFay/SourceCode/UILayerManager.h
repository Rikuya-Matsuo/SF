#pragma once
#include <list>

// UIのレイヤーを管理するクラス
class UILayerManager final
{
public:
	UILayerManager();
	~UILayerManager();

	// 更新
	void Update();

	// 描画
	void Draw(const class ElementBuffer * elemBuf);

	// UIの登録
	void RegisterUILayer(class UILayerBase * ui);

	// UIの登録解除
	void DeregisterUILayer(class UILayerBase * ui);

	// 描画リストのソート要請
	void RequestSortDrawList() { mSortDrawListFlag = true; }

	// 更新リストのソート要請
	void RequestSortUpdateList() { mSortUpdateListFlag = true; }

private:
	// 描画用リスト
	std::list<class UILayerBase *> mDrawList;

	// 描画リストソートフラグ
	bool mSortDrawListFlag;

	// 更新リストソートフラグ
	bool mSortUpdateListFlag;

	// 描画用リストへの登録
	void RegisterIntoDrawList(class UILayerBase * ui);
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
