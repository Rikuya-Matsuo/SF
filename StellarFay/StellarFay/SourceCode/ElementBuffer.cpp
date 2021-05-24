#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const void * indices, size_t indicesMass, size_t indexDataSize, GLenum type) :
	mType(type),
	mIndicesMass(indicesMass)
{
	// バッファ生成
	glGenBuffers(1, &mEBO);

	// 設定
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize * mIndicesMass, indices, GL_STATIC_DRAW);
}
