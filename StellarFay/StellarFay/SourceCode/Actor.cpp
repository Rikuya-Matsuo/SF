#include "Actor.h"
#include "ComponentBase.h"
#include "SceneBase.h"
#include "Shader.h"
#include "GameSystem.h"
#include "Renderer.h"
#include <algorithm>

Shader * Actor::mPhongShader = nullptr;

// 本cppファイル内でのみ、省略した名前を使う
namespace afm = ActorFlagMask;

Actor::Actor(int priority) :
	mPriority(priority),
	mActorFlags(afm::mInitialFlag)
{
	// 静的変数の設定
	if (!mPhongShader)
	{
		mPhongShader = RENDERER_INSTANCE.GetShader("Shaders/Phong.vert", "Shaders/Phong.frag");
	}

	// 所属シーン取得及び、シーンへのアクター登録
	SceneBase::GetLatestScene(this);
	mBelongScene->RegisterActor(this);
}

Actor::~Actor()
{
	// コンポーネントのメモリ解放
	for (auto itr = mComponents.begin(); itr != mComponents.end(); ++itr)
	{
		delete *itr;
		*itr = nullptr;
	}
	mComponents.clear();
	std::list<ComponentBase *>().swap(mComponents);
}

void Actor::Update()
{
	// コンポーネントのソート要請を受けた場合、実行
	if (BitFlagFunc::GetOr(mActorFlags, afm::mSortComponentsFlagMask))
	{
		// ソート順を示すラムダ式
		auto orderLambda = [](const ComponentBase * c1, const ComponentBase * c2)
		{
			return (c1->GetPriority() < c2->GetPriority());
		};

		// 各コンポーネントのmPriorityの値が小さい順にソート
		mComponents.sort(orderLambda);
	}

	// アクター更新前に更新するコンポーネントを更新
	auto itr = mComponents.begin();
	for (; itr != mComponents.end(); ++itr)
	{
		// アクター更新前に更新するものでなくなった場合、ループから抜ける
		if (!(*itr)->GetBeforeUpdateActorFlag())
		{
			break;
		}

		// 更新
		(*itr)->Update();
	}

	// 継承先依存の更新処理
	UpdateActor();

	// アクター更新後に更新するコンポーネントを更新
	for (; itr != mComponents.end(); ++itr)
	{
		(*itr)->Update();
	}

	// 継承先依存の最終更新
	UpdateActorLast();

	// 行列の更新
	UpdateModelMat();
}

void Actor::SetPriority(int priority)
{
	// 更新順変更
	mPriority = priority;
	
	// シーンに対しアクターのソートを要請
	mBelongScene->RequestSortActor();
}

void Actor::RegisterComponent(ComponentBase * cmp)
{
	// mPriorityの値が小さい順になるよう挿入する
	const int priority = cmp->GetPriority();

	for (auto itr = mComponents.begin(); itr != mComponents.end(); ++itr)
	{
		const int itrPriority = (*itr)->GetPriority();

		if (priority < itrPriority)
		{
			mComponents.insert(itr, cmp);
			return;
		}
	}

	// ここまでで挿入できなかった場合、最後尾に追加する
	mComponents.emplace_back(cmp);
}

void Actor::SetUniforms(Shader * shader)
{
	// デフォルトでPhongシェーダの設定を行う
	SetPhongUniforms(shader);
}

void Actor::UpdateActor()
{
	// 内容は継承先に依存する
}

void Actor::UpdateActorLast()
{
	// 内容は継承先に依存する
}

void Actor::SetPhongUniforms(Shader * shader) const
{
	shader->SetUniform4m("modelMat", mModelMat.GetAsFloatPtr());

	shader->SetUniform4m("projectionMat", RENDERER_INSTANCE.GetProjectionMat().GetAsFloatPtr());

	Matrix4 invModel = mModelMat;
	invModel.Invert();
	shader->SetUniform4m("invertedModelMat", invModel.GetAsFloatPtr());
}

void Actor::UpdateModelMat()
{
	mModelMat = Matrix4::CreateScale(mScales);
	mModelMat *= Matrix4::CreateFromQuaternion(mRotation);
	mModelMat *= Matrix4::CreateTranslation(mPosition);
}
