#pragma once
#include "Shader.h"
#include "Vector.h"
#include "Mesh.h"
#include <unordered_map>
#include <string>

// 描画直前でなくても、ユニフォーム変数に転送する値を予め設定できるようにする意図で作成
class ShaderWrapper
{
protected:
	// 型定義だけ先に済ませる
	typedef std::unordered_map<std::string, float> UnifList1f;
	typedef std::unordered_map<std::string, Vector3D> UnifList3f;
	typedef std::unordered_map<std::string, int> UnifList1i;

public:
	ShaderWrapper(Shader * shader = nullptr);

	virtual ~ShaderWrapper();

	virtual void SetUniforms() const;
	
	// ポリゴングループごとのユニフォーム設定
	// 挙動は継承先依存
	virtual void SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const;

protected:
	Shader * mShader;

	/////////////////////////////////////////////
	// ユニフォーム変数に転送する値のリスト
	/////////////////////////////////////////////

	// オブジェクト単位で転送
	UnifList1f mUniformList1f;

	// オブジェクト単位で転送
	UnifList3f mUniformList3f;

	// オブジェクト単位で転送
	UnifList1i mUniformList1i;
};
