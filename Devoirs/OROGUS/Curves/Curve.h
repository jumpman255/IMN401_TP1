#ifndef _CURVES_CURVE_H_
#define _CURVES_CURVE_H_

#include "../Utilities/Color.h"
#include "../Utilities/Types.h"
#include "../Utilities/Units.h"
#include "../Utilities/Vectors.h"

#include <string>
#include <vector>

class Material;
class Scene;

struct CurveVertex
{
    Vector3<Metre> Position;
};

class BaseCurve
{
    uint32 m_vertexBuffer[2];
    uint32 m_vao[2];

    Color m_color;
    std::string m_name;
    int m_curvePresicion;

    Material* m_material;
    const Scene* m_scene;

protected:
    std::vector<Vector3<Metre>> m_controlPoints;
    std::vector<CurveVertex> m_vertices;

public:
    BaseCurve(const std::string& name);
    virtual ~BaseCurve();

    BaseCurve(const BaseCurve& other) = delete;
    BaseCurve& operator=(const BaseCurve& other) = delete;

    void addControlPoint(const Vector3<Metre>& point);
    uint32 controlPointsCount() const;

    const std::string& getName() const;

    void setScene(const Scene* scene);

    Color getColor() const;
    void setColor(const Color& c);

    int getCurvePrecision() const;
    void setCurvePrecision(int precision);

    virtual void render() const;
    void unload();

    void updateVAO();
    virtual void updateVertices() = 0;
};

class HermiteCurve : public BaseCurve
{
public:
    HermiteCurve(const std::string& name);
    ~HermiteCurve();

    void updateVertices() override;
};

class BezierCurve : public BaseCurve
{
public:
    BezierCurve(const std::string& name);
    ~BezierCurve();

    void updateVertices() override;
};

class CatmullRomCurve : public BaseCurve
{
public:
    CatmullRomCurve(const std::string& name);
    ~CatmullRomCurve();

    void updateVertices() override;
};

class BSplineCurve : public BaseCurve
{
public:
    BSplineCurve(const std::string& name);
    ~BSplineCurve();

    void updateVertices() override;
};

#endif