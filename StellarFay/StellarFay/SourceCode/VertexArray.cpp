#include "VertexArray.h"

#

VertexArray::VertexArray(const void * verts, size_t vertsMass, size_t * attributeSizes, size_t attributeSizeElemMass, GLenum type, GLboolean normalize, GLenum usage)
{
	// 頂点配列、頂点バッファ生成
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	// 頂点配列バインド
	glBindVertexArray(mVAO);

	// 頂点の配列の型のサイズを計算
	// unsignedがついてもサイズは変わらないため、処理を同じにしている
	size_t vertTypeSize;
	switch (type)
	{
	case GL_UNSIGNED_INT:
	case GL_INT:
		vertTypeSize = sizeof(int);
		break;

	case GL_UNSIGNED_SHORT:
	case GL_SHORT:
		vertTypeSize = sizeof(short);
		break;

	case GL_UNSIGNED_BYTE:
	case GL_BYTE:
		vertTypeSize = sizeof(char);
		break;

	case GL_FLOAT:
		vertTypeSize = sizeof(float);
		break;

	default:
		break;
	}

	// 頂点バッファに頂点を設定
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vertTypeSize * vertsMass, verts, usage);

	// 頂点データのストライド設定のため、attributeSizesの合計値を計算
	size_t attSizesSum = 0;
	for (size_t i = 0; i < attributeSizeElemMass; ++i)
	{
		attSizesSum += attributeSizes[i];
	}

	// 頂点データの解釈方法設定
	const size_t stride = vertTypeSize * attSizesSum;
	size_t attSizesSumBuf = 0;
	for (size_t i = 0; i < attributeSizeElemMass; ++i)
	{
		glVertexAttribPointer(i, attributeSizes[i], type, normalize, stride, (void*)(vertTypeSize * attSizesSumBuf));
		glEnableVertexAttribArray(i);
		attSizesSumBuf += attributeSizes[i];
	}
}
