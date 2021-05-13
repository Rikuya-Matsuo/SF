#include <SDL/SDL_image.h>
#include "Texture.h"

Texture::Texture(const std::string & path)
{
	mSuccess = Load(path);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mID);
}

bool Texture::Load(const std::string & path)
{
	// テクスチャ読み込み
	// 画像をSDLサーフェスに読み込む
	SDL_Surface * surface = IMG_Load(path.c_str());
	if (!surface)
	{
		printf("画像ファイル読み込み失敗\n");
		return false;
	}

	// 画像の幅、高さを取得
	int width = surface->w;
	int height = surface->h;
	int channels = surface->format->BytesPerPixel;

	// 画像フォーマットの判別
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// テクスチャ生成＆バインド
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);

	// テクスチャラッピング＆フィルタリング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// SDLデータをGLのテクスチャデータとしてコピーする
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	// SDLサーフェス解放
	SDL_FreeSurface(surface);

	return true;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
