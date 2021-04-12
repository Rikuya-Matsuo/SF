#pragma once
#include <glad/glad.h>
#include <SDL/SDL.h>
#include <string>

class Texture
{
public:
	Texture(const std::string & path);
	~Texture();

	static void ActivateTextureUnit(Uint8 textureUnitIndex) { glActiveTexture(GL_TEXTURE0 + textureUnitIndex); }

	bool Fail() const { return !mSuccess; }

	void Activate() const { glBindTexture(GL_TEXTURE_2D, mID); }

protected:
	GLuint mID;

	bool mSuccess;

	bool Load(const std::string & path);
};
