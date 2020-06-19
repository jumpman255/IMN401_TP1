#include "Scene.h"

#include "Object3D.h"
#include "../Camera/Camera.h"
#include "../Curves/Curve.h"
#include "../Light/Lights.h"
#include "../Material/Material.h"

Scene::Scene()
    : m_ambientColor(ColorRGB::Black())
    , m_ambientPower(0, 0, 0)
	, m_currentSelectedObject(0)
	, m_showLights(true)
	, m_sceneMaterial(nullptr)
{
    m_camera.reset();
}

Scene::~Scene()
{
    for (Object3D* obj : m_objects)
    {
        delete obj;
    }

    for (LightObject* obj : m_lights)
    {
        delete obj;
    }

	for (BaseCurve* obj : m_curves)
	{
		delete obj;
	}

	m_objects.clear();
	m_lights.clear();
	m_curves.clear();

	if (m_sceneMaterial != nullptr)
	{
		delete m_sceneMaterial;
		m_sceneMaterial = nullptr;
	}
}

void Scene::addCurve(BaseCurve* curve)
{
    if (curve != nullptr)
    {
        m_curves.push_back(curve);
        curve->setScene(this);
    }
}

void Scene::addObject(Object3D* obj)
{
    if (obj != nullptr)
    {
        m_objects.push_back(obj);
        obj->setScene(this);
    }
}

void Scene::addLight(LightObject* light)
{
    if (light != nullptr)
    {
        m_lights.push_back(light);
        light->setScene(this);
    }
}

uint32 Scene::getNbObjects() const
{
	return (uint32)m_objects.size();
}

Object3D* Scene::getCurrentSelectedObject() const
{
	if (getNbObjects() > 0)
	{
		return m_objects[m_currentSelectedObject];
	}
	return nullptr;
}

void Scene::changeSelectedObject(int delta)
{
	int nextSelected = m_currentSelectedObject + delta;
	if (nextSelected < 0)
	{
		int count = -nextSelected / getNbObjects();
		count += 1;
		nextSelected += count * getNbObjects();
	}
	m_currentSelectedObject = nextSelected % getNbObjects();
}

bool Scene::lightsVisible() const
{
	return m_showLights;
}

void Scene::showLights(bool show)
{
	m_showLights = show;
}

void Scene::setCamera(const Camera& c)
{
    m_camera = c;
}

void Scene::setAmbientColor(const ColorRGB& c)
{
    m_ambientColor = c;
}

void Scene::setAmbientPower(const Vector3<Real>& p)
{
    m_ambientPower = p;
}

void Scene::setSceneTransform(const Transform& t)
{
	m_sceneTransform = t;
}

const Transform& Scene::getSceneTransform() const
{
	return m_sceneTransform;
}

void Scene::setSceneMaterial(Material* material)
{
	m_sceneMaterial = material;
}

const Material* Scene::getSceneMaterial() const
{
	return m_sceneMaterial;
}

Camera& Scene::getCamera()
{
    return m_camera;
}

const ColorRGB& Scene::getAmbientColor() const
{
    return m_ambientColor;
}

const Vector3<Real>& Scene::getAmbientPower() const
{
    return m_ambientPower;
}

const std::vector<LightObject*>& Scene::getLights() const
{
    return m_lights;
}

void Scene::bind(const Material& m) const
{
    if (m.isInitialized())
    {
        m.setMat4("gProjectionMatrix", m_camera.getPerspective());
        m.setMat4("gViewMatrix", m_camera.getView());
        m.setVec3("cameraPosition", m_camera.position());
        
        if (m.isUsingLighting())
        {
            m.setColor("ambientColor", getAmbientColor());
            m.setVec3("ambientPower", getAmbientPower());

            const std::vector<LightObject*> lights = getLights();
            uint32 dirCount = 0;
            uint32 pointCount = 0;
            uint32 spotCount = 0;
            for (const LightObject* l : lights)
            {
                if (l->getType() == LightType::Point)
                {
                    std::string noLight = std::to_string(pointCount);
                    const PointLight* pLight = static_cast<const PointLight*>(l);
                    m.setVec3(("pointLights[" + noLight + "].position").c_str(), getSceneTransform() * pLight->getPosition());
                    m.setColor(("pointLights[" + noLight + "].ambientColor").c_str(), pLight->getAmbientColor());
                    m.setColor(("pointLights[" + noLight + "].diffuseColor").c_str(), pLight->getDiffuseColor());
                    m.setColor(("pointLights[" + noLight + "].specularColor").c_str(), pLight->getSpecularColor());
                    m.setFloat(("pointLights[" + noLight + "].constant").c_str(), pLight->getConstantAttenuationCoefficient());
                    m.setFloat(("pointLights[" + noLight + "].linear").c_str(), pLight->getLinearAttenuationCoefficient());
                    m.setFloat(("pointLights[" + noLight + "].quadratic").c_str(), pLight->getQuadraticAttenuationCoefficient());
                    ++pointCount;
                }
                else if (l->getType() == LightType::Directional)
                {
                    std::string noLight = std::to_string(dirCount);
                    const DirectionalLight* dLight = static_cast<const DirectionalLight*>(l);
                    m.setVec3(("directionalLights[" + noLight + "].direction").c_str(), getSceneTransform() * dLight->getDirection());
                    m.setColor(("directionalLights[" + noLight + "].ambientColor").c_str(), dLight->getAmbientColor());
                    m.setColor(("directionalLights[" + noLight + "].diffuseColor").c_str(), dLight->getDiffuseColor());
                    m.setColor(("directionalLights[" + noLight + "].specularColor").c_str(), dLight->getSpecularColor());
                    ++dirCount;
                }
                else if (l->getType() == LightType::Spot)
                {
                    std::string noLight = std::to_string(spotCount);
                    const SpotLight* sLight = static_cast<const SpotLight*>(l);
                    m.setVec3(("spotLights[" + noLight + "].position").c_str(), getSceneTransform() * sLight->getPosition());
                    m.setVec3(("spotLights[" + noLight + "].direction").c_str(), getSceneTransform() * sLight->getDirection());
                    m.setColor(("spotLights[" + noLight + "].ambientColor").c_str(), sLight->getAmbientColor());
                    m.setColor(("spotLights[" + noLight + "].diffuseColor").c_str(), sLight->getDiffuseColor());
                    m.setColor(("spotLights[" + noLight + "].specularColor").c_str(), sLight->getSpecularColor());
                    m.setFloat(("spotLights[" + noLight + "].cosAngle").c_str(), sLight->getCosAngle());
                    ++spotCount;
                }
            }

            m.setInt("currentPointLights", pointCount);
            m.setInt("currentDirLights", dirCount);
            m.setInt("currentSpotLights", spotCount);
        }
    }
}

void Scene::bindNormals(const Material& m) const
{
	if (m.isInitialized())
	{
		m.setMat4("gProjectionMatrix", m_camera.getPerspective());
		m.setMat4("gViewMatrix", m_camera.getView());
	}
}

void Scene::render() const
{
	if (m_showLights)
	{
		for (LightObject* obj : m_lights)
		{
			obj->render();
		}
	}

    for (Object3D* obj : m_objects)
    {
        obj->render();
    }

    for (BaseCurve* curve : m_curves)
    {
        curve->render();
    }
}

void Scene::renderNormals() const
{
	for (Object3D* obj : m_objects)
	{
		obj->renderNormals();
	}
}