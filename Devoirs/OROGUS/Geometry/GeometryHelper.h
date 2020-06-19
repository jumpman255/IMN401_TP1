#ifndef _GEOMETRY_GEOMETRYHELPER_H_
#define _GEOMETRY_GEOMETRYHELPER_H_

#include "../Utilities/Color.h"
#include "../Utilities/Point.h"
#include "../Utilities/Types.h"
#include "../Utilities/Units.h"

#include <vector>

class Geometry;

class GeometryHelper
{
public:
	static Geometry* CreateGrid(Metre width, Metre depth, uint32 m, uint32 n, float uRepeatRatio, float vRepeatRatio);
    static Geometry* CreateGrid(Metre width, Metre depth, uint32 m, uint32 n, const Color& color);
    static Geometry* CreateGrid(Metre width, Metre depth, uint32 m, uint32 n, const ColorRGB& color);

    static Geometry* CreateBox(Metre width, Metre height, Metre depth, Real textureRatioX, Real textureRatioY);
    static Geometry* CreateBox(Metre size, const Color& color);
    static Geometry* CreateBox(Metre size, const ColorRGB& color);
    static Geometry* CreateBox(Metre width, Metre height, Metre depth);
    static Geometry* CreateBox(Metre width, Metre height, Metre depth, const Color& color);
    static Geometry* CreateBox(Metre width, Metre height, Metre depth, const ColorRGB& color);

    static Geometry* CreateSphere(Metre radius, uint32 slices, uint32 stacks);
    static Geometry* CreateSphere(Metre radius, uint32 slices, uint32 stacks, const Color& color);
    static Geometry* CreateSphere(Metre radius, uint32 slices, uint32 stacks, const ColorRGB& color);

    static Geometry* CreateCylinder(Metre topRadius, Metre bottomRadius, Metre height, uint32 sliceCount, uint32 stackCount);
    static Geometry* CreateCylinder(Metre topRadius, Metre bottomRadius, Metre height, uint32 sliceCount, uint32 stackCount, const Color& color);
    static Geometry* CreateCylinder(Metre topRadius, Metre bottomRadius, Metre height, uint32 sliceCount, uint32 stackCount, const ColorRGB& color);

    static Geometry* CreateTorus(Metre radius, Metre ringRadius, uint32 sides, uint32 rings, Color* color);

    static Geometry* CreateTriangle();

	static Geometry* CreateRevolutionSurface(const std::vector<Point2<Metre>>& slicePoints, uint32 precision);
};

#endif
