#include "MeshComponent.h"
#include "Actor.h"
#include "Mesh.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "Shader.h"

// 名前空間の省略
namespace mcfm = MeshComponentFlagMask;

Shader * MeshComponent::mInitialShader = nullptr;

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
	Shader * shd = RENDERER_INSTANCE.GetShader(vertFilePath, fragFilePath);
	SetInitialShader(shd);
}

void MeshComponent::DrawFullDissolveObject(Shader * shader) const
{
	
}

void MeshComponent::DrawNotFullDissolveObject(Shader * shader) const
{

}

void MeshComponent::DrawUnderCondition(Shader * shader, std::function<bool(Mesh::ObjectData*obj, size_t polyGroupIndex)> condition) const
{
	// 頂点配列アクティブ化
	mMesh->mVertexArray->Activate();

	// シェーダアクティブ化
	shader->Activate();

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

			const Mesh::MtlData * mtl = polygPtr->mUsemtl;
			const Texture * tex = mtl->GetTexture();

			if (tex)
			{
				tex->Activate();
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygPtr->mEBO);
			glDrawElements(GL_TRIANGLES, polygPtr->mIndices.size(), GL_UNSIGNED_INT, 0);
		}
	}
}
