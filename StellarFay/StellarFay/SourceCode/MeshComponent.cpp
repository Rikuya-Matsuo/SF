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

	// 条件を指定して描画
	DrawUnderCondition(shader, isFullDissolve);
}

void MeshComponent::DrawNotFullDissolveObject(ShaderWrapper * shader) const
{
	// ディゾルブが1未満であるかを検証するラムダ式
	auto isNotFullDissolve = [](Mesh::ObjectData * obj, size_t polyGroupIndex)
	{
		const Mesh::MtlData * mtl = obj->GetPolyGroups()[polyGroupIndex]->mUsemtl;
		return (mtl->GetDissolve() < 1.0f);
	};

	// 条件を指定して描画
	DrawUnderCondition(shader, isNotFullDissolve);
}

void MeshComponent::DrawUnderCondition(ShaderWrapper * shader, std::function<bool(Mesh::ObjectData*obj, size_t polyGroupIndex)> condition) const
{
	// 頂点配列アクティブ化
	mMesh->mVertexArray->Activate();

	// シェーダアクティブ化
	shader->Activate();

	// ユニフォーム変数設定
	shader->SetUniforms();

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
			shader->SetPolyUniforms(polygPtr);

			const Mesh::MtlData * mtl = polygPtr->mUsemtl;
			const Texture * tex = mtl->GetTexture();

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygPtr->mEBO);
			glDrawElements(GL_TRIANGLES, polygPtr->mIndices.size(), GL_UNSIGNED_INT, 0);
		}
	}
}
