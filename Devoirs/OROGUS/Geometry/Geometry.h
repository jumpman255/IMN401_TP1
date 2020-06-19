#ifndef _GEOMETRY_GEOMETRY_H_
#define _GEOMETRY_GEOMETRY_H_

#include "../Utilities/Color.h"
#include "../Utilities/Point.h"
#include "../Utilities/Types.h"
#include "../Utilities/Units.h"
#include "../Utilities/Vectors.h"

#include <string>
#include <vector>

class Material;
class Transform;

struct Vertex
{
    Point3<Metre> Position;
    Vector3<Real> Normal;
    Vector3<Real> Tangent;
    Vector2<Real> TexCoord;

    Vertex(const Point3<Metre>& pos, const Vector3<Real>& normal, const Vector3<Real>& tangent, const Vector2<Real>& uv)
        : Position(pos)
        , Normal(normal)
        , Tangent(tangent)
        , TexCoord(uv)
    {
    }

    Vertex(const Point3<Metre>& pos, const Vector3<Real>& normal, const Vector2<Real>& uv)
        : Position(pos)
        , Normal(normal)
        , Tangent()
        , TexCoord(uv)
    {
    }

    Vertex()
    {
    }

    bool operator==(const Vertex& v1) const
    {
        return Position == v1.Position && Normal == v1.Normal && Tangent == v1.Tangent && TexCoord == v1.TexCoord;
    }

    bool operator!=(const Vertex& v1) const
    {
        return !((*this) == v1);
    }
};

struct Triangle
{
	Vertex& Vertex1;
	Vertex& Vertex2;
	Vertex& Vertex3;

    Triangle(Vertex& v1, Vertex& v2, Vertex& v3)
        : Vertex1(v1)
        , Vertex2(v2)
        , Vertex3(v3)
    {
    }
};

class Geometry
{
private:
	uint32 m_indexBuffer;
	uint32 m_vertexBuffer;
	uint32 m_normalVertexBuffer;

    Color m_color;

    std::string m_name;

    std::vector<Triangle> m_triangles;
    std::vector<Vertex> m_vertices;
    std::vector<uint32> m_indices;
	std::vector<Point3<Metre>> m_normalVertices;
    
    Geometry(const std::string& name);
    void setGeometryData(std::vector<Vertex>&& vertices, std::vector<uint32>&& indices);
    void updateTangents();
    void unloadData();
    void constructTrianglesList();
	void updateVertexBuffer();
	void updateIndexBuffer() const;

public:
	static Geometry* CreateGeometry(const std::string& name, std::vector<Vertex>&& vertices, std::vector<uint32>&& indices);
    static Geometry* Combine(const std::string& name, const Geometry& first, const Geometry& second);
	
	~Geometry();

    // No copy or assignation
	Geometry(const Geometry& other) = delete;
	Geometry& operator=(const Geometry& other) = delete;

    const std::string& getName() const;

    Color getColor() const;
    void setColor(const Color& c);

    void merge(const Geometry& other);
    void transform(const Transform& t);

    void bindBuffersVAO() const;
	void bindBuffersNormalVAO() const;

	void updateNormals();

	void render(const Material& mat) const;
	void renderNormal() const;
	void unload();
};


#endif
