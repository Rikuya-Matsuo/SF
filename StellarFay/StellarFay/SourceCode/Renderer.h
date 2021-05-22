#pragma once
#include <SDL/SDL.h>
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <list>
#include "Vector.h"
#include "Matrix.h"
#include "Hash.h"

struct LightInfo
{
	Vector3D mDirection;
	Vector3D mDiffuseColor;
	Vector3D mAmbientColor;
	Vector3D mSpecularColor;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	// 初期化
	bool Init(Uint32 windowWidth, Uint32 windowHeight, bool fullScreen);

	// メッシュコンポーネントの登録
	void RegisterMeshComponent(class MeshComponent * meshComp);

	// メッシュコンポーネント登録解除
	void DeregisterMeshComponent(class MeshComponent * meshComp);

	// 描画するUIの登録
	void RegisterUI(const class UILayerBase * ui);

	// 描画するUIの登録解除
	void DeregisterUI(const class UILayerBase * ui);

	// アクティブとするカメラのセット
	void SetActiveCamera(const class Camera * cam) { mActiveCamera = cam; }

	// 背景色設定
	void SetBGColor(const Vector3D & color) { mBGColor = color; }

	// 画面のクリア
	void ClearWindow() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	// 描画
	void Draw();

	// シェーダのロード。すでにロードされている組み合わせであればそのポインタを返す
	class Shader * GetShader(const std::string & vertFilePath, const std::string & fragFilePath);

	// メッシュのロード。すでにロードされている組み合わせであればそのポインタを返す
	class Mesh * GetMesh(const std::string & filePath);

	// テクスチャのロード。すでにロードされている場合はそのポインタを返す。
	class Texture * GetTexture(const std::string & filePath);

	// アクティブなカメラの取得
	const class Camera * GetActiveCamera() const { return mActiveCamera; }

	// ライト情報の参照取得
	LightInfo & GetLightInfo() { return mLightInfo; }

	// 描画域の大きさ取得
	Uint32 GetWidth() const { return mWindowWidth; }

	// 描画域の大きさ取得
	Uint32 GetHeight() const { return mWindowHeight; }

private:
	// ウィンドウサイズ
	Uint32 mWindowWidth;
	Uint32 mWindowHeight;

	// ウィンドウ
	SDL_Window * mWindow;

	// SDLレンダラー
	SDL_Renderer * mSDLRenderer;

	// コンテクスト
	SDL_GLContext mContext;

	// 頂点シェーダファイル名と、フラグメントシェーダファイル名の組み合わせで検索できるようにする
	typedef std::pair<std::string, std::string> StringPair;

	// string同士のpairのためのハッシュ用ファンクタ
	struct StringPairHash final
	{
		size_t operator()(const StringPair & pair) const
		{
			size_t hashes[] = { std::hash<std::string>()(pair.first), std::hash<std::string>()(pair.second) };
			return Hash::Fnv1(hashes, 2);
		}
	};

	// シェーダーコンテナ
	typedef std::unordered_map<StringPair, class Shader *, StringPairHash> ShaderMap;
	ShaderMap mShaderMap;

	// メッシュコンテナ
	typedef std::unordered_map<std::string, class Mesh *> MeshMap;
	MeshMap mMeshMap;

	// テクスチャコンテナ
	typedef std::unordered_map<std::string, class Texture *> TextureMap;
	TextureMap mTextureMap;

	// メッシュコンポーネントのコンテナ（描画順ソート）
	std::list<class MeshComponent *> mMeshComponentsSortedInDrawPriority;

	// メッシュコンポーネントのコンテナ（カメラ距離ソート）
	std::list<class MeshComponent *> mMeshComponentsSortedInCameraDistance;

	// 描画するUI
	std::list<const class UILayerBase *> mUIs;
	
	// カメラ
	const class Camera * mActiveCamera;

	// 背景色
	Vector3D mBGColor;

	// ライト情報
	LightInfo mLightInfo;

	// UI用頂点配列
	class VertexArray * mSpriteVert;

	// UI用インデックスバッファ
	class ElementBuffer * mSpriteElementBuffer;

	// 不透明オブジェクト描画部分
	void DrawFullDissolveObjects();

	// 半透明オブジェクト描画部分
	void DrawNotFullDissolveObjects();

	// UI用頂点座標作成
	void CreateSpriteVert();

	// UI用インデックスバッファ作成
	void CreateSpriteElementBuffer();
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
