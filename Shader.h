#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

class Shader
{
	enum ShaderOperations
	{
		COMPILE_SHADER,
		LINK_SHADER_PROGRAM
	};

public:

	unsigned int ID;
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
	{
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;
		std::stringstream vertexShaderString, fragmentShaderString;

		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vertexShaderFile.open(vertexShaderFilePath);
			fragmentShaderFile.open(fragmentShaderFilePath);

			vertexShaderString << vertexShaderFile.rdbuf();
			fragmentShaderString << fragmentShaderFile.rdbuf();


			vertexShaderCode = vertexShaderString.str();
			fragmentShaderCode = fragmentShaderString.str();

		}
		catch (std::ifstream::failure e)
		{
			std::cout << "FILE READ ERROR" << std::endl;
		}

		const char* vShaderCode = vertexShaderCode.c_str();
		const char* fShaderCode = fragmentShaderCode.c_str();

		unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexShaderID, 1, &vShaderCode, NULL);
		glShaderSource(fragmentShaderID, 1, &fShaderCode, NULL);

		if (!TryShaderOperation(vertexShaderID, ShaderOperations::COMPILE_SHADER))
			std::cout << "SHADER COMPILATION ERROR - VERTEX SHADER\n";

		if (!TryShaderOperation(fragmentShaderID, ShaderOperations::COMPILE_SHADER))
			std::cout << "SHADER COMPILATION ERROR - FRAGMENT SHADER\n";

		ID = glCreateProgram();

		glAttachShader(ID, vertexShaderID);
		glAttachShader(ID, fragmentShaderID);

		if (!TryShaderOperation(ID, ShaderOperations::LINK_SHADER_PROGRAM))
			std::cout << "SHADER PROGRAM LINKING ERROR\n";

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	void Use()
	{
		glUseProgram(ID);
	}

	void SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	bool TryShaderOperation(const int shaderID, const ShaderOperations shaderOp)
	{
		int success;

		switch (shaderOp)
		{
			case COMPILE_SHADER:
				glCompileShader(shaderID);
				glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
				break;

			case LINK_SHADER_PROGRAM:
				glLinkProgram(shaderID);
				glGetShaderiv(shaderID, GL_LINK_STATUS, &success);
				break;

			default:
				break;
		}

		if (!success)
			return false;
		return true;
	}
};
#endif