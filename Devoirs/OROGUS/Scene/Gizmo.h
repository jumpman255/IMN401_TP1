#ifndef SCENE_GIZMO_H_
#define SCENE_GIZMO_H_

#include "../Utilities/Transforms.h"
#include "../Utilities/Types.h"

class Geometry;
class Material;
class Scene;

class Gizmo
{
	Geometry* m_axeX;
	Geometry* m_axeY;
	Geometry* m_axeZ;
	Material* m_material;
	Transform m_gizmoTransform;
	uint32 m_vao[3];

public:
	Gizmo();
	~Gizmo();

	Gizmo(const Gizmo&) = delete;
	Gizmo& operator=(const Gizmo&) = delete;

	void setTransform(const Transform& t);
	const Transform& getTransform() const;

	void render(const Scene& scene);
};

#endif
