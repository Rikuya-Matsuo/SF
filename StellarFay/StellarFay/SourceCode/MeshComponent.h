#pragma once
#include "ComponentBase.h"
#include "Mesh.h"
#include "BitFlagFunc.h"
#include <string>
#include <functional>

namespace MeshComponentFlagMask
{
	// 型
	typedef Uint8 Type;

	// マスク
	static const Type mDrawFlagMask			= BIT_SHIFT(0);

	// 初期化値
	static const Type mInitialFlag = mDrawFlagMask;
}

class MeshComponent : public ComponentBase
{
public:
	MeshComponent(class Actor * owner, Mesh * msh, int drawPriority);
	~MeshComponent();

	static void SetInitialShader(const std::string & vertFilePath, const std::string & fragFilePath);

	static void SetInitialShader(class Shader * shader) { mInitialShader = shader; }

	////////////////////////
	// セッター
	////////////////////////
	void SetShader(class Shader * shader) { mShader = shader; }

	void SetDrawFlag(bool value) { BitFlagFunc::Set(mMeshCompFlags, MeshComponentFlagMask::mDrawFlagMask, value); }

	////////////////////////
	// ゲッター
	////////////////////////
	class Shader * GetShader() const { return mShader; }

	bool GetDrawFlag() const { return BitFlagFunc::GetOr(mMeshCompFlags, MeshComponentFlagMask::mDrawFlagMask); }

	int GetDrawPriority() const { return mDrawPriority; }

	void DrawFullDissolveObject(class Shader * shader) const;

	void DrawFullDissolveObject() const { DrawFullDissolveObject(mShader); }

private:
	// ビットフラグ
	MeshComponentFlagMask::Type mMeshCompFlags;

	// メッシュ
	Mesh * mMesh;

	// 描画順
	// 半透明な部分に関してはこの値に関係なく、カメラからの距離で描画順を決定する予定
	int mDrawPriority;

	// 描画に原則使用するシェーダ
	class Shader * mShader;

	// シェーダの初期化値
	static class Shader * mInitialShader;

	void DrawUnderCondition(class Shader * shader, std::function<bool(Mesh::ObjectData * obj, size_t polyGroupIndex)> condition) const;
};
