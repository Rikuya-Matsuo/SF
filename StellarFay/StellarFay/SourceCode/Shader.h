#pragma once
#include <glad/glad.h>
#include <string>

class Shader
{
public:
	Shader(const std::string & vertPath, const std::string & fragPath);
	~Shader();

	bool Fail() const { return !mSuccess; }

	void Activate() const { glUseProgram(mID); }

	void SetUniform1f(const std::string & name, float value);

	void SetUniform2fv(const std::string & name, const float * values);
	void SetUniform2f(const std::string & name, float value0, float value1);

	void SetUniform3fv(const std::string & name, const float * values);
	void SetUniform3f(const std::string & name, float value0, float value1, float value2);

	void SetUniform4fv(const std::string & name, const float * values);
	void SetUniform4f(const std::string & name, float value0, float value1, float value2, float value3);

	void SetUniformb(const std::string & name, bool value) { SetUniform1i(name, static_cast<int>(value)); }

	void SetUniform1i(const std::string & name, int value);

	void SetUniform2iv(const std::string & name, const int * values);
	void SetUniform2i(const std::string & name, int value0, int value1);

	void SetUniform3iv(const std::string & name, const int * values);
	void SetUniform3i(const std::string & name, int value0, int value1, int value2);

	void SetUniform4iv(const std::string & name, const int * values);
	void SetUniform4i(const std::string & name, int value0, int value1, int value2, int value3);

	void SetUniform3m(const std::string & name, const float * values);

	void SetUniform4m(const std::string & name, const float * values);
	
private:
	GLuint mID;

	bool mSuccess;

	bool Load(GLuint & target, const std::string & path, GLenum shaderType);
};
