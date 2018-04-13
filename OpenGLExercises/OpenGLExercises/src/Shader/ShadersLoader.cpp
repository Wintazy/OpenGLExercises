#include "ShadersLoader.h"

ShadersLoader::ShadersLoader()
{
	m_shaderProgramId = glCreateProgram();
}

int ShadersLoader::CompileShader(const char* shaderSource, GLuint type)
{
	int shaderIdResult = glCreateShader(type);
	glShaderSource(shaderIdResult, 1, &shaderSource, NULL);
	glCompileShader(shaderIdResult);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderIdResult, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderIdResult, 512, NULL, infoLog);
		std::cout << "Failed to compile shader!\n" << infoLog << std::endl;
		std::cout << shaderSource << std::endl;
		return -1;
	}

	return shaderIdResult;
}

void ShadersLoader::EnableShaderProgram()
{
	glUseProgram(m_shaderProgramId);
}

void ShadersLoader::SetBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_shaderProgramId, name.c_str()), (int)value);
}

void ShadersLoader::SetInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_shaderProgramId, name.c_str()), value);
}

void ShadersLoader::SetFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_shaderProgramId, name.c_str()), value);
}

void ShadersLoader::SetMat4f(const std::string &name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramId, name.c_str()), 1, GL_FALSE, value);
}

void ShadersLoader::SetVec3f(const std::string &name, const float* value) const
{
	glUniform3fv(glGetUniformLocation(m_shaderProgramId, name.c_str()), 1, value);
}

void ShadersLoader::SetVec3f(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_shaderProgramId, name.c_str()), x, y, z);
}

void ShadersLoader::SetVec3f(const std::string &name, glm::vec3 i_vec3f) const
{
	glUniform3f(glGetUniformLocation(m_shaderProgramId, name.c_str()), i_vec3f.x, i_vec3f.y, i_vec3f.z);
}

void ShadersLoader::LoadShaders(const GLchar* shaderPath, GLuint type)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(shaderPath);
		std::stringstream shaderStream, fragmentShaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderCode = shaderStream.str();
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to read shader files " << shaderPath << std::endl;
	}

	int shaderId = CompileShader(shaderCode.c_str(), type);
	LinkShaders(shaderId);
	glDeleteShader(shaderId);
}

void ShadersLoader::LinkShaders(int shaderId)
{
	EnableShaderProgram();
	glAttachShader(m_shaderProgramId, shaderId);
	glLinkProgram(m_shaderProgramId);

	int success;
	char infoLog[512];
	glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgramId, 512, NULL, infoLog);
		std::cout << "Failed to link shader program!\n" << infoLog << std::endl;
	}
}