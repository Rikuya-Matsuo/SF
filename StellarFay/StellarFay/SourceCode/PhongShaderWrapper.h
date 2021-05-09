#pragma once
#include "ShaderWrapper.h"

class PhongShaderWrapper : public ShaderWrapper
{
public:
	// nullptrを渡す場合は、通常のPhongシェーダをラップする
	PhongShaderWrapper(Shader * shader = nullptr);

	void SetUniforms() const override;

	void SetPolyUniforms(const Mesh::ObjectData::PolyGroup * polyGroup) const override;

	// ユニフォーム変数"cameraPos"を設定できる関数
	void UpdateCameraPosUniform(const Vector3D & pos) { mUniformList3f["cameraPos"] = pos; }
	
	// ユニフォーム変数"cameraPos"を設定できる関数（アドレスによる参照）
	void UpdateCameraPosRefUniform(const Vector3D * pos) { mUniformAddressList3f["cameraPos"] = pos; }

private:
	static Shader * mPhongShader;
};
