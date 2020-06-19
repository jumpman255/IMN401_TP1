#include <glew/glew.h>

#include "Geometry.h"
#include "../Material/Material.h"
#include "../Utilities/Transforms.h"

#include <iostream>

Geometry* Geometry::CreateGeometry(const std::string& name, std::vector<Vertex>&& vertices, std::vector<uint32>&& indices)
{
    Geometry* geom = new Geometry(name);
    geom->setGeometryData(std::move(vertices), std::move(indices));
	return geom;
}

Geometry* Geometry::Combine(const std::string& name, const Geometry& first, const Geometry& second)
{
    Geometry* geom = new Geometry(name);
    geom->merge(first);
    geom->merge(second);
    return geom;
}

Geometry::Geometry(const std::string& name)
    : m_color(Color::White())
    , m_name(name)
{
    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);
    glGenBuffers(1, &m_normalVertexBuffer);
}

Geometry::~Geometry()
{
    unload();
}

const std::string& Geometry::getName() const
{
    return m_name;
}

Color Geometry::getColor() const
{
    return m_color;
}

void Geometry::setColor(const Color& c)
{
    m_color = c;
}

void Geometry::render(const Material& mat) const
{
    mat.setColor("uColor", getColor());    
    glDrawElements(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Geometry::renderNormal() const
{
    glDrawArrays(GL_LINES, 0, (int)m_normalVertices.size());
}

void Geometry::bindBuffersVAO() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
}

void Geometry::bindBuffersNormalVAO() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_normalVertexBuffer);
}

void Geometry::unload()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_indexBuffer);
    glDeleteBuffers(1, &m_normalVertexBuffer);
    unloadData();    
}

void Geometry::setGeometryData(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices)
{
	unloadData();

	m_vertices = std::move(vertices);
	m_indices = std::move(indices);

	constructTrianglesList();
	updateTangents();

	updateVertexBuffer();
	updateIndexBuffer();
}

void Geometry::updateIndexBuffer() const 
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32), &(m_indices[0]), GL_STATIC_DRAW);
}

void Geometry::updateVertexBuffer()
{
    m_normalVertices.clear();
    for (Vertex& v : m_vertices)
    {
        Point3<Metre> first = v.Position;
        Point3<Metre> second = v.Position + (v.Normal * Metre(1));
        m_normalVertices.push_back(first);
        m_normalVertices.push_back(second);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_normalVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_normalVertices.size() * sizeof(Point3<Metre>), &(m_normalVertices[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &(m_vertices[0]), GL_STATIC_DRAW);	
}

void Geometry::updateTangents()
{
    for (Triangle& t : m_triangles)
    {
        Vertex& v1 = t.Vertex1;
        Vertex& v2 = t.Vertex2;
        Vertex& v3 = t.Vertex3;

        Vector3<Metre> edge1 = v2.Position - v1.Position;
        Vector3<Metre> edge2 = v3.Position - v1.Position;
        Vector2<Real> dUV1 = v2.TexCoord - v1.TexCoord;
        Vector2<Real> dUV2 = v3.TexCoord - v1.TexCoord;

        float f = 1.0f / (dUV1.x() * dUV2.y() - dUV2.x() * dUV1.y());

        v1.Tangent = Vector3<Metre>(f * (dUV2.y() * edge1.x() - dUV1.y() * edge2.x()), f * (dUV2.y() * edge1.y() - dUV1.y() * edge2.y()), f * (dUV2.y() * edge1.z() - dUV1.y() * edge2.z())).normalized();
        v2.Tangent = v1.Tangent;
        v3.Tangent = v1.Tangent;
    }

    for (Vertex& v : m_vertices)
    {
        v.Tangent = v.Tangent.normalized();
    }
}

void Geometry::updateNormals()
{
	// TP3 : À compléter
}

void Geometry::constructTrianglesList()
{
    for (uint32 i = 0; i < m_indices.size(); i += 3)
    {
        Triangle t(m_vertices[m_indices[i]], m_vertices[m_indices[i + 1]], m_vertices[m_indices[i + 2]]);
        m_triangles.push_back(t);
    }
}

void Geometry::unloadData()
{
    m_triangles.clear();
    m_indices.clear();
    m_vertices.clear();
	m_normalVertices.clear();
}

void Geometry::transform(const Transform& t)
{
    for (Vertex& v : m_vertices)
    {
        v.Position = t * v.Position;
    }

	updateVertexBuffer();
}

void Geometry::merge(const Geometry& other)
{
    uint32 nbVertex = (uint32)m_vertices.size();
    for (Vertex v : other.m_vertices)
    {
        m_vertices.push_back(v);
    }

    for (uint32 i : other.m_indices)
    {
        m_indices.push_back(nbVertex + i);
    }

    m_triangles.clear();
    constructTrianglesList();
    updateTangents();

	updateVertexBuffer();
	updateIndexBuffer();
}