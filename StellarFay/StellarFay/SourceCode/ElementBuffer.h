#pragma once
#include <glad/glad.h>
#include <vector>

class ElementBuffer
{
public:
	// indices			: インデックス属性データ
	// indicesMass		: indicesの要素数
	// indexDataSize	: indicesの要素１つ当たりのデータの大きさ(byte)
	// type				: indicesの要素の型（デフォルトはGL_UNSIGNED_INT）
	ElementBuffer(const void * indices, size_t indicesMass, size_t indexDataSize, GLenum type = GL_UNSIGNED_INT);

	~ElementBuffer()
	{
		glDeleteBuffers(1, &mEBO);
	}

	// アクティブ化
	void Activate() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO); }

	// 描画。アクティブ化も込み。
	// 引数のデフォルトはGL_TRIANGLES
	void DrawElements(GLenum mode = GL_TRIANGLES) const;

private:
	GLuint mEBO;

	// 要素数
	size_t mIndicesMass;

	// バッファ内のデータ型
	GLenum mType;
};
