#pragma once
#include "Shader.h"
#include "Vector.h"
#include "Matrix.h"
#include "Mesh.h"
#include <unordered_map>
#include <string>

// 描画直前でなくても、ユニフォーム変数に転送する値を予め設定できるようにする意図で作成
class ShaderWrapper
{
protected:
	// 型定義だけ先に済ませる
	typedef std::unordered_map<std::string, float> UnifList1f;
	typedef std::unordered_map<std::string, const float *> UnifAddressList1f;
	typedef std::unordered_map<std::string, Vector3D> UnifList3f;
	typedef std::unordered_map<std::string, const Vector3D *> UnifAddressList3f;
	typedef std::unordered_map<std::string, int> UnifList1i;
	typedef std::unordered_map<std::string, const int *> UnifAddressList1i;
	typedef std::unordered_map<std::string, Matrix4> UnifList4m;
	typedef std::unordered_map<std::string, const Matrix4 *> UnifAddressList4m;

public:
	ShaderWrapper(Shader * shader = nullptr);

	virtual ~ShaderWrapper();

	virtual void SetUniforms() const;
	
	// ポリゴングループごとのユニフォーム設定
	// 挙動は継承先依存
	// テクスチャのアクティブ化もここに含まれる
	virtual void SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const;

	void Activate() const { mShader->Activate(); }

	void UpdateUniformElement(const std::string & name, float value) { mUniformList1f[name] = value; }
	void UpdateUniformElement(const std::string & name, const Vector3D & value) { mUniformList3f[name] = value; }
	void UpdateUniformElement(const std::string & name, int value) { mUniformList1i[name] = value; }
	void UpdateUniformElement(const std::string & name, const Matrix4 & value) { mUniformList4m[name] = value; }

	// 数値でなく、メモリアドレスを登録することで、数値に変化があっても追従してユニフォーム転送を行える
	// ローカル変数を登録すると当然エラーとなるので注意
	void UpdateUniformAddress(const std::string & name, const float * address) { mUniformAddressList1f[name] = address; }
	void UpdateUniformAddress(const std::string & name, const Vector3D * address) { mUniformAddressList3f[name] = address; }
	void UpdateUniformAddress(const std::string & name, const int * address) { mUniformAddressList1i[name] = address; }
	void UpdateUniformAddress(const std::string & name, const Matrix4 * address) { mUniformAddressList4m[name] = address; }

protected:
	Shader * mShader;

	/////////////////////////////////////////////
	// ユニフォーム変数に転送する値のリスト
	/////////////////////////////////////////////

	// float型
	UnifList1f mUniformList1f;

	// float型参照
	UnifAddressList1f mUniformAddressList1f;

	// float型 3つ
	UnifList3f mUniformList3f;

	// float型 3つの参照
	UnifAddressList3f mUniformAddressList3f;

	// int型
	UnifList1i mUniformList1i;

	// int型の参照
	UnifAddressList1i mUniformAddressList1i;

	// 行列型
	UnifList4m mUniformList4m;

	// 行列の参照
	UnifAddressList4m mUniformAddressList4m;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
