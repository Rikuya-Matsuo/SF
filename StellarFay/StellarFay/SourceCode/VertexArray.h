#pragma once
#include <glad/glad.h>
#include <vector>

class VertexArray
{
public:
	// verts					: 頂点データ
	// vertsMass				: 頂点データの要素数
	// attributeSize			: 頂点データ解釈方法（glVertexAttribPointerの第二引数を順番に、配列の形で入力すべし）
	// attributeSizeElemMass	: attributeSizeの要素数
	VertexArray(const void * verts, size_t vertsMass, const size_t * attributeSizes, size_t attributeSizeElemMass, GLenum type, GLboolean normalize, GLenum usage = GL_STATIC_DRAW);

	// コンストラクタの引数を予め用意し、使いまわすための構造体
	struct ArgumentPreset
	{
		std::vector<size_t> mAttributeSizes;
		GLenum mType;
		GLboolean mNormalize;
		GLenum mUsage = GL_STATIC_DRAW;

		// mAttributeSizesを設定する関数
		// argMassには設定したい要素数を入力し、その後ろに値を書き連ねる
		void SetAttributeSizes(size_t argMass, ...);
	};

	VertexArray(const void * verts, size_t vertsMass, const ArgumentPreset & arg) :
		VertexArray(verts, vertsMass, arg.mAttributeSizes.data(), arg.mAttributeSizes.size(), arg.mType, arg.mNormalize, arg.mUsage)
	{}

	~VertexArray();

	void Activate() const
	{
		glBindVertexArray(mVAO);
	}

private:
	GLuint mVAO;
	GLuint mVBO;
};

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
