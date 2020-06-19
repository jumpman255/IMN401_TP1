#ifndef _GEOMETRY_OBJIMPORTER_H_
#define _GEOMETRY_OBJIMPORTER_H_

#include <string>

class Geometry;

class OBJGeometryImporter
{
public:
	static Geometry* Import(const std::string& fileName);
};

#endif
