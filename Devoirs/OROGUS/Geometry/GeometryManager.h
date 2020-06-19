#ifndef _GEOMETRY_GEOMETRY_MANAGER_H_
#define _GEOMETRY_GEOMETRY_MANAGER_H_

#include "Geometry.h"
#include "../Utilities/InstanceCounter.h"

#include <map>
#include <string>

class GeometryManager
{
private:
	static GeometryManager* s_instance;

	GeometryManager();
	~GeometryManager();

	GeometryManager(const GeometryManager& other) = delete;
	GeometryManager& operator=(const GeometryManager& other) = delete;

	std::map<std::string, InstanceCounter<Geometry>*> m_geometries;
	std::map<Geometry*, std::string> m_inverseLookup;
	

public:
	static GeometryManager* GetInstance();
	static void Initialize();
	static void Uninitialize();

	Geometry* loadGeometry(const std::string& geometryName);
	Geometry* operator[](const std::string& geometryName) const;
	std::string getGeometryName(Geometry * const geom) const;
	bool unloadGeometry(const std::string& geometryName);
	bool unloadGeometry(Geometry*& geometry);
	void unloadAll();
};

#endif
