#ifndef _LIGHT_LIGHTS_H_
#define _LIGHT_LIGHTS_H_

#include "../Material/Material.h"
#include "../Utilities/Color.h"
#include "../Utilities/Point.h"
#include "../Utilities/Transforms.h"
#include "../Utilities/Types.h"
#include "../Utilities/Units.h"
#include "../Utilities/Vectors.h"

class Geometry;
class Scene;

enum class LightType
{
    Directional,
    Point,
    Spot
};


class LightObject
{
private:
    const Scene* m_scene;
    Geometry* m_geometry;
    Material m_material;
    bool m_enabled;
    uint32 m_vao;

protected:    
    virtual Geometry* createRenderGeometry(const ColorRGB& color) const = 0;
    void updateVAO() const;
    void updateGeometry(const ColorRGB& color);

public:
    LightObject();
    virtual ~LightObject();

    LightObject(const LightObject&) = delete;
    LightObject& operator=(const LightObject&) = delete;

    virtual Transform getModelTransform() const = 0;
    virtual LightType getType() const = 0;

    void setScene(const Scene* scene);
    void render();    
};


class DirectionalLight : public LightObject
{
private:
    struct DirectionalLightAttribute
    {
        Vector3<Real> Direction;
        ColorRGB AmbientColor;
        ColorRGB DiffuseColor;
        ColorRGB SpecularColor;
    };

    DirectionalLightAttribute m_attribute;

protected:
    Geometry* createRenderGeometry(const ColorRGB& color) const override;

public:
    DirectionalLight(const Vector3<Real>& direction, const ColorRGB& ambientColor, const ColorRGB& diffuseColor, const ColorRGB& specularColor);
    ~DirectionalLight();

    DirectionalLight(const DirectionalLight&) = delete;
    DirectionalLight& operator=(const DirectionalLight&) = delete;

    Transform getModelTransform() const override;
    LightType getType() const override;

    const Vector3<Real>& getDirection() const;
    const ColorRGB& getAmbientColor() const;
    const ColorRGB& getDiffuseColor() const;
    const ColorRGB& getSpecularColor() const;
};


class PointLight : public LightObject
{
private:
    struct PointLightAttribute
    {
        Point3<Metre> Position;
        ColorRGB AmbientColor;
        ColorRGB DiffuseColor;
        ColorRGB SpecularColor;
        Real ConstantFactor;
        Real LinearFactor;
        Real QuadraticFactor;
    };

    PointLightAttribute m_attribute;

protected:
    Geometry* createRenderGeometry(const ColorRGB& color) const override;

public:
    PointLight(const Point3<Metre>& position, const ColorRGB& ambientColor, const ColorRGB& diffuseColor, const ColorRGB& specularColor, float constant, float linear, float quadradic);
    ~PointLight();

    PointLight(const PointLight&) = delete;
    PointLight& operator=(const PointLight&) = delete;

    Transform getModelTransform() const override;
    LightType getType() const override;

    const Point3<Metre>& getPosition() const;
    const ColorRGB& getAmbientColor() const;
    const ColorRGB& getDiffuseColor() const;
    const ColorRGB& getSpecularColor() const;
    Real getConstantAttenuationCoefficient() const;
    Real getLinearAttenuationCoefficient() const;
    Real getQuadraticAttenuationCoefficient() const;
};


class SpotLight : public LightObject
{
private:
    struct SpotLightAttribute
    {
        Point3<Metre> Position;
        ColorRGB AmbientColor;
        ColorRGB DiffuseColor;
        ColorRGB SpecularColor;
        Vector3<Real> Direction;
        Real CosAngle;
    };

    SpotLightAttribute m_attribute;

protected:
    Geometry* createRenderGeometry(const ColorRGB& color) const override;

public:
    SpotLight(const Point3<Metre>& position, const ColorRGB& ambientColor, const ColorRGB& diffuseColor, const ColorRGB& specularColor, const Vector3<Real>& direction, Radian angle);
    ~SpotLight();

    SpotLight(const SpotLight&) = delete;
    SpotLight& operator=(const SpotLight&) = delete;

    Transform getModelTransform() const override;
    LightType getType() const override;

    const Point3<Metre>& getPosition() const;
    const ColorRGB& getAmbientColor() const;
    const ColorRGB& getDiffuseColor() const;
    const ColorRGB& getSpecularColor() const;
    const Vector3<Real>& getDirection() const;
    Real getCosAngle() const;
};

#endif
