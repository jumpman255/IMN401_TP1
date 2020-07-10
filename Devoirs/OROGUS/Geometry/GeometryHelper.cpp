#define _USE_MATH_DEFINES

#include "GeometryHelper.h"

#include "Geometry.h"
#include "Math.h"

#include "../Utilities/Maths.h"
#include "../Utilities/Vectors.h"

Geometry* GeometryHelper::CreateTriangle()
{
    std::vector<Vertex> vertices;

    vertices.push_back(Vertex(Point3<Metre>(Metre(0), Metre(0.5f), Metre(0)), Vector3<Real>(), Vector2<Real>()));
    vertices.push_back(Vertex(Point3<Metre>(Metre(0.5f), Metre(-0.5f), Metre(0)), Vector3<Real>(), Vector2<Real>()));
    vertices.push_back(Vertex(Point3<Metre>(Metre(-0.5f), Metre(-0.5f), Metre(0)), Vector3<Real>(), Vector2<Real>()));

    std::vector<uint32> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    Geometry* geom = Geometry::CreateGeometry("Triangle", std::move(vertices), std::move(indices));
    geom->setColor(Color::Red());
    return geom;
}


Geometry* GeometryHelper::CreateGrid(Metre width, Metre depth, uint32 m, uint32 n, float uRepeatRatio, float vRepeatRatio)
{
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;

	// TP2 : � compl�ter
    // Vous n'utiliserez pas les textures pour ce TP, alors vous n'avez pas � consid�rer les param�tres float uRepeatRatio et float vRepeatRatio.
    // Un vertex contient sa position, son vecteur normal et ses coordonn�es de texture. Pour ce TP, les coordonn�es de texture seront (0,0).
    // Vous pouvez vous inspirer du cylindre pour faire la grille.
    
    // Précalculer quelques valeurs utiles
    Metre subWidth = width / m;
    Metre subDepth = depth / n;
    Metre xOffset = width / 2;
    Metre zOffset = depth / 2;
    Vector3<Real> normale = Vector3<Real>(Real(), Real(1), Real());

    // Calculer les vertex
    for (uint32 x = 0; x <= m; ++x) {
        for (uint32 z = 0; z <= n; ++z) {
            Metre px = Metre(x * subWidth) - xOffset;
            Metre pz = Metre(z * subDepth) - zOffset;
            Point3<Metre> p = Point3<Metre>(px, Metre(0), pz);
            vertices.emplace_back(Vertex(p, normale, Vector2<Real>()));
        }
    }

    // Effectuer le maillage
    uint32 verticesWidth = m + 1;
    for (uint32 x = 0; x < m; ++x) {
        for (uint32 z = 0; z < n; ++z) {
            uint32 start = (z * verticesWidth) + x;
            // Ajouter le triangle au coin (0, 0)
            indices.push_back(start);
            indices.push_back(start + 1);
            indices.push_back(start + verticesWidth);

            // Ajouter le triangle au coin (1, 1)
            indices.push_back(start + 1);
            indices.push_back(start + verticesWidth + 1);
            indices.push_back(start + verticesWidth);
        }
    }
    
    Geometry* geom = Geometry::CreateGeometry("Grid", std::move(vertices), std::move(indices));
    return geom;
}

Geometry* GeometryHelper::CreateGrid(Metre width, Metre depth, uint32 m, uint32 n, const Color& color)
{
    Geometry* grid = GeometryHelper::CreateGrid(width, depth, m, n, 1.0f, 1.0f);
    grid->setColor(color);
    return grid;
}

Geometry* GeometryHelper::CreateGrid(Metre width, Metre depth, uint32 m, uint32 n, const ColorRGB& color)
{
    return CreateGrid(width, depth, m, n, Color(color));
}

Geometry* GeometryHelper::CreateBox(Metre width, Metre height, Metre depth, Real textureRatioX, Real textureRatioY)
{
	// TP2 : � compl�ter

	std::vector<Vertex> vertices;
	std::vector<uint32> indices;

	// Points du cube
    Vector3<Metre> offset = Vector3<Metre>(width / 2, height / 2, depth / 2);

	Point3<Metre> p1 = Point3<Metre>(Metre(0), Metre(0), Metre(0)) - offset;
	Point3<Metre> p2 = Point3<Metre>(width, Metre(0), Metre(0)) - offset;
	Point3<Metre> p3 = Point3<Metre>(Metre(0), Metre(0), depth) - offset;
	Point3<Metre> p4 = Point3<Metre>(width, Metre(0), depth) - offset;
	Point3<Metre> p5 = Point3<Metre>(Metre(0), height, Metre(0)) - offset;
	Point3<Metre> p6 = Point3<Metre>(Metre(0), height, depth) - offset;
	Point3<Metre> p7 = Point3<Metre>(width, height, Metre(0)) - offset;
	Point3<Metre> p8 = Point3<Metre>(width, height, depth) - offset;

    const Point3<Metre> faces[6][4] = {
        // Devant & Derrière
        {p3, p4, p6, p8},
        {p7, p2, p5, p1},
        // Dessus & Dessous
        {p1, p2, p3, p4},
        {p5, p6, p7, p8},
        // Gauche & Droite
        {p1, p3, p5, p6},
        {p8, p4, p7, p2},
    };

    // Génère les faces
    uint32 indicesOffset = 0;
    for (uint32 i = 0; i < 6; ++i) {
        Vector3<Real> normal = (faces[i][1] - faces[i][0]).normalized().crossProduct((faces[i][2] - faces[i][0]).normalized());
        vertices.push_back(Vertex(faces[i][0], normal, Vector2<Real>()));
        vertices.push_back(Vertex(faces[i][1], normal, Vector2<Real>()));
        vertices.push_back(Vertex(faces[i][2], normal, Vector2<Real>()));
        vertices.push_back(Vertex(faces[i][3], normal, Vector2<Real>()));

        indices.push_back(indicesOffset + 0);
        indices.push_back(indicesOffset + 1);
        indices.push_back(indicesOffset + 2);

        indices.push_back(indicesOffset + 1);
        indices.push_back(indicesOffset + 3);
        indices.push_back(indicesOffset + 2);
        indicesOffset += 4;
    }

	Geometry* geom = Geometry::CreateGeometry("Cube", std::move(vertices), std::move(indices));
	return geom;
}

Geometry* GeometryHelper::CreateBox(Metre size, const Color& color)
{
    return CreateBox(size, size, size, color);
}

Geometry* GeometryHelper::CreateBox(Metre size, const ColorRGB& color)
{
    return CreateBox(size, Color(color));
}

Geometry* GeometryHelper::CreateBox(Metre width, Metre height, Metre depth)
{
    return CreateBox(width, height, depth, 1.0f, 1.0f);
}

Geometry* GeometryHelper::CreateBox(Metre width, Metre height, Metre depth, const Color& color)
{
    Geometry* box = CreateBox(width, height, depth, 1.0f, 1.0f);
    box->setColor(color);
    return box;
}

Geometry* GeometryHelper::CreateBox(Metre width, Metre height, Metre depth, const ColorRGB& color)
{
    return CreateBox(width, height, depth, Color(color));
}

Geometry* GeometryHelper::CreateSphere(Metre radius, uint32 slices, uint32 stacks)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

	// TP3 : � compl�ter
	// Pour le TP3, vous devez remplacer le code du t�trah�dre suivant par celui d'une sph�re.

	Metre x, y, z, xy;
	float sectorStep = 2 * M_PI / slices;
	float stackStep = M_PI / stacks;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stacks; ++i) {
		stackAngle = M_PI / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);

		for (int j = 0; j <= slices; ++j)
		{
			sectorAngle = j * sectorStep;
			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);
			Point3<Metre> point = Point3<Metre>(x, y, z);

			vertices.push_back(Vertex(point, point.normalized(), Vector2<Real>()));
		}
	}

	uint32 k1, k2;
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (slices + 1);
		k2 = k1 + slices + 1;

		for (int j = 0; j < slices; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	// Fin du code � compl�ter du TP3

    Geometry* geom = Geometry::CreateGeometry("Sphere", std::move(vertices), std::move(indices));
    return geom;
}

Geometry* GeometryHelper::CreateSphere(Metre radius, uint32 slices, uint32 stacks, const Color& color)
{
    Geometry* sphere = CreateSphere(radius, slices, stacks);
    sphere->setColor(color);
    return sphere;
}

Geometry* GeometryHelper::CreateSphere(Metre radius, uint32 slices, uint32 stacks, const ColorRGB& color)
{
    return CreateSphere(radius, slices, stacks, Color(color));
}
            
Geometry* GeometryHelper::CreateCylinder(Metre topRadius, Metre bottomRadius, Metre height, uint32 sliceCount, uint32 stackCount)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    
	// Build Stacks.
    Metre stackHeight = height / stackCount;

    // Amount to increment radius as we move up each stack level from bottom to top.
    Metre radiusStep = (topRadius - bottomRadius) / stackCount;

    uint32 ringCount = stackCount + 1;

    // Compute vertices for each stack ring starting at the bottom and moving up.
    for (uint32 i = 0; i < ringCount; ++i)
    {
        Metre y = -0.5f * height + i * stackHeight;
        Metre r = bottomRadius + i * radiusStep;

        // vertices of ring
        Degree dTheta = 2.0f * Degree(180) / sliceCount;
        for (uint32 j = 0; j <= sliceCount; ++j)
        {
            float c = Maths::Cos(j * dTheta);
            float s = Maths::Sin(j * dTheta);

            Point3<Metre> position = Point3<Metre>(r * c, y, r * s);

            // Cylinder can be parameterized as follows, where we introduce v
            // parameter that goes in the same direction as the v tex-coord
            // so that the bitangent goes in the same direction as the v tex-coord.
            //   Let r0 be the bottom radius and let r1 be the top radius.
            //   y(v) = h - hv for v in [0,1].
            //   r(v) = r1 + (r0-r1)v
            //
            //   x(t, v) = r(v)*cos(t)
            //   y(t, v) = h - hv
            //   z(t, v) = r(v)*sin(t)
            // 
            //  dx/dt = -r(v)*sin(t)
            //  dy/dt = 0
            //  dz/dt = +r(v)*cos(t)
            //
            //  dx/dv = (r0-r1)*cos(t)
            //  dy/dv = -h
            //  dz/dv = (r0-r1)*sin(t)

            // This is unit length.
            Vector3<Real> tangentU = Vector3<Real>(-s, Real(), c);

            Metre dr = bottomRadius - topRadius;
            Vector3<Metre> bitangent = Vector3<Metre>(dr * c, -height, dr * s);

            Vector3<Real> normal = tangentU.crossProduct(bitangent).normalized();

            Vertex v = Vertex(position, normal, Vector2<Real>(Real(float(j)) / sliceCount, 1.0f - Real(float(i)) / stackCount));

            vertices.push_back(v);
        }
    }

    // Add one because we duplicate the first and last vertex per ring
    // since the texture coordinates are different.
    uint32 ringVertexCount = sliceCount + 1;

    // Compute indices for each stack.
    for (uint32 i = 0; i < stackCount; ++i)
    {
        for (uint32 j = 0; j < sliceCount; ++j)
        {
            indices.push_back(i * ringVertexCount + j);
            indices.push_back((i + 1) * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j + 1);

            indices.push_back(i * ringVertexCount + j);
            indices.push_back((i + 1) * ringVertexCount + j + 1);
			indices.push_back(i * ringVertexCount + j + 1);
        }
    }

    // Build cylinder top cap
    uint32 baseIndexTop = (uint32)vertices.size();

    Metre yTop = 0.5f * height;
    Degree dThetaTop = 2.0f * Degree(180) / sliceCount;

    // Duplicate cap ring vertices because the texture coordinates and normals differ.
    for (uint32 i = 0; i <= sliceCount; ++i)
    {
        Metre x = topRadius * Maths::Cos(i * dThetaTop);
        Metre z = topRadius * Maths::Sin(i * dThetaTop);

        // Scale down by the height to try and make top cap texture coord area
        // proportional to base.
        float u = x / height + 0.5f;
        float v = z / height + 0.5f;

        Vertex vert = Vertex(Point3<Metre>(x, yTop, z), Vector3<Real>(Real(), Real(1), Real()), Vector2<Real>(u, v));
        vertices.push_back(vert);
    }

    // Cap center vertex.
    Vertex capVert = Vertex(Point3<Metre>(Metre(), yTop, Metre()), Vector3<Real>(Real(), Real(1), Real()), Vector2<Real>(0.5f, 0.5f));
    vertices.push_back(capVert);

    // Index of center vertex.
    uint32 centerIndexTop = (uint32)(vertices.size() - 1);

    for (uint32 i = 0; i < sliceCount; ++i)
    {
        indices.push_back(centerIndexTop);
        indices.push_back(baseIndexTop + i + 1);
		indices.push_back(baseIndexTop + i);
    }

    //Build cylinder bottom cap
    uint32 baseIndexBottom = (uint32)vertices.size();
    Metre yBottom = -0.5f * height;

    // vertices of ring
    Degree dThetaBottom = Degree(360) / sliceCount;
    for (uint32 i = 0; i <= sliceCount; ++i)
    {
        Metre x = bottomRadius * Maths::Cos(i * dThetaBottom);
        Metre z = bottomRadius * Maths::Sin(i * dThetaBottom);

        // Scale down by the height to try and make top cap texture coord area
        // proportional to base.
        float u = x / height + 0.5f;
        float v = z / height + 0.5f;

        Vertex vert = Vertex(Point3<Metre>(x, yBottom, z), Vector3<Real>(Real(), Real(-1), Real()), Vector2<Real>(u, v));
        vertices.push_back(vert);
    }

    // Cap center vertex.
    capVert = Vertex(Point3<Metre>(Metre(), yBottom, Metre()), Vector3<Real>(Real(), Real(-1), Real()), Vector2<Real>(0.5f, 0.5f));
    vertices.push_back(capVert);

    // Cache the index of center vertex.
    uint32 centerIndexBottom = (uint32)(vertices.size() - 1);

    for (uint32 i = 0; i < sliceCount; ++i)
    {
        indices.push_back(centerIndexBottom);
        indices.push_back(baseIndexBottom + i);
		indices.push_back(baseIndexBottom + i + 1);
    }

    Geometry* geom = Geometry::CreateGeometry("Cylinder", std::move(vertices), std::move(indices));
    return geom;
}

Geometry* GeometryHelper::CreateCylinder(Metre topRadius, Metre bottomRadius, Metre height, uint32 sliceCount, uint32 stackCount, const Color& color)
{
    Geometry* cylinder = CreateCylinder(topRadius, bottomRadius, height, sliceCount, stackCount);
    cylinder->setColor(color);
    return cylinder;
}

Geometry* GeometryHelper::CreateCylinder(Metre topRadius, Metre bottomRadius, Metre height, uint32 sliceCount, uint32 stackCount, const ColorRGB& color)
{
    return CreateCylinder(topRadius, bottomRadius, height, sliceCount, stackCount, Color(color));
}

// Rings : Nombre d'anneau
// Sides : Nombre de points pour une anneau
// Radius : Distance du centre � l'anneau
// ringRadius : Rayon du cercle interne autour de l'anneau
Geometry* GeometryHelper::CreateTorus(Metre radius, Metre ringRadius, uint32 sides, uint32 rings, Color* color)
{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

	// TP3 : Bonus � compl�ter
    // Remplacer le code du t�trah�dre suivant par celui d'un tore (anneau)
    // Thetrahedron
    Point3<Metre> p1 = Point3<Metre>(-radius, Metre(-radius), Metre());
    Point3<Metre> p2 = Point3<Metre>(radius, Metre(-radius), Metre());
    Point3<Metre> p3 = Point3<Metre>(Metre(), Metre(radius), Metre(-radius));
    Point3<Metre> p4 = Point3<Metre>(Metre(), Metre(radius), Metre(radius));

    // Face 1
    Vector3<Real> normalF1 = (p4 - p1).normalized().crossProduct((p3 - p1).normalized());
    Vertex f1v1 = Vertex(p1, normalF1, Vector2<Real>());
    Vertex f1v2 = Vertex(p3, normalF1, Vector2<Real>());
    Vertex f1v3 = Vertex(p4, normalF1, Vector2<Real>());

    vertices.push_back(f1v1);
    vertices.push_back(f1v2);
    vertices.push_back(f1v3);

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);

    // Face 2
    Vector3<Real> normalF2 = (p3 - p2).normalized().crossProduct((p4 - p2).normalized());
    Vertex f2v1 = Vertex(p2, normalF2, Vector2<Real>());
    Vertex f2v2 = Vertex(p3, normalF2, Vector2<Real>());
    Vertex f2v3 = Vertex(p4, normalF2, Vector2<Real>());

    vertices.push_back(f2v1);
    vertices.push_back(f2v2);
    vertices.push_back(f2v3);

    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(5);

    // Face 3
    Vector3<Real> normalF3 = (p1 - p4).normalized().crossProduct((p2 - p4).normalized());
    Vertex f3v1 = Vertex(p4, normalF3, Vector2<Real>());
    Vertex f3v2 = Vertex(p1, normalF3, Vector2<Real>());
    Vertex f3v3 = Vertex(p2, normalF3, Vector2<Real>());

    vertices.push_back(f3v1);
    vertices.push_back(f3v2);
    vertices.push_back(f3v3);

    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(8);

    // Face 4
    Vector3<Real> normalF4 = (p2 - p3).normalized().crossProduct((p1 - p3).normalized());
    Vertex f4v1 = Vertex(p3, normalF4, Vector2<Real>());
    Vertex f4v2 = Vertex(p1, normalF4, Vector2<Real>());
    Vertex f4v3 = Vertex(p2, normalF4, Vector2<Real>());

    vertices.push_back(f4v1);
    vertices.push_back(f4v2);
    vertices.push_back(f4v3);

    indices.push_back(9);
    indices.push_back(11);
    indices.push_back(10);
	// Fin du bonus...

    Geometry* geom = Geometry::CreateGeometry("Torus", std::move(vertices), std::move(indices));
    if (color != nullptr)
    {
        geom->setColor(*color);
    }
    return geom;
}

Geometry* GeometryHelper::CreateRevolutionSurface(const std::vector<Point2<Metre>>& slicePoint, uint32 precision)
{
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;

	// TP3 : � compl�ter
	
	Geometry* geom = Geometry::CreateGeometry("Revolution", std::move(vertices), std::move(indices));
	geom->updateNormals();
	return geom;
}

            