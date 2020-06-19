#include "Shaders.h"

#include <fstream>
#include <iostream>
#include <sstream>


BaseShader::BaseShader(const std::string& shaderName, const std::string& shaderCode, GLenum shaderType)
	: m_shaderId(0)
	, m_isInitialized(false)
	, m_shaderName(shaderName)
{
	m_shaderId = glCreateShader(shaderType);
	if (m_shaderId == 0)
	{
		std::cout << "Erreur lors de la creation d'un shader (" << shaderName << ")." << std::endl;
		return;
	}

	const char *shaderText = shaderCode.c_str();
	glShaderSource(m_shaderId, 1, &shaderText, 0);
	glCompileShader(m_shaderId);
	m_isInitialized = validateShader(m_shaderId, shaderName);
}

BaseShader::~BaseShader()
{
	glDeleteShader(m_shaderId);
	m_shaderId = 0;
	m_isInitialized = false;
    m_shaderName = "";
}

uint32 BaseShader::id() const
{
	return m_shaderId;
}

bool BaseShader::isValid() const
{
	return m_isInitialized;
}

const std::string& BaseShader::shaderName() const
{
	return m_shaderName;
}

bool BaseShader::operator==(const BaseShader& other) const
{
	return id() == other.id() && isValid() == other.isValid();
}

bool BaseShader::operator!=(const BaseShader& other) const
{
	return !(*this == other);
}

// Validate if the shaders compiled correctly
bool BaseShader::validateShader(GLuint shader, const std::string& shaderName)
{
	GLint compileResult;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult != GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar * buffer = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, nullptr, buffer);
		std::cout << "Shader " << shaderName << " (" << shader << ") errors log : " << std::endl << buffer << std::endl;
		delete[] buffer;
	}
	return compileResult == GL_TRUE;
}

VertexShader::VertexShader(const std::string& shaderName, const std::string& shaderCode)
	: BaseShader(shaderName, shaderCode, shaderType())
{
}

VertexShader::~VertexShader()
{
}

GLenum VertexShader::shaderType() const
{
	return GL_VERTEX_SHADER;
}

FragmentShader::FragmentShader(const std::string& shaderName, const std::string& shaderCode)
	: BaseShader(shaderName, shaderCode, shaderType())
{
}

FragmentShader::~FragmentShader()
{
}

GLenum FragmentShader::shaderType() const
{
	return GL_FRAGMENT_SHADER;
}

