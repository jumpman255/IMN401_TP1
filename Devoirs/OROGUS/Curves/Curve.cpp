#include "Curve.h"
#include "../Material/Material.h"
#include "../Material/ShaderHelper.h"
#include "../Scene/Scene.h"

#include <cmath>

BaseCurve::BaseCurve(const std::string& name)
    : m_name(name)
{
    glGenBuffers(2, m_vertexBuffer);
    glCreateVertexArrays(2, m_vao);

    m_material = new Material(ShaderHelper::LoadBaseCurveVertexShader(), ShaderHelper::LoadBaseCurveFragmentShader());
}

BaseCurve::~BaseCurve()
{
    unload();
}

void BaseCurve::addControlPoint(const Vector3<Metre>& point)
{
    m_controlPoints.push_back(point);
}

uint32 BaseCurve::controlPointsCount() const
{
    return (uint32)m_controlPoints.size();
}

void BaseCurve::setScene(const Scene* scene)
{
    m_scene = scene;
}

const std::string& BaseCurve::getName() const
{
    return m_name;
}

Color BaseCurve::getColor() const
{
    return m_color;
}

void BaseCurve::setColor(const Color& c)
{
    m_color = c;
}

int BaseCurve::getCurvePrecision() const
{
    return m_curvePresicion;
}

void BaseCurve::setCurvePrecision(int precision)
{
    m_curvePresicion = precision;
}

void BaseCurve::render() const
{
    glBindVertexArray(m_vao[1]);
    if (m_material != nullptr)
    {
        m_material->bind();
        m_scene->bind(*m_material);
        m_material->setColor("gColor", m_color);

        glPointSize(10.0f);
        glDrawArrays(GL_POINTS, 0, (int)m_controlPoints.size());

        m_material->unbind();
    }
    glBindVertexArray(0);

    glBindVertexArray(m_vao[0]);
    if (m_material != nullptr)
    {
        m_material->bind();
        m_scene->bind(*m_material);
        m_material->setColor("gColor", m_color);

        glDrawArrays(GL_LINE_STRIP, 0, (int)m_vertices.size() - 1);

        m_material->unbind();
    }
    glBindVertexArray(0);
    

}

void BaseCurve::unload()
{
	delete m_material;
	m_material = nullptr;
	m_controlPoints.clear();
	m_vertices.clear();
    glDeleteVertexArrays(2, m_vao);
    glDeleteBuffers(2, m_vertexBuffer);
}

void BaseCurve::updateVAO()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(CurveVertex), &(m_vertices[0]), GL_STATIC_DRAW);

    glBindVertexArray(m_vao[0]);
    uint32 posAttribute = m_material->attribute("aPosition");
    glEnableVertexAttribArray(posAttribute);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[0]);

    uint32 vertexSize = sizeof(CurveVertex);
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[1]);
    CurveVertex* points = new CurveVertex[m_controlPoints.size()];
    int i = 0;
    for (const Vector3<Metre>& p : m_controlPoints)
    {
        points[i].Position = p;
        ++i;
    }
    glBufferData(GL_ARRAY_BUFFER, m_controlPoints.size() * sizeof(CurveVertex), points, GL_STATIC_DRAW);
    delete points;

    glBindVertexArray(m_vao[1]);
    posAttribute = m_material->attribute("aPosition");
    glEnableVertexAttribArray(posAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[1]);

    vertexSize = sizeof(CurveVertex);
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    glBindVertexArray(0);
}



BezierCurve::BezierCurve(const std::string& name)
    :BaseCurve(name)
{
}

BezierCurve::~BezierCurve()
{
}

void BezierCurve::updateVertices()
{
	// TP4 : À compléter
}


HermiteCurve::HermiteCurve(const std::string& name)
    :BaseCurve(name)
{
}

HermiteCurve::~HermiteCurve()
{
}

void HermiteCurve::updateVertices()
{
	// TP4 : À compléter
}


CatmullRomCurve::CatmullRomCurve(const std::string& name)
    :BaseCurve(name)
{
}

CatmullRomCurve::~CatmullRomCurve()
{
}

void CatmullRomCurve::updateVertices()
{
	// TP4 : À compléter
}


BSplineCurve::BSplineCurve(const std::string& name)
    :BaseCurve(name)
{
}

BSplineCurve::~BSplineCurve()
{
}

void BSplineCurve::updateVertices()
{
	// TP4 : À compléter
}
