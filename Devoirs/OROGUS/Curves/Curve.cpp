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
	for (int i = 0; i <= getCurvePrecision(); i++) {
		float t = (float)i / (float)getCurvePrecision();
		float h1 = -1 * pow(t, 3) + 3 * pow(t, 2) + -3 * t + 1;
		float h2 = 3 * pow(t, 3) + -6 * pow(t, 2) + 3 * t;
		float h3 = -3 * pow(t, 3) + 3 * pow(t, 2);
		float h4 = pow(t, 3);
		Vector3<Metre> point = Vector3<Metre>(h1 * m_controlPoints[0] + h2 * m_controlPoints[1] + h3 * m_controlPoints[2] + h4 * m_controlPoints[3]);
		CurveVertex vertice;
		vertice.Position = point;
		m_vertices.push_back(vertice);
	}
	CurveVertex vertice;
	vertice.Position = m_controlPoints[3];
	m_vertices.push_back(vertice);
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
	Vector3<Metre> p1 = m_controlPoints[0];
	Vector3<Metre> p2 = m_controlPoints[3];
	Vector3<Metre> r1 = 3 * (m_controlPoints[1] - m_controlPoints[0]);
	Vector3<Metre> r2 = 3 * (m_controlPoints[3] - m_controlPoints[2]);
	for (int i = 0; i <= getCurvePrecision(); i++) {
		float t = (float)i / (float)getCurvePrecision();
		float h1 = 2 * pow(t, 3) - 3 * pow(t, 2) + 1;
		float h2 = -2 * pow(t, 3) + 3 * pow(t, 2);
		float h3 = pow(t, 3) - 2 * pow(t, 2) + t;
		float h4 = pow(t, 3) - pow(t, 2);
		Vector3<Metre> point = Vector3<Metre>(h1 * p1 + h2 * p2 + h3 * r1 + h4 * r2);
		CurveVertex vertice;
		vertice.Position = point;
		m_vertices.push_back(vertice);
	}
	CurveVertex vertice;
	vertice.Position = m_controlPoints[3];
	m_vertices.push_back(vertice);
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
	float s = 0.5f;

	float split = getCurvePrecision()/(m_controlPoints.size() - 3);
	for (std::size_t i = 2; i < m_controlPoints.size() - 1; ++i) {
		for (int j = 0; j <= split; j++) {
			float t = (float)j / split;
			float h1 = -s * pow(t, 3) + 2 * s * pow(t, 2) + -s * t;
			float h2 = (2 - s) * pow(t, 3) + (s - 3) * pow(t, 2) + 1;
			float h3 = (s - 2) * pow(t, 3) + (3 - 2 * s) * pow(t, 2) + t * s;
			float h4 = s * pow(t, 3) + (-s) * pow(t, 2);
			Vector3<Metre> point = Vector3<Metre>(h1 * m_controlPoints[i - 2] + h2 * m_controlPoints[i - 1] + h3 * m_controlPoints[i] + h4 * m_controlPoints[i + 1]);
			CurveVertex vertice;
			vertice.Position = point;
			m_vertices.push_back(vertice);
		}
		CurveVertex vertice;
		vertice.Position = m_controlPoints[i + 1];
		m_vertices.push_back(vertice);
	}
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
	float split = getCurvePrecision() / (m_controlPoints.size() - 3);
	for (std::size_t i = 3; i < m_controlPoints.size(); ++i) {
		for (int j = 0; j <= split; j++) {
			float t = (float)j / split;
			float h1 = pow(1 - t, 3)/6;
			float h2 = (3 * pow(t, 3) -6 * pow(t, 2) + 4)/6;
			float h3 = (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)/6;
			float h4 = pow(t, 3)/6;
			Vector3<Metre> point = Vector3<Metre>(h1 * m_controlPoints[i - 3] + h2 * m_controlPoints[i - 2] + h3 * m_controlPoints[i - 1] + h4 * m_controlPoints[i]);
			CurveVertex vertice;
			vertice.Position = point;
			m_vertices.push_back(vertice);
		}
		CurveVertex vertice;
		vertice.Position = m_controlPoints[i];
		m_vertices.push_back(vertice);
	}
}
