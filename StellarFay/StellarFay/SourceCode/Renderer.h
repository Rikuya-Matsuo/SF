#pragma once
#include <SDL/SDL.h>
#include <string>
#include <unordered_map>
#include "Hash.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Init(Uint32 windowWidth, Uint32 windowHeight, bool fullScreen);

	class Shader * GetShader(const std::string & vertFilePath, const std::string & fragFilePath);

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

	// シェーダーコンテナ
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
	typedef std::unordered_map<StringPair, class Shader *, StringPairHash> ShaderMap;
	ShaderMap mShaderMap;
};
