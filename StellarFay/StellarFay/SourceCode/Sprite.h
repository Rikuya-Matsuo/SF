#pragma once
#include <SDL/SDL.h>
#include "Vector.h"
#include "Matrix.h"

// UILayerBase内で表示する画像のデータ
class Sprite
{
public:
	// 引数shaderWrapperには使用するシェーダラッパーへのポインタを入力するが、
	// デフォルト、nullptrが与えられた場合はSpriteShaderWrapperを生成する。
	// この際、コンストラクタに与える引数はデフォルトである。
	Sprite(class UILayerBase * uiLayer, class ShaderWrapper * shaderWrapper = nullptr);
	~Sprite();

	// シェーダラッパーの取得
	class ShaderWrapper * GetShaderWrapper() { return mShaderWrapper; }

	// 位置の設定
	void SetPosition(const Vector2D & pos) { mPosition = pos; }

	// 大きさの設定
	void SetScale(const Vector2D & scale) { mScale = scale; }

	// テクスチャの設定
	void SetTexture(const class Texture * tex, Uint8 texUnitIndex = 0);

	// 透明度の設定
	void SetDissolve(float value);

	// 行列の更新
	void UpdateMatrix();

	// 描画
	virtual void Draw(const class ElementBuffer * elemBuf) const;

protected:
	// 描画位置
	Vector2D mPosition;

	// 大きさ
	Vector2D mScale;

	// 行列
	Matrix4 mModelMat;

	// シェーダのラッパー
	class ShaderWrapper * mShaderWrapper;
};
