#ifndef _SCENE_SCENE_H_
#define _SCENE_SCENE_H_

#include "../Camera/Camera.h"
#include "../Utilities/Color.h"
#include "../Utilities/Transforms.h"
#include "../Utilities/Types.h"
#include "../Utilities/Vectors.h"

#include <vector>

class BaseCurve;
class LightObject;
class Material;
class Object3D;

class Scene
{
private:
    std::vector<BaseCurve*> m_curves;
    std::vector<Object3D*> m_objects;
    std::vector<LightObject*> m_lights;

    Camera m_camera;
    ColorRGB m_ambientColor;
    Vector3<Real> m_ambientPower;
	Transform m_sceneTransform;
	Material* m_sceneMaterial;

	uint32 m_currentSelectedObject = 0;
	bool m_showLights;

public:
    Scene();
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    void addCurve(BaseCurve* curve);
    void addObject(Object3D* obj);
    void addLight(LightObject* light);

	uint32 getNbObjects() const;

	Object3D* getCurrentSelectedObject() const;
	void changeSelectedObject(int delta);

	bool lightsVisible() const;
	void showLights(bool show);

    void setAmbientColor(const ColorRGB& ambientColor);
    void setAmbientPower(const Vector3<Real>& ambientPower);
    void setCamera(const Camera& c);
	void setSceneTransform(const Transform& t);
	void setSceneMaterial(Material* material);

	const Transform& getSceneTransform() const;
	const Material* getSceneMaterial() const;

    Camera& getCamera();
    const ColorRGB& getAmbientColor() const;
    const Vector3<Real>& getAmbientPower() const;
    const std::vector<LightObject*>& getLights() const;

    void bind(const Material& m) const;
	void bindNormals(const Material& m) const;
    void render() const;
	void renderNormals() const;
};

#endif
