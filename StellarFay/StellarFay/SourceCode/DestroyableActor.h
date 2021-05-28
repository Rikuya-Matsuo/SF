#pragma once
#include "Actor.h"

// アクターの中でも、ヒットポイントを持つもの
class DestroyableActor : public Actor
{
	// 型だけ先に定義する
protected:
	// ヒットポイントの型
	typedef float HitPoint;

public:
	// コンストラクタ（hpがヒットポイントの初期値でかつ最大値となる）
	DestroyableActor(int priority, HitPoint hp) : DestroyableActor(priority, hp, hp) {}

	// コンストラクタ（ヒットポイントの初期値と最大値を分けることができる）
	DestroyableActor(int priority, HitPoint hp, HitPoint maxHp);

	virtual ~DestroyableActor();

	// ダメージを受ける
	// 負の値を入力すれば回復させることもできるが、可読性の観点から推奨しない。
	// 回復の場合Recover()を使うことを推奨。
	void Damage(HitPoint damage);

	// ヒットポイントを回復する
	void Recover(HitPoint recover) { Damage(-recover); }

	// ヒットポイント全回復
	void MaxRecover() { mHitPoint = mMaxHitPoint; }

	// ヒットポイント取得
	int GetHitPoint() const { return static_cast<int>(mHitPoint); }

	// 最大ヒットポイント取得
	int GetMaxHitPoint() const { return static_cast<int>(mMaxHitPoint); }

	// 生存フラグ取得
	bool GetAliveFlag() const { return mAliveFlag; }

	// 生存フラグ更新
	// 自動で呼び出されたりはしないので、継承先に呼んでもらう必要がある
	void UpdateAliveFlag() { mAliveFlag = (mHitPoint > 0); }

protected:
	// ヒットポイント
	HitPoint mHitPoint;

	// 最大ヒットポイント
	HitPoint mMaxHitPoint;

	// 生存フラグ
	// これが偽の場合の挙動は継承先に依存する
	bool mAliveFlag;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
