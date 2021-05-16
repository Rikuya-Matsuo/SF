#include <cstdio>
#include <algorithm>
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "MeshComponent.h"
#include "Actor.h"
#include "CommonMath.h"
#include "Camera.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	// シェーダ削除
	for (auto itr : mShaderMap)
	{
		delete itr.second;
	}
	mShaderMap.clear();

	// テクスチャ削除
	for (auto itr : mTextureMap)
	{
		delete itr.second;
	}
	mTextureMap.clear();

	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

bool Renderer::Init(Uint32 windowWidth, Uint32 windowHeight, bool fullScreen)
{
	// ウィンドウサイズを記録
	mWindowWidth = windowWidth;
	mWindowHeight = windowHeight;

	// OpenGL アトリビュートのセット
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// GL version 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// 8bit RGBAチャンネル
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// ダブルバッファリング
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// ハードウェアアクセラレーションを強制
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// ウィンドウ作成
	mWindow = SDL_CreateWindow("StellarFay", 0, 0,
		mWindowWidth, mWindowHeight, SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		printf("ウィンドウ作成失敗\n");
		return false;
	}

	// 引数に応じてフルスクリーン化
	if (fullScreen)
	{
		int failFullScreen = SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		if (failFullScreen)
		{
			printf("フルスクリーン化失敗\n");
		}
		else
		{
			glViewport(0, 0, mWindowWidth, mWindowHeight);
		}
	}

	// レンダラー作成
	mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mSDLRenderer)
	{
		printf("SDLRenderer作成失敗：\n%s\n", SDL_GetError());
		return false;
	}

	// コンテクスト作成
	mContext = SDL_GL_CreateContext(mWindow);

	// glad初期化
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("GL 拡張読み込み失敗\n");
	}

	// 透明度や深度バッファの設定
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);

	glCullFace(GL_FRONT);

	return true;
}

Shader * Renderer::GetShader(const std::string & vertFilePath, const std::string & fragFilePath)
{
	// 返却値
	Shader * ret = nullptr;

	// 入力された各ファイル名で検索
	StringPair key = std::make_pair(vertFilePath, fragFilePath);
	auto itr = mShaderMap.find(key);

	// 検索にヒットしなかった場合、新たに作成し、コンテナに追加する
	if (itr == mShaderMap.end())
	{
		// 作成
		Shader * shader = new Shader(vertFilePath, fragFilePath);

		// もし、シェーダの作成に失敗した場合、シェーダのインスタンスを削除し、nullを返す
		if (shader->Fail())
		{
			delete shader;
			return nullptr;
		}

		// コンテナ追加
		mShaderMap[key] = shader;

		// 返却値設定
		ret = shader;
	}

	// 見つかった場合
	else
	{
		// 返却値設定
		ret = itr->second;
	}

	return ret;
}

Texture * Renderer::GetTexture(const std::string & filePath)
{
	// 返却値
	Texture * ret = nullptr;

	// 同じファイル名のデータを探す
	auto itr = mTextureMap.find(filePath);

	// 見つからなかった場合、新たに作成し、コンテナに追加する
	if (itr == mTextureMap.end())
	{
		// 作成
		Texture * tex = new Texture(filePath);

		// 失敗時はインスタンスを削除し、nullを返す
		if (tex->Fail())
		{
			delete tex;
			return nullptr;
		}

		// コンテナに追加
		mTextureMap[filePath] = tex;

		// 返却値設定
		ret = tex;
	}

	// 見つかった場合
	else
	{
		// 返却値設定
		ret = itr->second;
	}

	return ret;
}

void Renderer::RegisterMeshComponent(MeshComponent * meshComp)
{
	//////////////////////////////
	// カメラ距離ソートコンテナ
	//////////////////////////////
	// ソートは描画直前に行うため、単純に最後尾に追加するだけでよい
	mMeshComponentsSortedInCameraDistance.emplace_back(meshComp);

	//////////////////////////////
	// 描画順ソートコンテナ
	//////////////////////////////
	// 描画順の数値取得
	const int drawPriority = meshComp->GetDrawPriority();

	// 描画順になるように挿入
	for (auto itr = mMeshComponentsSortedInDrawPriority.begin(); itr != mMeshComponentsSortedInDrawPriority.end(); ++itr)
	{
		if (drawPriority < (*itr)->GetDrawPriority())
		{
			mMeshComponentsSortedInDrawPriority.insert(itr, meshComp);

			return;
		}
	}

	// ここまでに挿入されなければ最後尾に追加する
	mMeshComponentsSortedInDrawPriority.emplace_back(meshComp);
}

void Renderer::DeregisterMeshComponent(MeshComponent * meshComp)
{
	// 指定されたコンポーネントを検索
	auto itr = std::find(mMeshComponentsSortedInDrawPriority.begin(), mMeshComponentsSortedInDrawPriority.end(), meshComp);

	// 検索にヒットすれば、リストから排除する
	if (itr != mMeshComponentsSortedInDrawPriority.end())
	{
		mMeshComponentsSortedInDrawPriority.erase(itr);
	}

	// カメラ順ソートのコンテナからも同様に削除する
	itr = std::find(mMeshComponentsSortedInCameraDistance.begin(), mMeshComponentsSortedInCameraDistance.end(), meshComp);

	if (itr != mMeshComponentsSortedInCameraDistance.end())
	{
		mMeshComponentsSortedInCameraDistance.erase(itr);
	}
}

void Renderer::Draw()
{
	// 背景色を反映
	glClearColor(mBGColor.x, mBGColor.y, mBGColor.z, 1.0f);

	// 画面クリア
	ClearWindow();

	// 不透明部分
	DrawFullDissolveObjects();

	// 半透明部分
	DrawNotFullDissolveObjects();

	// ウィンドウフリップ
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::DrawFullDissolveObjects()
{
	// 透明度無効化
	glDisable(GL_BLEND);

	// 深度テスト有効化
	glEnable(GL_DEPTH_TEST);

	// 描画順に従って描画
	for (auto itr : mMeshComponentsSortedInDrawPriority)
	{
		// ただし、コンポーネントとアクターの両方が描画を認めたものに限る
		bool actorAllow = itr->GetOwner()->GetDrawFlag();
		bool compAllow = itr->GetDrawFlag();
		if (actorAllow && compAllow)
		{
			itr->DrawFullDissolveObject();
		}
	}
}

void Renderer::DrawNotFullDissolveObjects()
{
	// カメラとコンポーネント所持者の距離が長い順にソート
	// ラムダ式
	// カメラ距離の2乗を計算
	auto calcCamDistSq = [this](const Actor * actor) -> float
	{
		return (actor->GetPosition() - mActiveCamera->GetPosition()).LengthSq();
	};
	// カメラ距離大きい順
	auto camDistLongOrder = [&calcCamDistSq](const MeshComponent * lhs, const MeshComponent * rhs)
	{
		return (calcCamDistSq(lhs->GetOwner()) > calcCamDistSq(rhs->GetOwner()));
	};

	// ソート
	mMeshComponentsSortedInCameraDistance.sort(camDistLongOrder);

	// 透明度有効化
	glEnable(GL_BLEND);

	// 描画
	for (auto itr : mMeshComponentsSortedInCameraDistance)
	{
		bool actorAllow = itr->GetOwner()->GetDrawFlag();
		bool compAllow = itr->GetDrawFlag();
		if (actorAllow && compAllow)
		{
			itr->DrawNotFullDissolveObject();
		}
	}
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
