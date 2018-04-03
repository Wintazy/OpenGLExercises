#include "ShadersLoader.h"

ShadersLoader::ShadersLoader()
{

}

void ShadersLoader::LoadShaders(const char* vetexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;
	std::ifstream vetexShaderFile;
	std::ifstream fragmentShaderFile;

	vetexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vetexShaderFile.open(vetexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);
		std::stringstream vetexShaderStream, fragmentShaderStream;

		vetexShaderStream << vetexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vetexShaderFile.close();
		fragmentShaderFile.close();

		vertexShaderCode = vetexShaderStream.str();
		fragmentShaderCode = fragmentShaderStream.str();
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to read shader files " << vetexShaderPath << " " << fragmentShaderPath << std::endl;
	}

	int vertexShaderId = CompileShader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
	int fragmentShaderId = CompileShader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);
	m_shaderProgramId = LinkShaders(vertexShaderId, fragmentShaderId);
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
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

int ShadersLoader::LinkShaders(int vertexShaderId, int fragmentShaderId)
{
	m_shaderProgramId = glCreateProgram();
	glAttachShader(m_shaderProgramId, vertexShaderId);
	glAttachShader(m_shaderProgramId, fragmentShaderId);
	glLinkProgram(m_shaderProgramId);

	int success;
	char infoLog[512];
	glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgramId, 512, NULL, infoLog);
		std::cout << "Failed to link shader program!\n" << infoLog << std::endl;
		return -1;
	}

	return m_shaderProgramId;
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