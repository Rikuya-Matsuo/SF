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
	VertexArray(const void * verts, size_t vertsMass, size_t * attributeSizes, size_t attributeSizeElemMass, GLenum type, GLboolean normalize, GLenum usage = GL_STATIC_DRAW);

private:
	GLuint mVAO;
	GLuint mVBO;

	static std::vector<size_t> Attribute;
};
