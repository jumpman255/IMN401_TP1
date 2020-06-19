#include "GeometryManager.h"

#include "OBJImporter.h"

#include <iostream>

GeometryManager* GeometryManager::s_instance = nullptr;

GeometryManager* GeometryManager::GetInstance()
{
	return s_instance;
}

void GeometryManager::Initialize()
{
	if (GetInstance() == nullptr)
	{
		s_instance = new GeometryManager();
	}
}

void GeometryManager::Uninitialize()
{
	if (GetInstance() != nullptr)
	{
		delete s_instance;
		s_instance = nullptr;
	}
}

GeometryManager::GeometryManager()
{
}

GeometryManager::~GeometryManager()
{
	unloadAll();
}

Geometry* GeometryManager::loadGeometry(const std::string& geometryName)
{
	if (geometryName.empty())
		return nullptr;

	auto it = m_geometries.find(geometryName);
	if (it != m_geometries.end())
	{
		InstanceCounter<Geometry>* instance = m_geometries[geometryName];
		instance->AddRef();
		return instance->getObjectPtr();
	}
	else
	{
		Geometry* geometry = OBJGeometryImporter::Import(geometryName);			
		if (geometry != nullptr)
		{
			m_geometries.insert(std::pair<std::string, InstanceCounter<Geometry>*>(geometryName, new InstanceCounter<Geometry>(geometry)));
			m_inverseLookup.insert(std::pair<Geometry*, std::string>(geometry, geometryName));
		}

		return geometry;
	}
}

Geometry* GeometryManager::operator[](const std::string& geometryName) const
{
	auto it = m_geometries.find(geometryName);
	if (it != m_geometries.end())
	{
		return (*it).second->getObjectPtr();
	}
	return nullptr;
}

std::string GeometryManager::getGeometryName(Geometry * const geom) const
{
	auto it = m_inverseLookup.find(geom);
	if (it != m_inverseLookup.end())
	{
		return (*it).second;
	}
	return std::string();
}

bool GeometryManager::unloadGeometry(const std::string& geometryName)
{
	auto it = m_geometries.find(geometryName);
	if (it != m_geometries.end())
	{
		(*it).second->RemoveRef();

		if (!(*it).second->hasRef())
		{
			m_inverseLookup.erase((*it).second->getObjectPtr());
			delete (*it).second->getObjectPtr();
			delete (*it).second;
			m_geometries.erase(it);
		}
		return true;
	}
	return false;
}

bool GeometryManager::unloadGeometry(Geometry*& geometry)
{
	const std::string& key = getGeometryName(geometry);
	if (!key.empty())
	{
		InstanceCounter<Geometry>* instance = m_geometries[key];
		instance->RemoveRef();

		if (!instance->hasRef())
		{
            m_inverseLookup.erase(geometry);
			delete geometry;
			delete instance;
			m_geometries.erase(key);			
		}
		// On modifie le pointeur original puisqu'on a demandé 
		// de le decharger. Mais la geometrie pourrait encore exister 
		// car d'autre reference peuvent encore exister.
		geometry = nullptr;
		return true;
	}
    else if (geometry != nullptr)
    {
        // Lorsqu'on demande de décharger une géométrie qui est une forme générée
        // elle n'aura pas de clé dans le map, mais on doit la supprimer quand même.
        delete geometry;
        geometry = nullptr;
        return true;
    }
	return false;
}

void GeometryManager::unloadAll()
{
	if (m_geometries.size() > 0)
	{
        std::cout << "Il reste " << m_geometries.size() << " geometries en memoire..." << std::endl;
		for(auto &gi : m_geometries)
		{
			delete gi.second->getObjectPtr();
			delete gi.second;
		}
		m_geometries.clear();
		m_inverseLookup.clear();
	}
}