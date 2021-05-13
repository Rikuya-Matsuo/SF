#include "ComponentBase.h"
#include "Actor.h"

ComponentBase::ComponentBase(Actor * owner, int priority) :
	mOwner(owner),
	mPriority(priority)
{
	owner->RegisterComponent(this);
}

ComponentBase::~ComponentBase()
{
}

void ComponentBase::Update()
{
	// 処理内容は継承先に依存する
}

void ComponentBase::SetPriority(int priority)
{
	mOwner->RequestSortComponents();
	mPriority = priority;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
