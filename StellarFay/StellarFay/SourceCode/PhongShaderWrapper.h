#pragma once
#include "ShaderWrapper.h"

// ユニフォーム変数"invertedModelMat"は登録されたモデル行列から自動的に作成するため、
// 登録をする必要はない
class PhongShaderWrapper : public ShaderWrapper
{
public:
	// nullptrを渡す場合は、通常のPhongシェーダをラップする
	PhongShaderWrapper(Shader * shader = nullptr);

	void SetUniforms() const override;

	void SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const override;

	// 面倒な行列系の設定をこの関数一つで行うことを理想とした関数
	// オーバーライド複数あり
	void UpdateMatricesReference(const Matrix4 * modelMatAddress, const class Renderer & renderer);
	void UpdateMatricesReference(const Matrix4 * modelMatAddress, const Matrix4 * viewMat, const Matrix4 * projMat);

	////////////////////////////////////
	// ユニフォーム変数設定関数群
	////////////////////////////////////

	// modelMat //

	// ユニフォーム変数"modelMat"を設定できる関数
	void UpdateModelMatUniform(const Matrix4 & mat) { mUniformList4m["modelMat"] = mat; }

	// ユニフォーム変数"modelMat"を設定できる関数（アドレスによる参照）
	void UpdateModelMatRefUniform(const Matrix4 * matAddress) { mUniformAddressList4m["modelMat"] = matAddress; }

	// viewMat //

	// ユニフォーム変数"viewMat"を設定できる関数
	void UpdateViewMatUniform(const Matrix4 & mat) { mUniformList4m["viewMat"] = mat; }

	// ユニフォーム変数"modelMat"を設定できる関数（アドレスによる参照）
	void UpdateViewMatRefUniform(const Matrix4 * matAddress) { mUniformAddressList4m["viewMat"] = matAddress; }

	// projectionMat //

	// ユニフォーム変数"projectionMat"を設定できる関数
	void UpdateProjectionMatUniform(const Matrix4 & mat) { mUniformList4m["projectionMat"] = mat; }

	// ユニフォーム変数"projectionMat"を設定できる関数（アドレスによる参照）
	void UpdateProjectionMatRefUniform(const Matrix4 * matAddress) { mUniformAddressList4m["projectionMat"] = matAddress; }

	// cameraPos //

	// ユニフォーム変数"cameraPos"を設定できる関数
	void UpdateCameraPosUniform(const Vector3D & pos) { mUniformList3f["cameraPos"] = pos; }
	
	// ユニフォーム変数"cameraPos"を設定できる関数（アドレスによる参照）
	void UpdateCameraPosRefUniform(const Vector3D * posAddress) { mUniformAddressList3f["cameraPos"] = posAddress; }

private:
	static Shader * mPhongShader;
};
