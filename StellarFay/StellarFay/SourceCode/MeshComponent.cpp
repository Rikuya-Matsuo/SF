#include "MeshComponent.h"
#include "Actor.h"
#include "Mesh.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "ShaderWrapper.h"

// 名前空間の省略
namespace mcfm = MeshComponentFlagMask;

ShaderWrapper * MeshComponent::mInitialShader = nullptr;

MeshComponent::MeshComponent(Actor * owner, Mesh * msh, int drawPriority) :
	ComponentBase(owner, 1000),
	mMesh(msh),
	mDrawPriority(drawPriority),
	mMeshCompFlags(mcfm::mInitialFlag)
{
	RENDERER_INSTANCE.RegisterMeshComponent(this);
}

MeshComponent::~MeshComponent()
{
	RENDERER_INSTANCE.DeregisterMeshComponent(this);
}

void MeshComponent::SetInitialShader(const std::string & vertFilePath, const std::string & fragFilePath)
{
	//ShaderWrapper * shd = RENDERER_INSTANCE.GetShader(vertFilePath, fragFilePath);
	//SetInitialShader(shd);
}

void MeshComponent::DrawFullDissolveObject(ShaderWrapper * shader) const
{
	// ディゾルブが1であるかを検証するラムダ式
	auto isFullDissolve = [](Mesh::ObjectData * obj, size_t polyGroupIndex)
	{
		const Mesh::MtlData * mtl = obj->GetPolyGroups()[polyGroupIndex]->mUsemtl;
		return (mtl->GetDissolve() == 1.0f);
	};

	// 透明度に関する設定を取得
	using DisSet = ShaderWrapper::DissolveSetting;
	DisSet setting = shader->GetDissolveSetting();
	
	// 「全てのポリゴンを透明・半透明としてみなす」場合、何も描画しない
	if (setting == DisSet::AllNotFullDissolve)
	{
		return;
	}

	// 全てのポリゴンを不透明とみなすか
	bool isAllFullDissolve = (setting == DisSet::AllFullDissolve);

	// 状況ごとに条件を指定して描画
	if (!isAllFullDissolve)
	{
		DrawUnderCondition(shader, isFullDissolve);
	}
	else
	{
		// 常に真を返すラムダ式
		auto allTrue = [](Mesh::ObjectData * obj, size_t polyGroupIndex)
		{
			return true;
		};

		DrawUnderCondition(shader, allTrue);
	}
}

void MeshComponent::DrawNotFullDissolveObject(ShaderWrapper * shader) const
{
	// ディゾルブが1未満であるかを検証するラムダ式
	auto isNotFullDissolve = [](Mesh::ObjectData * obj, size_t polyGroupIndex)
	{
		const Mesh::MtlData * mtl = obj->GetPolyGroups()[polyGroupIndex]->mUsemtl;
		float dissolve = mtl->GetDissolve();
		return (dissolve < 1.0f);
	};

	// 透明度に関する設定を取得
	using DisSet = ShaderWrapper::DissolveSetting;
	DisSet setting = shader->GetDissolveSetting();

	// 「全てのポリゴンを不透明とみなす」場合、何も描画しない
	if (setting == DisSet::AllFullDissolve)
	{
		return;
	}

	// 全てのポリゴンを透明・半透明とみなすか
	bool isAllNotFullDissolve = (setting == DisSet::AllNotFullDissolve);

	// 状況ごとに条件を指定して描画
	if (!isAllNotFullDissolve)
	{
		DrawUnderCondition(shader, isNotFullDissolve);
	}
	else
	{
		// 元のディゾルブが0である場合を除き、全て描画する事を許可するラムダ式
		auto isNot0 = [](Mesh::ObjectData * obj, size_t polyGroupIndex)
		{
			const Mesh::MtlData * mtl = obj->GetPolyGroups()[polyGroupIndex]->mUsemtl;
			return (mtl->GetDissolve() > 0.0f);
		};

		DrawUnderCondition(shader, isNot0);
	}
}

void MeshComponent::DrawUnderCondition(ShaderWrapper * shader, std::function<bool(Mesh::ObjectData*obj, size_t polyGroupIndex)> condition) const
{
	// 頂点配列アクティブ化
	mMesh->mVertexArray->Activate();

	// シェーダアクティブ化
	shader->Activate();

	// ユニフォーム変数設定
	shader->InputUniforms();

	// ポリゴングループごとに描画処理
	for (auto objItr : mMesh->mObjects)
	{
		// 描画フラグが偽であればスキップ
		if (!objItr->GetDrawFlag())
		{
			continue;
		}

		// オブジェクトに所属するポリゴングループに対し、処理を行う
		for (size_t polygIndex = 0; polygIndex < objItr->GetPolyGroups().size(); ++polygIndex)
		{
			// 指定した条件が偽になるものはスキップ
			if (!condition(objItr, polygIndex))
			{
				continue;
			}

			// エイリアス取得
			const Mesh::ObjectData::PolyGroup * polygPtr = objItr->GetPolyGroups()[polygIndex];

			// ポリゴングループごとのユニフォーム設定
			shader->InputPolyUniforms(polygPtr);

			// 描画
			polygPtr->mElementBuffer->DrawElements();
		}
	}
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
