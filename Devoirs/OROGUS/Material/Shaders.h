#ifndef _MATERIAL_SHADERS_H_
#define _MATERIAL_SHADERS_H_

// GLEW must be included first
#include <glew/glew.h>

#include "../Utilities/Types.h"

#include <string>

// =====================================
// Base class for shader representation
// =====================================
class BaseShader
{
public:
	BaseShader(const std::string& shaderName, const std::string& shaderCode, GLenum shaderType);
	virtual ~BaseShader();

    // No default constructor, copy constructor and assignation operator
	BaseShader() = delete;
    BaseShader(const BaseShader&) = delete;
	BaseShader& operator=(const BaseShader&) = delete;

	bool operator==(const BaseShader& other) const;
	bool operator!=(const BaseShader& other) const;

	uint32 id() const;
	bool isValid() const;
	const std::string& shaderName() const;

protected:
	virtual GLenum shaderType() const = 0;

private:
	bool validateShader(GLuint shader, const std::string& shaderName);

	uint32 m_shaderId;
	bool m_isInitialized;
	std::string m_shaderName;
};


// =====================================
// Class for Vertex shader definition
// =====================================
class VertexShader : public BaseShader {
public:
	VertexShader(const std::string& shaderName, const std::string& shaderCode);
	virtual ~VertexShader();

    // Disable default constructor, copy constructor and assignation operator
	VertexShader() = delete;
    VertexShader(const VertexShader& other) = delete;
	VertexShader& operator=(const VertexShader& other) = delete;
protected:
	GLenum shaderType() const override;
};


// =====================================
// Class for Fragment shader definition
// =====================================
class FragmentShader : public BaseShader {
public:
	FragmentShader(const std::string& shaderName, const std::string& shaderCode);
	virtual ~FragmentShader();

    // Disable default constructor, copy constructor and assignation operator
	FragmentShader() = delete;
    FragmentShader(const FragmentShader& other) = delete;
	FragmentShader& operator=(const FragmentShader& other) = delete;
protected:
	GLenum shaderType() const override;
};

#endif
