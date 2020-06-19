#include "Material.h"

#include "Shaders.h"
#include "ShaderManager.h"
#include "../Light/Lights.h"
#include "../Scene/Scene.h"
#include "../Texture/Texture.h"
#include "../Texture/TextureManager.h"
#include "../Utilities/Color.h"
#include "../Utilities/Logger.h"
#include "../Utilities/StringUtilities.h"

#include <iostream>
#include <string>

Material::Material(VertexShader* vShader, FragmentShader* fShader)
	: m_vertexShader(vShader)
	, m_fragmentShader(fShader)
	, m_programId(0)
	, m_isInitialized(false)
    , m_isUsingLighting(false)
{
	if (m_vertexShader != nullptr && m_vertexShader->isValid() && m_fragmentShader != nullptr && m_fragmentShader->isValid())
	{
		m_programId = glCreateProgram();
		if (m_programId > 0)
		{
			glAttachShader(m_programId, m_vertexShader->id());
			glAttachShader(m_programId, m_fragmentShader->id());
			glLinkProgram(m_programId);
			m_isInitialized = validateProgram();
            m_isUsingLighting = glGetUniformLocation(id(), "currentPointLights") != -1;
		}
	}
	else
	{
		Log() << "--Erreur : Probleme lors de la creation du materiel compose du VertexShader " << m_vertexShader->shaderName() << " et du FragmentShader " << m_fragmentShader->shaderName() << "." << std::endl;
	}
}

Material::~Material()
{
	if (m_programId > 0)
	{
		glDetachShader(m_programId, m_vertexShader->id());
		glDetachShader(m_programId, m_fragmentShader->id());
		glDeleteProgram(m_programId);
		m_programId = 0;

        ShaderManager::GetInstance()->UnloadShader(m_vertexShader);
        ShaderManager::GetInstance()->UnloadShader(m_fragmentShader);
        
        for (BindingInfo<Texture2D*>& info : m_textures)
        {
            TextureManager::GetInstance()->unloadTexture(info.Value);
        }
        m_textures.clear();
        m_uniformFloat.clear();
        m_uniformInt.clear();
        m_uniformVec3.clear();
        m_uniformVec4.clear();
	}
}

// Return the id of the shader program
uint32 Material::id() const
{
	return m_programId;
}

uint32 Material::attribute(const char* attName) const
{
    return glGetAttribLocation(m_programId, attName);
}

bool Material::isInitialized() const
{
    return m_isInitialized;
}

bool Material::isUsingLighting() const
{
    return m_isUsingLighting;
}

// Bind the shader
void Material::bind() const
{
    if (isInitialized())
    {
        glUseProgram(m_programId);        

        uint32 bindingUnit = 0;
        for (const BindingInfo<Texture2D*>& info : m_textures)
        {
            info.Value->bind(bindingUnit);
            setInt(info.BindingAttribute, bindingUnit);
            ++bindingUnit;
        }

        for (const BindingInfo<Vector3<Real>>& info : m_uniformVec3)
        {
            setVec3(info.BindingAttribute, info.Value);
        }

        for (const BindingInfo<Vector4<Real>>& info : m_uniformVec4)
        {
            setVec4(info.BindingAttribute, info.Value);
        }

        for (const BindingInfo<Real>& info : m_uniformFloat)
        {
            setFloat(info.BindingAttribute, info.Value);
        }

        for (const BindingInfo<int>& info : m_uniformInt)
        {
            setInt(info.BindingAttribute, info.Value);
        }
    }
}

// Unbind the shader
void Material::unbind() const
{
	glUseProgram(0);
}

void Material::addTextureBinding(const char* bindingName, Texture2D* texture)
{
    BindingInfo<Texture2D*> info;
    info.BindingName = std::string(bindingName);
    info.Value = texture;
    info.BindingAttribute = glGetUniformLocation(id(), bindingName);
    m_textures.push_back(info);
}

void Material::addVec3Binding(const char* bindingName, const Vector3<Real>& value)
{
    BindingInfo<Vector3<Real>> info;
    info.BindingName = std::string(bindingName);
    info.Value = value;
    info.BindingAttribute = glGetUniformLocation(id(), bindingName);
    m_uniformVec3.push_back(info);
}

void Material::addVec4Binding(const char* bindingName, const Vector4<Real>& value)
{
    BindingInfo<Vector4<Real>> info;
    info.BindingName = std::string(bindingName);
    info.Value = value;
    info.BindingAttribute = glGetUniformLocation(id(), bindingName);
    m_uniformVec4.push_back(info);
}

void Material::addFloatBinding(const char* bindingName, Real value)
{
    BindingInfo<Real> info;
    info.BindingName = std::string(bindingName);
    info.Value = value;
    info.BindingAttribute = glGetUniformLocation(id(), bindingName);
    m_uniformFloat.push_back(info);
}

void Material::addIntBinding(const char* bindingName, int value)
{
    BindingInfo<int> info;
    info.BindingName = std::string(bindingName);
    info.Value = value;
    info.BindingAttribute = glGetUniformLocation(id(), bindingName);
    m_uniformInt.push_back(info);
}

void Material::addColorBinding(const char* bindingName, const ColorRGB& c)
{
    addVec3Binding(bindingName, Vector3<Real>(c.r(), c.g(), c.b()));
}

void Material::addColorBinding(const char* bindingName, const Color& c)
{
    addVec4Binding(bindingName, Vector4<Real>(c.r(), c.g(), c.b(), c.a()));
}

void Material::setColor(const char* uniformName, const Color& c) const
{
    uint32 uniformIndex = glGetUniformLocation(id(), uniformName);
    glUniform4fv(uniformIndex, 1, c.constData());
}

void Material::setColor(const char* uniformName, const ColorRGB& c) const
{
    uint32 uniformIndex = glGetUniformLocation(id(), uniformName);
    glUniform3fv(uniformIndex, 1, c.constData());
}

void Material::setInt(const std::string& name, int value) const
{
    setInt(name.c_str(), value);
}

void Material::setInt(const char* uniformName, int val) const
{
    setInt(glGetUniformLocation(id(), uniformName), val);
}

void Material::setInt(uint32 uniformLocation, int val) const
{
    glUniform1i(uniformLocation, val);
}

void Material::setBool(const std::string& name, bool value) const
{
    setBool(name.c_str(), value);
}

void Material::setBool(const char* name, bool value) const
{
    setBool(glGetUniformLocation(id(), name), (int)value);
}

void Material::setBool(uint32 uniformLocation, bool value) const
{
    glUniform1i(uniformLocation, (int)value);
}

void Material::setFloat(const std::string& name, float value) const
{
    setFloat(name.c_str(), value);
}

void Material::setFloat(const char* name, float value) const
{
    setFloat(glGetUniformLocation(id(), name), value);
}

void Material::setFloat(uint32 uniformLocation, float value) const
{
	// TP2 : À compléter
}

void Material::setVec2(const std::string& name, float x, float y) const
{
    setVec2(name.c_str(), x, y);
}

void Material::setVec2(const char* name, float x, float y) const
{
    setVec2(glGetUniformLocation(id(), name), x, y);
}

void Material::setVec2(uint32 uniformLocation, float x, float y) const
{
	glUniform2f(uniformLocation, x, y);
}

void Material::setVec3(const std::string& name, float x, float y, float z) const
{
    setVec3(name.c_str(), x, y, z);
}

void Material::setVec3(const char* name, float x, float y, float z) const
{
	// TP2 : À compléter
}

void Material::setVec3(uint32 uniformLocation, float x, float y, float z) const
{
	// TP2 : À compléter
}

void Material::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    setVec4(name.c_str(), x, y, z, w);
}

void Material::setVec4(const char* name, float x, float y, float z, float w) const
{
    setVec4(glGetUniformLocation(id(), name), x, y, z, w);
}

void Material::setVec4(uint32 uniformLocation, float x, float y, float z, float w) const
{
	glUniform4f(uniformLocation, x, y, z, w);
}

// Validate if the program linked correctly to the shaders
bool Material::validateProgram() const
{
	GLint linkResult;
	glGetProgramiv(m_programId, GL_LINK_STATUS, &linkResult);
	if (linkResult != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            char* logBuffer = new char[infoLogLength];
            glGetProgramInfoLog(m_programId, infoLogLength, nullptr, logBuffer);
            Log() << "--Erreur lors de la liaison du materiel compose du VertexShader " << m_vertexShader->shaderName() << " et du FragmentShader " << m_fragmentShader->shaderName() << std::endl;
            Log() << logBuffer << std::endl;
            delete[] logBuffer;
        }
	}

	glValidateProgram(m_programId);
	GLint status;
	glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            char* logBuffer = new char[infoLogLength];
            glGetProgramInfoLog(m_programId, infoLogLength, nullptr, logBuffer);
            Log() << "Erreur lors de la validation du materiel compose du VertexShader " << m_vertexShader->shaderName() << " et du FragmentShader " << m_fragmentShader->shaderName() << std::endl;
            Log() << logBuffer << std::endl;
            delete[] logBuffer;
        }
	}

	return linkResult == GL_TRUE && status == GL_TRUE;
}

void Material::logBindingDetails() const
{
    Logger::IncIndent();
    Log() << "Detail du materiel : " << std::endl;
    Logger::IncIndent();
    Log() << "VertexShader   : " << m_vertexShader->shaderName() << std::endl;
    Log() << "FragmentShader : " << m_fragmentShader->shaderName() << std::endl;
    Log() << "Attributs et Uniforms actifs :" << std::endl;
    int32 attributeCount = 0;
    glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTES, &attributeCount);
    int32 maxAttributeNameLength = 0;
    glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
    char* attributeName = new char[maxAttributeNameLength];
    attributeName[0] = '\0';
    Logger::IncIndent();
    for (uint32 i = 0; i < (uint32)attributeCount; ++i)
    {
        GLint size;
        GLenum type;
        GLsizei length;
        glGetActiveAttrib(m_programId, i, maxAttributeNameLength, &length, &size, &type, attributeName);
        if (length > 0)
        {
            Log() << "Attribut : " << attributeName << std::endl;
        }
        else
        {
            Log() << "Attribut : Inconnu" << std::endl;
        }
    }
    delete[] attributeName;

    std::cout << std::endl;

    int32 uniformCount = 0;
    glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &uniformCount);
    int32 maxUniformNameLength = 0;
    glGetProgramiv(m_programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
    char* uniformName = new char[maxUniformNameLength];
    uniformName[0] = '\0';
    for (uint32 i = 0; i < (uint32)uniformCount; ++i)
    {
        GLenum type;
        int32 size;
        glGetActiveUniform(m_programId, i, maxUniformNameLength, nullptr, &size, &type, uniformName);
        Log() << "Uniform : ";
        if (type == GL_FLOAT)
        {
            std::cout << "float ";
        }
        else if (type == GL_FLOAT_VEC2)
        {
            std::cout << "vec2 ";
        }
        else if (type == GL_FLOAT_VEC3)
        {
            std::cout << "vec3 ";
        }
        else if (type == GL_FLOAT_VEC4)
        {
            std::cout << "vec4 ";
        }
        else if (type == GL_INT)
        {
            std::cout << "int ";
        }
        else if (type == GL_FLOAT_MAT3)
        {
            std::cout << "mat3 ";
        }
        else if (type == GL_FLOAT_MAT4)
        {
            std::cout << "mat4 ";
        }
        else if (type == GL_SAMPLER_2D)
        {
            std::cout << "sampler2D ";
        }
        else
        {
            std::cout << "autre... ";
        }
        std::cout << uniformName;
        if (size > 1)
        {
            std::cout << "[" << size << "]";
        }
        showBinding(type, uniformName);
        std::cout << std::endl;
    }
    Logger::DecIndent();
    Logger::DecIndent();
    Logger::DecIndent();
    std::cout << std::endl;
    delete[] uniformName;
}

void Material::showBinding(GLenum type, const char* name) const
{
    if (type == GL_FLOAT)
    {
        for (const BindingInfo<Real>& bi : m_uniformFloat)
        {
            if (StringUtilities::Equals(bi.BindingName, name))
            {
                std::cout << " : " << bi.Value;
                return;
            }
        }
    }
    else if (type == GL_FLOAT_VEC3)
    {
        for (const BindingInfo<Vector3<Real>>& bi : m_uniformVec3)
        {
            if (StringUtilities::Equals(bi.BindingName, name))
            {
                std::cout << " : " << bi.Value;
                return;
            }
        }
    }
    else if (type == GL_FLOAT_VEC4)
    {
        for (const BindingInfo<Vector4<Real>>& bi : m_uniformVec4)
        {
            if (StringUtilities::Equals(bi.BindingName, name))
            {
                std::cout << " : " << bi.Value;
                return;
            }
        }
    }
    else if (type == GL_INT)
    {
        for (const BindingInfo<int>& bi : m_uniformInt)
        {
            if (StringUtilities::Equals(bi.BindingName, name))
            {
                std::cout << " : " << bi.Value;
                return;
            }
        }
    }
    else if (type == GL_SAMPLER_2D)
    {
        for (const BindingInfo<Texture2D*>& bi : m_textures)
        {
            if (StringUtilities::Equals(bi.BindingName, name))
            {
                std::cout << " : " << bi.Value->getName();
                return;
            }
        }
    }
    std::cout << " : Aucune valeur fixe prevue...";
}