#pragma once
#include "..\Define.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShadersLoader
{
public:
	unsigned int m_shaderProgramId;
	ShadersLoader();
	void LoadShaders(const GLchar* vertexShaderPath, const GLchar* fragmentShaderpath);

	void EnableShaderProgram();

	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetMat4f(const std::string &name, const float* value) const;
	void SetVec3f(const std::string &name, const float* value) const;
	void SetVec3f(const std::string &name, float x, float y, float z) const;
	void SetVec3f(const std::string &name, glm::vec3 i_vec3f) const;
private:
	int CompileShader(const char* shaderSource, GLuint type);
	int LinkShaders(int vertexShaderId, int fragmentShaderId);
};