#ifndef _SCENE_SCENELOADER_H_
#define _SCENE_SCENELOADER_H_

#include "../Utilities/Color.h"
#include "../Utilities/Point.h"
#include "../Utilities/Vectors.h"

#include <TinyXML/tinyxml2.h>

#include <string>

class BaseCurve;
class Camera;
class Geometry;
class LightObject;
class Material;
class Object3D;
class Scene;
class Transform;

class SceneLoader
{
private:
    static void LoadUniformsForMaterial(const std::string& path, const tinyxml2::XMLElement* element, Material& material);
    static Material* LoadMaterial(const std::string& path, const tinyxml2::XMLElement* element);
    static Geometry* LoadGeometry(const std::string& path, const tinyxml2::XMLElement* element);
    static Object3D* LoadObject(const std::string& path, const tinyxml2::XMLElement* element);
    static BaseCurve* LoadCurve(const std::string& path, const tinyxml2::XMLElement* element);
    static LightObject* LoadLight(const std::string& path, const tinyxml2::XMLElement* element);
    static Color LoadColor(const tinyxml2::XMLElement* element, const Color& defaultColor);
    static ColorRGB LoadColorRGB(const tinyxml2::XMLElement* element, const ColorRGB& defaultColor);
	static Transform LoadTransform(const tinyxml2::XMLElement* element);
	static Camera LoadCamera(const tinyxml2::XMLElement* element);
    
    template<typename U>
    static U LoadValue(const tinyxml2::XMLElement* element, float defaultValue)
    {
        if (element != nullptr)
        {
            return U(element->FloatAttribute("value", defaultValue));
        }
        return U(defaultValue);
    }

    template<typename U>
    static U LoadValue(const tinyxml2::XMLElement* element)
    {
        if (element != nullptr)
        {
            return U(element->FloatAttribute("value", 0.0f));
        }
        return U(0.0f);
    }

    template<>
    static int LoadValue<int>(const tinyxml2::XMLElement* element)
    {
        if (element != nullptr)
        {
            return element->IntAttribute("value", 0);
        }
        return 0;
    }

	template<typename U>
	static Point2<U> LoadPoint2(const tinyxml2::XMLElement* element, float defaultValue = 0.0f)
	{
		if (element != nullptr)
		{
			return Point2<U>(U(element->FloatAttribute("x", defaultValue)), U(element->FloatAttribute("y", defaultValue)));
		}
		return Point2<U>(U(defaultValue), U(defaultValue));
	}

    template<typename U>
    static Point3<U> LoadPoint3(const tinyxml2::XMLElement* element, float defaultValue = 0.0f)
    {
        if (element != nullptr)
        {
            return Point3<U>(U(element->FloatAttribute("x", defaultValue)), U(element->FloatAttribute("y", defaultValue)), U(element->FloatAttribute("z", defaultValue)));
        }
        return Point3<U>(U(defaultValue), U(defaultValue), U(defaultValue));
    }

    template<typename U>
    static Vector3<U> LoadVector3(const tinyxml2::XMLElement* element, float defaultValue = 0.0f)
    {
        if (element != nullptr)
        {
            return Vector3<U>(U(element->FloatAttribute("x", defaultValue)), U(element->FloatAttribute("y", defaultValue)), U(element->FloatAttribute("z", defaultValue)));
        }
        return Vector3<U>(U(defaultValue), U(defaultValue), U(defaultValue));
    }

    template<typename U>
    static Vector4<U> LoadVector4(const tinyxml2::XMLElement* element, float defaultValue = 0.0f)
    {
        if (element != nullptr)
        {
            return Vector4<U>(U(element->FloatAttribute("x", defaultValue)), U(element->FloatAttribute("y", defaultValue)), U(element->FloatAttribute("z", defaultValue)), U(element->FloatAttribute("w", defaultValue)));
        }
        return Vector4<U>(U(defaultValue), U(defaultValue), U(defaultValue), U(defaultValue));
    }

public:
    static Scene* LoadScene(const std::string& path, const std::string& sceneFile);
};

#endif
