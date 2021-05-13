#include "Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string & vertPath, const std::string & fragPath)
{
	// 頂点シェーダ読み込み
	GLuint vert;
	mSuccess = Load(vert, vertPath, GL_VERTEX_SHADER);
	if (!mSuccess)
	{
		return;
	}

	// フラグメントシェーダ読み込み
	GLuint frag;
	mSuccess = Load(frag, fragPath, GL_FRAGMENT_SHADER);
	if (!mSuccess)
	{
		return;
	}

	// 両シェーダーをリンク
	mID = glCreateProgram();
	glAttachShader(mID, vert);
	glAttachShader(mID, frag);
	glLinkProgram(mID);

	// リンクエラーをチェック
	int success;
	glGetProgramiv(mID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(mID, 512, nullptr, infoLog);

		std::cout << "SHADER::LINK::ERROR\n\t" << infoLog << std::endl;

		mSuccess = false;
		glDeleteShader(vert);
		glDeleteShader(frag);
		return;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

Shader::~Shader()
{
	if (mSuccess)
	{
		glDeleteProgram(mID);
	}
}

void Shader::SetUniform1f(const std::string & name, float value)
{
	int location = glGetUniformLocation(mID, name.c_str());

	glUniform1f(location, value);
}

void Shader::SetUniform2fv(const std::string & name, const float * values)
{
	int location = glGetUniformLocation(mID, name.c_str());

	glUniform2fv(location, 1, values);
}

void Shader::SetUniform2f(const std::string & name, float value0, float value1)
{
	float values[] = { value0, value1 };

	SetUniform2fv(name, values);
}

void Shader::SetUniform3fv(const std::string & name, const float * values)
{
	int location = glGetUniformLocation(mID, name.c_str());
	
	glUniform3fv(location, 1, values);
}

void Shader::SetUniform3f(const std::string & name, float value0, float value1, float value2)
{
	float values[] = { value0, value1, value2 };
	
	SetUniform3fv(name, values);
}

void Shader::SetUniform4fv(const std::string & name, const float * values)
{
	int location = glGetUniformLocation(mID, name.c_str());

	glUniform4fv(location, 1, values);
}

void Shader::SetUniform4f(const std::string & name, float value0, float value1, float value2, float value3)
{
	float values[] = { value0, value1, value2, value3 };

	SetUniform4fv(name, values);
}

void Shader::SetUniform1i(const std::string & name, int value)
{
	int location = glGetUniformLocation(mID, name.c_str());

	glUniform1i(location, value);
}

void Shader::SetUniform2iv(const std::string & name, const int * values)
{
	int location = glGetUniformLocation(mID, name.c_str());

	glUniform2iv(location, 1, values);
}

void Shader::SetUniform2i(const std::string & name, int value0, int value1)
{
	int values[] = { value0, value1 };

	SetUniform2iv(name, values);
}

void Shader::SetUniform3iv(const std::string & name, const int * values)
{
	int location = glGetUniformLocation(mID, name.c_str());

	glUniform3iv(location, 1, values);
}

void Shader::SetUniform3i(const std::string & name, int value0, int value1, int value2)
{
	int values[] = { value0, value1, value2 };

	SetUniform3iv(name, values);
}

void Shader::SetUniform4iv(const std::string & name, const int * values)
{
	int location = glGetUniformLocation(mID, name.c_str());

	glUniform4iv(location, 1, values);
}

void Shader::SetUniform4i(const std::string & name, int value0, int value1, int value2, int value3)
{
	int values[] = { value0, value1, value2, value3 };

	SetUniform4iv(name, values);
}

void Shader::SetUniform3m(const std::string & name, const float * values)
{
	int location = glGetUniformLocation(mID, name.c_str());

	glUniformMatrix3fv(location, 1, GL_FALSE, values);
}

void Shader::SetUniform4m(const std::string & name, const float * values)
{
	int location = glGetUniformLocation(mID, name.c_str());

	glUniformMatrix4fv(location, 1, GL_FALSE, values);
}

bool Shader::Load(GLuint & target, const std::string & path, GLenum shaderType)
{
	// シェーダ作成
	target = glCreateShader(shaderType);

	// ファイルオープン
	std::ifstream file(path);
	if (file.fail())
	{
		std::cout << "ファイル読み込み失敗：" << path << std::endl;

		glDeleteShader(target);
		return false;
	}

	// ファイル内容を文字列ストリームに流し込む
	std::stringstream content;
	content << file.rdbuf();

	// ファイルクローズ
	file.close();

	// シェーダをコンパイル
	std::string contentStr = content.str();			// 一度string型にしてあげないとうまく動かないらしい
	const char * contentPtr = contentStr.c_str();
	glShaderSource(target, 1, &contentPtr, nullptr);
	glCompileShader(target);

	// コンパイルエラーをチェック
	int success;
	glGetShaderiv(target, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(target, 512, nullptr, infoLog);

		std::cout << "SHADER::" <<
			(shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") <<
			"::COMPILE_ERROR\n"
			<< infoLog << std::endl;

		glDeleteShader(target);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
