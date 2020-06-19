#ifndef _MATERIAL_MATERIAL_H_
#define _MATERIAL_MATERIAL_H_

#include <glew/glew.h>

#include "../Utilities/Matrices.h"
#include "../Utilities/Point.h"
#include "../Utilities/Types.h"
#include "../Utilities/Vectors.h"

#include <string>
#include <vector>

class Color;
class ColorRGB;
class FragmentShader;
class Scene;
class Texture2D;
class VertexShader;

class Material {
private:
    template<typename T>
    struct BindingInfo
    {
        T Value;
        std::string BindingName;
        uint32 BindingAttribute;
    };

    uint32 m_programId;
    bool m_isInitialized;
    bool m_isUsingLighting;
    VertexShader* m_vertexShader;
    FragmentShader* m_fragmentShader;

    std::vector<BindingInfo<Texture2D*> > m_textures;
    std::vector<BindingInfo<Vector3<Real> > > m_uniformVec3;
    std::vector<BindingInfo<Vector4<Real> > > m_uniformVec4;
    std::vector<BindingInfo<Real> > m_uniformFloat;
    std::vector<BindingInfo<int> > m_uniformInt;

    void setBool(uint32 uniformLocation, bool value) const;
    void setInt(uint32 uniformLocation, int value) const;
    void setFloat(uint32 uniformLocation, float value) const;
	void setVec2(uint32 uniformLocation, float x, float y) const;
	void setVec3(uint32 uniformLocation, float x, float y, float z) const;
	void setVec4(uint32 uniformLocation, float x, float y, float z, float w) const;

    template<typename U>
    void setVec2(uint32 uniformLocation, const Vector2<U>& value) const
    {
        glUniform2fv(uniformLocation, 1, value.constValues());
    }

    template<typename U>
    void setVec3(uint32 uniformLocation, const Vector3<U>& value) const
    {
        glUniform3fv(uniformLocation, 1, value.constValues());
    }

    template<typename U>
    void setVec3(uint32 uniformLocation, const Point3<U>& value) const
    {
        glUniform3fv(uniformLocation, 1, value.constValues());
    }

    template<typename U>
    void setVec4(uint32 uniformLocation, const Vector4<U>& value) const
    {
        glUniform4fv(uniformLocation, 1, value.constValues());
    }

    template<typename U>
    void setMat3(uint32 uniformLocation, const Matrix3x3<U>& value) const
    {
        glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, value.constValues());
    }

    template<typename U>
    void setMat4(uint32 uniformLocation, const Matrix4x4<U>& value) const
    {
		// TP2 : À compléter
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value.constValues());
    }

    bool validateProgram() const;
    void showBinding(GLenum type, const char* name) const;
public:
	Material(VertexShader* vShader, FragmentShader* fShader);
	~Material();

	uint32 id() const;
    uint32 attribute(const char* attName) const;

    bool isInitialized() const;
	bool isUsingLighting() const;
    
    void bind() const;
    void unbind() const;

    void logBindingDetails() const;

    void addTextureBinding(const char* bindingName, Texture2D* texture);
    void addVec3Binding(const char* bindingName, const Vector3<Real>& value);
    void addVec4Binding(const char* bindingName, const Vector4<Real>& value);
    void addFloatBinding(const char* bindingName, Real value);
    void addIntBinding(const char* bindingName, int value);
    void addColorBinding(const char* bindingName, const ColorRGB& value);
    void addColorBinding(const char* bindingName, const Color& value);
    
    void setColor(const char* uniformName, const Color& c) const;
    void setColor(const char* uniformName, const ColorRGB& c) const;
    
    void setBool(const std::string &name, bool value) const;
    void setBool(const char* name, bool value) const;
    
    void setInt(const std::string &name, int value) const;
    void setInt(const char* name, int value) const;
    
    void setFloat(const std::string &name, float value) const;
    void setFloat(const char* name, float value) const;

    template<typename T>
    void setUnit(const std::string& name, Unit<T> u) const
    {
        setUnit(name.c_str(), u);
    }

    template<typename T>
    void setUnit(const char* name, Unit<T> u) const
    {
        setFloat(name, u.Value());
    }
    
    template<typename U>
    void setVec2(const std::string &name, const Vector2<U>& value) const
    {
        setVec2(name.c_str(), value);
    }

    template<typename U>
    void setVec2(const char * name, const Vector2<U>& value) const
    {
        setVec2(glGetUniformLocation(id(), name), value);
    }

    void setVec2(const std::string &name, float x, float y) const;
    void setVec2(const char* name, float x, float y) const;
    
    template<typename U>
    void setVec3(const std::string &name, const Vector3<U>& value) const
    {
        setVec3(name.c_str(), value);
    }

    template<typename U>
    void setVec3(const std::string &name, const Point3<U>& value) const
    {
        setVec3(name.c_str(), value);
    }

    template<typename U>
    void setVec3(const char* name, const Vector3<U>& value) const
    {
        setVec3(glGetUniformLocation(id(), name), value);
    }

    template<typename U>
    void setVec3(const char* name, const Point3<U>& value) const
    {
        setVec3(glGetUniformLocation(id(), name), value);
    }

    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec3(const char* name, float x, float y, float z) const;

    template<typename U>
    void setVec4(const std::string &name, const Vector4<U>& value) const
    {
        setVec4(name.c_str(), value);
    }

    template<typename U>
    void setVec4(const char* name, const Vector4<U>& value) const
    {
        setVec4(glGetUniformLocation(id(), name), value);
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setVec4(const char* name, float x, float y, float z, float w) const;

    template<typename U>
    void setMat3(const std::string &name, const Matrix3x3<U>& value) const
    {
        setMat3(name.c_str(), value);
    }

    template<typename U>
    void setMat3(const char* name, const Matrix3x3<U>& value) const
    {
        setMat3(glGetUniformLocation(id(), name), value);
    }

    template<typename U>
    void setMat4(const std::string &name, const Matrix4x4<U>& value) const
    {
        setMat4(name.c_str(), value);
    }

    template<typename U>
    void setMat4(const char* name, const Matrix4x4<U>& value) const
    {
        setMat4(glGetUniformLocation(id(), name), value);
    }
};

#endif