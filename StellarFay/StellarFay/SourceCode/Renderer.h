#pragma once
#include <SDL/SDL.h>
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <list>
#include "Vector.h"
#include "Hash.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	// 初期化
	bool Init(Uint32 windowWidth, Uint32 windowHeight, bool fullScreen);

	// シェーダのロード。すでにロードされている組み合わせであればそのポインタを返す
	class Shader * GetShader(const std::string & vertFilePath, const std::string & fragFilePath);

	// メッシュコンポーネントの登録
	void RegisterMeshComponent(class MeshComponent * meshComp);

	// メッシュコンポーネント登録解除
	void DeregisterMeshComponent(class MeshComponent * meshComp);

	// カメラ位置のセット
	void SetCameraPos(const Vector3D & pos) { mCameraPosition = pos; }

	// 背景色設定
	void SetBGColor(const Vector3D & color) { mBGColor = color; }

	// 画面のクリア
	void ClearWindow() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	// 描画
	void Draw();

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

	// メッシュコンポーネントのコンテナ（描画順ソート）
	std::list<class MeshComponent *> mMeshComponentsSortedInDrawPriority;

	// メッシュコンポーネントのコンテナ（カメラ距離ソート）
	std::list<class MeshComponent *> mMeshComponentsSortedInCameraDistance;

	// カメラ位置
	Vector3D mCameraPosition;

	// 背景色
	Vector3D mBGColor;

	// 不透明オブジェクト描画部分
	void DrawFullDissolveObjects();

	// 半透明オブジェクト描画部分
	void DrawNotFullDissolveObjects();
};
