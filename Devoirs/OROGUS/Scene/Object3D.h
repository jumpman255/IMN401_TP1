#ifndef _SCENE_OBJECT3D_H_
#define _SCENE_OBJECT3D_H_

#include "../Utilities/Transforms.h"
#include "../Utilities/Types.h"

#include <string>
#include <vector>

class Geometry;
class Material;
class Scene;

class Object3D
{
    Object3D* m_parent;
    Geometry* m_geometry;
    Material* m_material;
	Material* m_normalMaterial;
    Transform m_transformation;
    const Scene* m_scene;
    
    uint32 m_vao;
	uint32 m_vaoNormals;
    std::string m_name;

    std::vector<Object3D*> m_children;

    void updateVAO() const;
    const Material* getMaterial() const;

public:
    Object3D(const std::string& name, Material* material, Geometry* geometry);
    ~Object3D();

	const std::string& getName() const;

    // Disable copy constructor and assignment operator
    Object3D(const Object3D&) = delete;
    Object3D& operator=(const Object3D&) = delete;

    Transform getObjectTransform() const;
    Transform getTransform() const;   

    void setScene(const Scene* scene);
    void setParent(Object3D* parent);
    void setTransform(const Transform& t);

    void addChildren(Object3D* child);
    void transformObject(const Transform& t);
    
    void render() const;
	void renderNormals() const;
};

#endif
