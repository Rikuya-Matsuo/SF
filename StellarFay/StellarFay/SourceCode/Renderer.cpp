#include <cstdio>
#include "Renderer.h"
#include "Shader.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
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

	return true;
}

Shader * Renderer::GetShader(const std::string & vertFilePath, const std::string & fragFilePath)
{
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
	}

	// コンテナのkeyの要素を返す
	return mShaderMap[key];
}
