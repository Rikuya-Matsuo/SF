#pragma once
#include "Vector.h"

// UI層のクラス
class UILayerBase
{
public:
	UILayerBase(int drawOrder = 0, int updateOrder = 0);

	~UILayerBase();

	// 更新
	void Update();

	// 描画
	virtual void Draw(const class ElementBuffer * elemBuf) const;

	// 描画優先度の取得
	int GetDrawPriority() const { return mDrawPriority; }

	// 描画フラグの取得
	bool GetDrawFlag() const { return mDrawFlag; }

	// 描画優先度の設定
	void SetDrawPriority(int drawPriority);

protected:
	// 描画順
	// この数値が小さい方が順序として先に描画される
	// 言い換えると、この数値が大きい方が手前に表示される
	int mDrawPriority;
	
	// 描画フラグ
	bool mDrawFlag;

	// 継承先独自の更新
	virtual void UpdateUI();
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
