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
	ShaderWrapper(Shader * shader = nullptr, Uint8 textureMass = 0);

	virtual ~ShaderWrapper();

	// リスト内のユニフォーム設定をシェーダに転送する
	virtual void InputUniforms() const;
	
	// ポリゴングループごとのユニフォーム設定
	// 挙動は継承先依存
	// テクスチャのアクティブ化もここに含まれる
	virtual void InputPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const;

	// シェーダのアクティブ化
	void Activate() const { mShader->Activate(); }

	// テクスチャの設定
	// 描画時は配列を頭から走査し、nullptrが検出された時点でテクスチャユニットへのセットを中止するので
	// unitIndexは0から順に連番でセットしなければならない
	void SetTexture(const class Texture * tex, Uint8 unitIndex);

	// ユニフォームの値を設定
	void SetUniformElement(const std::string & name, float value) { mUniformList1f[name] = value; }
	void SetUniformElement(const std::string & name, const Vector3D & value) { mUniformList3f[name] = value; }
	void SetUniformElement(const std::string & name, int value) { mUniformList1i[name] = value; }
	void SetUniformElement(const std::string & name, const Matrix4 & value) { mUniformList4m[name] = value; }

	// 数値でなく、メモリアドレスを登録することで、数値に変化があっても追従してユニフォーム転送を行える
	// ローカル変数を登録すると当然エラーとなるので注意
	void SetUniformAddress(const std::string & name, const float * address) { mUniformAddressList1f[name] = address; }
	void SetUniformAddress(const std::string & name, const Vector3D * address) { mUniformAddressList3f[name] = address; }
	void SetUniformAddress(const std::string & name, const int * address) { mUniformAddressList1i[name] = address; }
	void SetUniformAddress(const std::string & name, const Matrix4 * address) { mUniformAddressList4m[name] = address; }

	// 透明度に関する描画設定
	// Default				: ポリゴンごとに決められた透明度を用いる
	// AllNotFullDissolve	: 全て透明・半透明なポリゴンとみなす
	// AllFullDissolve		: 全て不透明なポリゴンとみなす
	enum DissolveSetting : Uint8
	{
		Default = 0,
		AllNotFullDissolve,
		AllFullDissolve,
		Invalid
	};

	// 透明度統一設定の変更
	void SetDissolveSetting(DissolveSetting setting) { mDissolveSetting = setting; }

	// 透明度統一設定の取得
	virtual DissolveSetting GetDissolveSetting() const;

protected:
	// 使用するシェーダ
	Shader * mShader;

	// テクスチャユニットにセットするテクスチャ
	const class Texture ** mTextures;

	// mTexturesの要素数
	const Uint8 mTextureMass;

	// 透明度に関する設定
	DissolveSetting mDissolveSetting;

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

	/////////////////////////////////////////////
	// 内部関数
	/////////////////////////////////////////////

	// テクスチャをテクスチャユニットにセットしていく関数
	void InputTextureUnits() const;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
