#include <glew/glew.h>

#include "Lights.h"

#include "../Geometry/Geometry.h"
#include "../Geometry/GeometryHelper.h"
#include "../Material/ShaderHelper.h"
#include "../Scene/Scene.h"

LightObject::LightObject()
    : m_enabled(true)
    , m_material(ShaderHelper::LoadBaseVertexShader(), ShaderHelper::LoadBaseNoLitFragmentShader())
	, m_geometry(nullptr)
{
    glGenVertexArrays(1, &m_vao);
}

LightObject::~LightObject()
{
    delete m_geometry;
    glDeleteVertexArrays(1, &m_vao);
}

void LightObject::updateVAO() const
{
    glBindVertexArray(m_vao);
    if (m_material.isInitialized())
    {
        uint32 posAttribute = m_material.attribute("aPosition");
        uint32 normalAttribute = m_material.attribute("aNormal");
        uint32 uvAttribute = m_material.attribute("aTexCoord");
        glEnableVertexAttribArray(posAttribute);
        glEnableVertexAttribArray(normalAttribute);
        glEnableVertexAttribArray(uvAttribute);

        if (m_geometry != nullptr)
        {
            m_geometry->bindBuffersVAO();
        }

        uint32 vertexSize = sizeof(Vertex);
        glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
        glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)12);
        glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)24);
    }
    glBindVertexArray(0);
}

void LightObject::updateGeometry(const ColorRGB& color)
{
	if (m_geometry != nullptr)
	{
		delete m_geometry;
		m_geometry = nullptr;
	}
    m_geometry = createRenderGeometry(color);
    updateVAO();
}

void LightObject::setScene(const Scene* scene)
{
    m_scene = scene;
}

void LightObject::render()
{
    glBindVertexArray(m_vao);
    if (m_material.isInitialized() && m_geometry != nullptr)
    {
        m_material.bind();
        m_material.setMat4("gModelMatrix", m_scene->getSceneTransform() * getModelTransform());
        m_scene->bind(m_material);
		if (m_geometry != nullptr)
		{
			m_geometry->render(m_material);
		}
    }
    glBindVertexArray(0);
}


DirectionalLight::DirectionalLight(const Vector3<Real>& direction, const ColorRGB& ambientColor, const ColorRGB& diffuseColor, const ColorRGB& specularColor)
    : LightObject()
{
    m_attribute.Direction = direction.normalized();
    m_attribute.AmbientColor = ambientColor;
    m_attribute.DiffuseColor = diffuseColor;
    m_attribute.SpecularColor = specularColor;

    updateGeometry(diffuseColor);
}

DirectionalLight::~DirectionalLight()
{
}

Geometry* DirectionalLight::createRenderGeometry(const ColorRGB& color) const
{
    // On crée une flèche composée d'un cone et d'un cylindre
    Geometry* cone = GeometryHelper::CreateCylinder(Metre(0), Metre(0.2f), Metre(0.5f), 10, 4, color);
    Geometry* cylinder = GeometryHelper::CreateCylinder(Metre(0.1f), Metre(0.1f), Metre(0.6f), 10, 2, color);
    cone->transform(Transform::MakeTranslation(Vector3<Metre>(Metre(0), Metre(0.55f), Metre(0))));
    Geometry* arrow = Geometry::Combine("Fleche", *cone, *cylinder);
    delete cylinder;
    delete cone;
    return arrow;
}

Transform DirectionalLight::getModelTransform() const
{
    Vector3<Real> up = Vector3<Real>(0, 1, 0);
    Real cosAngle = up.dotProduct(m_attribute.Direction);
    Vector3<Real> w = up.crossProduct(m_attribute.Direction).normalized();
    Real sinAngle = w.length();
    return Transform::MakeRotation(w, cosAngle, sinAngle);
}

LightType DirectionalLight::getType() const
{
    return LightType::Directional;
}

const Vector3<Real>& DirectionalLight::getDirection() const
{
    return m_attribute.Direction;
}

const ColorRGB& DirectionalLight::getAmbientColor() const
{
    return m_attribute.AmbientColor;
}

const ColorRGB& DirectionalLight::getDiffuseColor() const
{
    return m_attribute.DiffuseColor;
}

const ColorRGB& DirectionalLight::getSpecularColor() const
{
    return m_attribute.SpecularColor;
}




PointLight::PointLight(const Point3<Metre>& position, const ColorRGB& ambientColor, const ColorRGB& diffuseColor, const ColorRGB& specularColor, float constant, float linear, float quadratic)
    : LightObject()
{
    m_attribute.Position = position;
    m_attribute.AmbientColor = ambientColor;
    m_attribute.DiffuseColor = diffuseColor;
    m_attribute.SpecularColor = specularColor;
    m_attribute.ConstantFactor = constant;
    m_attribute.LinearFactor = linear;
    m_attribute.QuadraticFactor = quadratic;

    updateGeometry(diffuseColor);
}

PointLight::~PointLight()
{
}

Geometry* PointLight::createRenderGeometry(const ColorRGB& color) const
{
    // On crée une sphère pour ce PointLight
    return GeometryHelper::CreateSphere(Metre(0.1f), 10, 10, color);
}

Transform PointLight::getModelTransform() const
{
    return Transform::MakeTranslation(m_attribute.Position);
}

LightType PointLight::getType() const
{
    return LightType::Point;
}

const Point3<Metre>& PointLight::getPosition() const
{
    return m_attribute.Position;
}

const ColorRGB& PointLight::getAmbientColor() const
{
    return m_attribute.AmbientColor;
}

const ColorRGB& PointLight::getDiffuseColor() const
{
    return m_attribute.DiffuseColor;
}

const ColorRGB& PointLight::getSpecularColor() const
{
    return m_attribute.SpecularColor;
}

Real PointLight::getConstantAttenuationCoefficient() const
{
    return m_attribute.ConstantFactor;
}

Real PointLight::getLinearAttenuationCoefficient() const
{
    return m_attribute.LinearFactor;
}

Real PointLight::getQuadraticAttenuationCoefficient() const
{
    return m_attribute.QuadraticFactor;
}



SpotLight::SpotLight(const Point3<Metre>& position, const ColorRGB& ambientColor, const ColorRGB& diffuseColor, const ColorRGB& specularColor, const Vector3<Real>& direction, Radian angle)
    : LightObject()
{
    m_attribute.Position = position;
    m_attribute.AmbientColor = ambientColor;
    m_attribute.DiffuseColor = diffuseColor;
    m_attribute.SpecularColor = specularColor;
    m_attribute.Direction = direction;
    m_attribute.CosAngle = Maths::Cos(angle/2);
    
    updateGeometry(diffuseColor);
}

SpotLight::~SpotLight()
{
}

Geometry* SpotLight::createRenderGeometry(const ColorRGB& color) const
{
    // On crée un cone dont l'angle du sommet est celui du spot
    Metre adjacent = Metre(1);
    Metre hypo = adjacent / m_attribute.CosAngle;
    Metre overture = Maths::Sqrt((hypo * hypo) - (adjacent * adjacent));
    return GeometryHelper::CreateCylinder(overture, Metre(), adjacent, 10, 10, color);
}

Transform SpotLight::getModelTransform() const
{
    Vector3<Real> up = Vector3<Real>(0, 1, 0);
    Real cosAngle = up.dotProduct(m_attribute.Direction);
    Vector3<Real> w = up.crossProduct(m_attribute.Direction).normalized();
    Real sinAngle = w.length();
    return Transform::MakeTranslation(m_attribute.Position) * Transform::MakeRotation(w, cosAngle, sinAngle);
}

LightType SpotLight::getType() const
{
    return LightType::Spot;
}

const Point3<Metre>& SpotLight::getPosition() const
{
    return m_attribute.Position;
}

const ColorRGB& SpotLight::getAmbientColor() const
{
    return m_attribute.AmbientColor;
}

const ColorRGB& SpotLight::getDiffuseColor() const
{
    return m_attribute.DiffuseColor;
}

const ColorRGB& SpotLight::getSpecularColor() const
{
    return m_attribute.SpecularColor;
}

const Vector3<Real>& SpotLight::getDirection() const
{
    return m_attribute.Direction;
}

Real SpotLight::getCosAngle() const
{
    return m_attribute.CosAngle;
}