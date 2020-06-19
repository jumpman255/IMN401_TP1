#include "SceneLoader.h"

#include "Object3D.h"
#include "Scene.h"

#include "../Curves/Curve.h"
#include "../Geometry/Geometry.h"
#include "../Geometry/GeometryHelper.h"
#include "../Geometry/GeometryManager.h"
#include "../Geometry/OBJImporter.h"
#include "../Light/Lights.h"
#include "../Material/Material.h"
#include "../Material/Shaders.h"
#include "../Material/ShaderHelper.h"
#include "../Material/ShaderManager.h"
#include "../Texture/Texture.h"
#include "../Texture/TextureManager.h"
#include "../Utilities/Logger.h"
#include "../Utilities/StringUtilities.h"
#include "../Utilities/Transforms.h"
#include "../Utilities/Types.h"

#include <fstream>
#include <iostream>

Scene* SceneLoader::LoadScene(const std::string& path, const std::string& sceneFile)
{
    Scene* loadedScene = nullptr;

    tinyxml2::XMLDocument document;
    if (document.LoadFile((path + sceneFile).c_str()) != tinyxml2::XML_SUCCESS)
    {
        Log() << "Il y a une erreur dans le format du fichier de la scene. Impossible de changer la scene." << std::endl;
        return nullptr;
    }

    const tinyxml2::XMLElement* sceneElement = document.RootElement();
    if (sceneElement == nullptr)
    {
        Log() << "Scene " << path + sceneFile << " introuvable..." << std::endl;
        return nullptr;
    }

    if (StringUtilities::Equals(sceneElement->Name(), "scene"))
    {
        loadedScene = new Scene();

        const tinyxml2::XMLElement* propertiesElement = sceneElement->FirstChildElement("properties");
        if (propertiesElement != nullptr)
        {
            const tinyxml2::XMLElement* cameraElement = propertiesElement->FirstChildElement("camera");
            if (cameraElement != nullptr)
            {
                loadedScene->setCamera(LoadCamera(cameraElement));
            }

            const tinyxml2::XMLElement* ambientColorElement = propertiesElement->FirstChildElement("ambientColor");
            const tinyxml2::XMLElement* ambientPowerElement = propertiesElement->FirstChildElement("ambientPower");

            loadedScene->setAmbientColor(LoadColorRGB(ambientColorElement, ColorRGB::Black()));
            loadedScene->setAmbientPower(LoadVector3<Real>(ambientPowerElement, 1.0f));

			const tinyxml2::XMLElement* transformElement = propertiesElement->FirstChildElement("transforms");
			Transform sceneTransform = LoadTransform(transformElement);
			loadedScene->setSceneTransform(sceneTransform);

			const tinyxml2::XMLElement* materialElement = propertiesElement->FirstChildElement("material");
			if (materialElement != nullptr)
			{
				Material* sceneMaterial = LoadMaterial(path, materialElement);
				loadedScene->setSceneMaterial(sceneMaterial);
			}
        }

        const tinyxml2::XMLElement* lightsElement = sceneElement->FirstChildElement("lights");
        if (lightsElement != nullptr)
        {
            const tinyxml2::XMLElement* lightElement = lightsElement->FirstChildElement("light");
            while (lightElement != nullptr)
            {
                loadedScene->addLight(LoadLight(path, lightElement));
                lightElement = lightElement->NextSiblingElement("light");
            }
        }

        const tinyxml2::XMLElement* objectsElement = sceneElement->FirstChildElement("objects");
        if (objectsElement != nullptr)
        {
            const tinyxml2::XMLElement* objElement = objectsElement->FirstChildElement("object");
            while (objElement != nullptr)
            {
                loadedScene->addObject(LoadObject(path, objElement));
                objElement = objElement->NextSiblingElement("object");
            }
        }
        const tinyxml2::XMLElement* curvesElement = sceneElement->FirstChildElement("curves");
        if (curvesElement != nullptr)
        {
            const tinyxml2::XMLElement* curveElement = curvesElement->FirstChildElement("curve");
            while (curveElement != nullptr)
            {
                loadedScene->addCurve(LoadCurve(path, curveElement));
                curveElement = curveElement->NextSiblingElement("curve");
            }
        }
    }
    else
    {
        Log() << "--Erreur : Noeud 'scene' attendu; Noeud '" << sceneElement->Name() << "' recu!" << std::endl;
    }
    return loadedScene;
}

Camera SceneLoader::LoadCamera(const tinyxml2::XMLElement* cameraElement)
{
	Camera camera;
	Point3<Metre> position = Point3<Metre>(Metre(), Metre(), Metre(-5));

	const tinyxml2::XMLElement* nearElement = cameraElement->FirstChildElement("near");
	const tinyxml2::XMLElement* farElement = cameraElement->FirstChildElement("far");
	const tinyxml2::XMLElement* fovElement = cameraElement->FirstChildElement("fieldOfView");
	const tinyxml2::XMLElement* positionElement = cameraElement->FirstChildElement("position");
	const tinyxml2::XMLElement* lookAtElement = cameraElement->FirstChildElement("lookAt");
	const tinyxml2::XMLElement* upElement = cameraElement->FirstChildElement("up");

	camera.near() = LoadValue<Metre>(nearElement, 0.1f);
	camera.far() = LoadValue<Metre>(farElement, 1000);
	camera.fieldOfView() = LoadValue<Degree>(fovElement, 45);
	if (upElement != nullptr)
	{
		Camera::SetWorldUp(LoadVector3<Real>(upElement));
	}

	if (positionElement != nullptr)
	{
		position = LoadPoint3<Metre>(positionElement);
	}
	Point3<Metre> lookAt = LoadPoint3<Metre>(lookAtElement);

	camera.setCoordinate(lookAt, position);

	return camera;
}

Color SceneLoader::LoadColor(const tinyxml2::XMLElement* element, const Color& defaultColor = Color::Black())
{
    if (element != nullptr)
    {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        if (element->Attribute("red")) 
        {
            r = (uint8)element->UnsignedAttribute("red", 0) / 255.0f;
        }
        else
        {
            r = element->FloatAttribute("r", 0.0f);
        }

        if (element->Attribute("green"))
        {
            g = (uint8)element->UnsignedAttribute("green", 0) / 255.0f;
        }
        else
        {
            g = element->FloatAttribute("g", 0.0f);
        }

        if (element->Attribute("blue"))
        {
            b = (uint8)element->UnsignedAttribute("blue", 0) / 255.0f;
        }
        else
        {
            b = element->FloatAttribute("b", 0.0f);
        }

        return Color(r, g, b, element->FloatAttribute("a", 1.0f));
    }
    return defaultColor;
}

ColorRGB SceneLoader::LoadColorRGB(const tinyxml2::XMLElement* element, const ColorRGB& defaultColor = ColorRGB::Black())
{
    if (element != nullptr)
    {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        if (element->Attribute("red"))
        {
            r = (uint8)element->UnsignedAttribute("red", 0) / 255.0f;
        }
        else
        {
            r = element->FloatAttribute("r", 0.0f);
        }

        if (element->Attribute("green"))
        {
            g = (uint8)element->UnsignedAttribute("green", 0) / 255.0f;
        }
        else
        {
            g = element->FloatAttribute("g", 0.0f);
        }

        if (element->Attribute("blue"))
        {
            b = (uint8)element->UnsignedAttribute("blue", 0) / 255.0f;
        }
        else
        {
            b = element->FloatAttribute("b", 0.0f);
        }

        return ColorRGB(r, g, b);
    }
    return defaultColor;
}

LightObject* SceneLoader::LoadLight(const std::string& path, const tinyxml2::XMLElement* element)
{
    if (StringUtilities::Equals(element->Attribute("type"), "point"))
    {
        Log() << "Chargement d'une lumiere PointLight..." << std::endl;
        const tinyxml2::XMLElement* positionElement = element->FirstChildElement("position");
        const tinyxml2::XMLElement* ambientColorElement = element->FirstChildElement("ambient");
        const tinyxml2::XMLElement* diffuseColorElement = element->FirstChildElement("diffuse");
        const tinyxml2::XMLElement* specularColorElement = element->FirstChildElement("specular");
        const tinyxml2::XMLElement* constantElement = element->FirstChildElement("constant");
        const tinyxml2::XMLElement* linearElement = element->FirstChildElement("linear");
        const tinyxml2::XMLElement* quadraticElement = element->FirstChildElement("quadratic");
        ColorRGB aColor = LoadColorRGB(ambientColorElement);
        ColorRGB dColor = LoadColorRGB(diffuseColorElement);
        ColorRGB sColor = LoadColorRGB(specularColorElement);
        Point3<Metre> position = LoadPoint3<Metre>(positionElement);
        float constant  = LoadValue<float>(constantElement);
        float linear    = LoadValue<float>(linearElement);
        float quadratic = LoadValue<float>(quadraticElement);
        return new PointLight(position, aColor, dColor, sColor, constant, linear, quadratic);
    }
    else if (StringUtilities::Equals(element->Attribute("type"), "directional"))
    {
        Log() << "Chargement d'une lumiere DirectionalLight..." << std::endl;
        const tinyxml2::XMLElement* directionElement = element->FirstChildElement("direction");
        const tinyxml2::XMLElement* ambientColorElement = element->FirstChildElement("ambient");
        const tinyxml2::XMLElement* diffuseColorElement = element->FirstChildElement("diffuse");
        const tinyxml2::XMLElement* specularColorElement = element->FirstChildElement("specular");
        Vector3<Real> direction = LoadVector3<Real>(directionElement);
        ColorRGB aColor = LoadColorRGB(ambientColorElement);
        ColorRGB dColor = LoadColorRGB(diffuseColorElement);
        ColorRGB sColor = LoadColorRGB(specularColorElement);        
        return new DirectionalLight(direction, aColor, dColor, sColor);
    }
    else if (StringUtilities::Equals(element->Attribute("type"), "spot"))
    {
        Log() << "Chargement d'une lumiere SpotLight..." << std::endl;
        const tinyxml2::XMLElement* positionElement = element->FirstChildElement("position");
        const tinyxml2::XMLElement* directionElement = element->FirstChildElement("direction");
        const tinyxml2::XMLElement* ambientColorElement = element->FirstChildElement("ambient");
        const tinyxml2::XMLElement* diffuseColorElement = element->FirstChildElement("diffuse");
        const tinyxml2::XMLElement* specularColorElement = element->FirstChildElement("specular");
        const tinyxml2::XMLElement* angleElement = element->FirstChildElement("angle");
        ColorRGB aColor = LoadColorRGB(ambientColorElement);
        ColorRGB dColor = LoadColorRGB(diffuseColorElement);
        ColorRGB sColor = LoadColorRGB(specularColorElement);
        Degree angle = LoadValue<Degree>(angleElement, 10.0f);
        Point3<Metre> position = LoadPoint3<Metre>(positionElement);
        Vector3<Real> direction = LoadVector3<Real>(directionElement);        
        return new SpotLight(position, aColor, dColor, sColor, direction, angle);
    }
    else
    {
        Log() << "Chargement d'une lumiere inconnu (" << element->Attribute("type") << "). Lumiere non chargee..." << std::endl;
    }
    return nullptr;
}

Object3D* SceneLoader::LoadObject(const std::string& path, const tinyxml2::XMLElement* element)
{
    const char* objName = element->Attribute("name");
    if (objName == nullptr)
        objName = "Unknown";

    Log() << "Chargement de l'objet " << objName << "..." << std::endl;

    Logger::IncIndent();

    const tinyxml2::XMLElement* materialElement = element->FirstChildElement("material");
    const tinyxml2::XMLElement* transformElement = element->FirstChildElement("transform");
    const tinyxml2::XMLElement* geometryElement = element->FirstChildElement("geometry");
    const tinyxml2::XMLElement* childrenElement = element->FirstChildElement("children");

    Material* objMaterial = nullptr;
    Geometry* objGeom = nullptr;
    Transform objTransform;

    if (materialElement != nullptr)
    {
        objMaterial = LoadMaterial(path, materialElement);
    }

    if (geometryElement != nullptr)
    {
        objGeom = LoadGeometry(path, geometryElement);
    }

	objTransform = LoadTransform(transformElement);
    
    Object3D* obj = new Object3D(objName, objMaterial, objGeom);
    obj->setTransform(objTransform);

    if (childrenElement != nullptr)
    {
        const tinyxml2::XMLElement* objElement = childrenElement->FirstChildElement("object");
        while (objElement != nullptr)
        {
            obj->addChildren(LoadObject(path, objElement));
            objElement = objElement->NextSiblingElement("object");
        }
    }
    
    Logger::DecIndent();    
    return obj;
}

Transform SceneLoader::LoadTransform(const tinyxml2::XMLElement* element)
{
	Transform objTransform;
	if (element != nullptr)
	{
		const tinyxml2::XMLElement* child = element->FirstChildElement();
		while (child != nullptr)
		{
			// Les angles doivent être spécifiés en Degré et les distances en Mètre.
			if (StringUtilities::Equals(child->Name(), "translation"))
			{
				Transform t = Transform::MakeTranslation(LoadVector3<Metre>(child));
				objTransform = t * objTransform;
			}
			else if (StringUtilities::Equals(child->Name(), "rotation"))
			{
				Transform t = Transform::MakeRotation(Degree(child->FloatAttribute("angle", 0.0f)), Vector3<Real>(child->FloatAttribute("axeX", 0.0f), child->FloatAttribute("axeY", 0.0f), child->FloatAttribute("axeZ", 0.0f)));
				objTransform = t * objTransform;
			}
			else if (StringUtilities::Equals(child->Name(), "rotationX"))
			{
				Transform t = Transform::MakeRotationX(Degree(child->FloatAttribute("angle", 0.0f)));
				objTransform = t * objTransform;
			}
			else if (StringUtilities::Equals(child->Name(), "rotationY"))
			{
				Transform t = Transform::MakeRotationY(Degree(child->FloatAttribute("angle", 0.0f)));
				objTransform = t * objTransform;
			}
			else if (StringUtilities::Equals(child->Name(), "rotationZ"))
			{
				Transform t = Transform::MakeRotationZ(Degree(child->FloatAttribute("angle", 0.0f)));
				objTransform = t * objTransform;
			}
			else if (StringUtilities::Equals(child->Name(), "scale"))
			{
				Transform t = Transform::MakeScale(LoadVector3<Real>(child, 1.0f));
				objTransform = t * objTransform;
			}
			else if (StringUtilities::Equals(child->Name(), "shearX"))
			{
				Transform t = Transform::MakeShearX(Real(child->FloatAttribute("value", 0.0f)));
				objTransform = t * objTransform;
			}
			else if (StringUtilities::Equals(child->Name(), "shearY"))
			{
				Transform t = Transform::MakeShearY(Real(child->FloatAttribute("value", 0.0f)));
				objTransform = t * objTransform;
			}
			else if (StringUtilities::Equals(child->Name(), "shearZ"))
			{
				Transform t = Transform::MakeShearZ(Real(child->FloatAttribute("value", 0.0f)));
				objTransform = t * objTransform;
			}
			else
			{
				Log() << "--Attention : Transformation inconnue (" << child->Name() << ")..." << std::endl;
			}
			child = child->NextSiblingElement();
		}
	}
	return objTransform;
}

BaseCurve* SceneLoader::LoadCurve(const std::string& path, const tinyxml2::XMLElement* element)
{
    const char* curveName = element->Attribute("name");
    if (curveName == nullptr)
        curveName = "Unknown";

    Log() << "Chargement de la courbe " << curveName << "..." << std::endl;

    Logger::IncIndent();

    const char* curveType = element->Attribute("type");
    if (curveType == nullptr)
    {
        Log() << "--Erreur : La courbe doit avoir un type." << std::endl;
        Logger::DecIndent();
        return nullptr;
    }

    const tinyxml2::XMLElement* colorElement = element->FirstChildElement("color");
    const tinyxml2::XMLElement* precisionElement = element->FirstChildElement("precision");
    const tinyxml2::XMLElement* controlPointsElement = element->FirstChildElement("controlPoints");

    Color c = LoadColor(colorElement, Color::White());
    int precision = LoadValue<int>(precisionElement, 10);
    BaseCurve* curve = nullptr;
    if (controlPointsElement != nullptr)
    {        
        if (StringUtilities::Equals(curveType, "bezier"))
        {
            curve = new BezierCurve(curveName);
        }
        else if (StringUtilities::Equals(curveType, "hermite"))
        {
            curve = new HermiteCurve(curveName);
        }
        else if (StringUtilities::Equals(curveType, "catmullrom"))
        {
            curve = new CatmullRomCurve(curveName);
        }
        else if (StringUtilities::Equals(curveType, "bspline"))
        {
            curve = new BSplineCurve(curveName);
        }
        else
        {
            Log() << "--Erreur : Le type " << curveType << " de la courbe " << curveName << " est inconnu." << std::endl;
            Logger::DecIndent();
            return nullptr;
        }

        curve->setColor(c);
        curve->setCurvePrecision(precision);

        const tinyxml2::XMLElement* child = controlPointsElement->FirstChildElement("point");
        while (child != nullptr)
        {
            Vector3<Metre> point = LoadVector3<Metre>(child);
            curve->addControlPoint(point);
            child = child->NextSiblingElement("point");
        }

        if (curve->controlPointsCount() < 4)
        {
            Log() << "--Erreur : Une courbe doit specifier au moins quatre points de controle." << std::endl;
            delete curve;
            curve = nullptr;
        }
        else
        {
            curve->updateVertices();
            curve->updateVAO();
        }
    }
    else
    {
        Log() << "--Erreur : Une courbe doit specifier au moins quatre points de controle." << std::endl;
    }

    Logger::DecIndent();
    return curve;
}

Material* SceneLoader::LoadMaterial(const std::string& path, const tinyxml2::XMLElement* element)
{
    const tinyxml2::XMLElement* vShaderElement = element->FirstChildElement("vertexShader");
    VertexShader* vShader = nullptr;
    if (vShaderElement != nullptr)
    {
        vShader = ShaderManager::GetInstance()->LoadVertexShader(path, vShaderElement->Attribute("name"));
        if (vShader == nullptr)
        {
            Log() << "--Erreur : VertexShader (" << vShaderElement->Attribute("name") << ") introuvable." << std::endl;
            Log() << "           Utilisation du VertexShader par defaut." << std::endl;
            vShader = ShaderHelper::LoadBaseVertexShader();
        }
    }
    else
    {
        vShader = ShaderHelper::LoadBaseVertexShader();
    }

    const tinyxml2::XMLElement* fShaderElement = element->FirstChildElement("fragmentShader");
    FragmentShader* fShader = nullptr;
    if (fShaderElement != nullptr)
    {
        fShader = ShaderManager::GetInstance()->LoadFragmentShader(path, fShaderElement->Attribute("name"));
        if (fShader == nullptr)
        {
            Log() << "--Erreur : FragmentShader (" << fShaderElement->Attribute("name") << ") introuvable." << std::endl;
            Log() << "           Utilisation du FragmentShader par defaut." << std::endl;
            fShader = ShaderHelper::LoadBaseNoLitFragmentShader();
        }
    }
    else
    {
        fShader = ShaderHelper::LoadBaseNoLitFragmentShader();
    }

    Material* objMaterial = new Material(vShader, fShader);
    if (objMaterial->isInitialized())
    {
        // On charge l'ensemble des textures et des uniforms pour ce matériel
        if (vShaderElement != nullptr)
        {
            LoadUniformsForMaterial(path, vShaderElement, *objMaterial);
        }

        if (fShaderElement != nullptr)
        {
            LoadUniformsForMaterial(path, fShaderElement, *objMaterial);
        }
        objMaterial->logBindingDetails();
    }

    return objMaterial;
}

void SceneLoader::LoadUniformsForMaterial(const std::string& path, const tinyxml2::XMLElement* shaderElement, Material& material)
{
    // On commence par ajouter l'ensemble des textures
    const tinyxml2::XMLElement* textureInfo = shaderElement->FirstChildElement("texture");
    while (textureInfo != nullptr)
    {
        Texture2D* objTexture = TextureManager::GetInstance()->loadTexture(path + textureInfo->Attribute("value"));
        if (objTexture != nullptr)
        {
            const char* samplerName = textureInfo->Attribute("name");
            material.addTextureBinding(samplerName, objTexture);
        }
        else
        {
            Log() << "--Erreur : Texture " << textureInfo->Attribute("value") << " introuvable." << std::endl;
        }
        textureInfo = textureInfo->NextSiblingElement("texture");
    }

    // On ajoute ensuite chacun des uniforms spécifiés
    // Les types supportés sont :
    //    vec3
    //    vec4
    //    float
    //    int
    //    color
    const tinyxml2::XMLElement* uniformInfo = shaderElement->FirstChildElement("uniform");
    while (uniformInfo != nullptr)
    {
        const char* type = uniformInfo->Attribute("type");
        if (StringUtilities::Equals(type, "vec3"))
        {
            material.addVec3Binding(uniformInfo->Attribute("name"), LoadVector3<Real>(uniformInfo));
        }
        else if (StringUtilities::Equals(type, "vec4"))
        {
            material.addVec4Binding(uniformInfo->Attribute("name"), LoadVector4<Real>(uniformInfo));
        }
        else if (StringUtilities::Equals(type, "float"))
        {
            material.addFloatBinding(uniformInfo->Attribute("name"), LoadValue<Real>(uniformInfo));
        }
        else if (StringUtilities::Equals(type, "int"))
        {
            material.addIntBinding(uniformInfo->Attribute("name"), LoadValue<int>(uniformInfo));
        }
        else if (StringUtilities::Equals(type, "color"))
        {
            if (uniformInfo->Attribute("a") == nullptr)
            {
                material.addColorBinding(uniformInfo->Attribute("name"), LoadColorRGB(uniformInfo));
            }
            else
            {
                material.addColorBinding(uniformInfo->Attribute("name"), LoadColor(uniformInfo));
            }
        }

        uniformInfo = uniformInfo->NextSiblingElement("uniform");
    }
}

Geometry* SceneLoader::LoadGeometry(const std::string& path, const tinyxml2::XMLElement* element)
{
    Geometry* objGeom = nullptr;
    if (StringUtilities::Equals(element->Attribute("type"), "forme"))
    {
        const tinyxml2::XMLElement* formeElement = element->FirstChildElement("forme");
        if (formeElement != nullptr)
        {
            float repeatX = 1.0f;
            float repeatY = 1.0f;
            const tinyxml2::XMLElement* geomInfo = formeElement->FirstChildElement("color");
            bool colorSpecified = geomInfo != nullptr;

            if (StringUtilities::Equals(formeElement->Attribute("type"), "triangle"))
            {
                objGeom = GeometryHelper::CreateTriangle();
            }
            else if (StringUtilities::Equals(formeElement->Attribute("type"), "cube"))
            {
                float val = -1.0f;
                Metre width;
                Metre height;
                Metre depth;
                if (formeElement->QueryFloatAttribute("size", &val) == tinyxml2::XML_NO_ATTRIBUTE)
                {
                    width = Metre(formeElement->FloatAttribute("width", 1.0f));
                    height = Metre(formeElement->FloatAttribute("height", 1.0f));
                    depth = Metre(formeElement->FloatAttribute("depth", 1.0f));
                }
                else
                {
                    width = height = depth = Metre(val);
                }

                if (colorSpecified)
                {
                    objGeom = GeometryHelper::CreateBox(width, height, depth, LoadColor(geomInfo));
                }
                else
                {
                    objGeom = GeometryHelper::CreateBox(width, height, depth, repeatX, repeatY);
                }
            }
            else if (StringUtilities::Equals(formeElement->Attribute("type"), "sphere"))
            {
                Metre radius = Metre(formeElement->FloatAttribute("radius", 1.0f));
                uint32 slices = formeElement->UnsignedAttribute("slices", 10);
                uint32 stacks = formeElement->UnsignedAttribute("stacks", 10);

                if (colorSpecified)
                {
                    objGeom = GeometryHelper::CreateSphere(radius, slices, stacks, LoadColor(geomInfo));
                }
                else
                {
                    objGeom = GeometryHelper::CreateSphere(radius, slices, stacks);
                }
            }
            else if (StringUtilities::Equals(formeElement->Attribute("type"), "cylinder"))
            {
                Metre topRadius = Metre(formeElement->FloatAttribute("topRadius", 1.0f));
                Metre bottomRadius = Metre(formeElement->FloatAttribute("bottomRadius", 1.0f));
                Metre height = Metre(formeElement->FloatAttribute("height", 1.0f));
                uint32 slices = formeElement->UnsignedAttribute("slices", 10);
                uint32 stacks = formeElement->UnsignedAttribute("stacks", 10);

                if (colorSpecified)
                {
                    objGeom = GeometryHelper::CreateCylinder(topRadius, bottomRadius, height, slices, stacks, LoadColor(geomInfo));
                }
                else
                {
                    objGeom = GeometryHelper::CreateCylinder(topRadius, bottomRadius, height, slices, stacks);
                }
            }
            else if (StringUtilities::Equals(formeElement->Attribute("type"), "torus"))
            {
                Metre radius = Metre(formeElement->FloatAttribute("radius", 2.0f));
                Metre ringRadius = Metre(formeElement->FloatAttribute("ringRadius", 0.5f));
                uint32 sides = formeElement->UnsignedAttribute("sides", 10);
                uint32 rings = formeElement->UnsignedAttribute("rings", 10);

                objGeom = GeometryHelper::CreateTorus(radius, ringRadius, sides, rings, &LoadColor(geomInfo));
            }
            else if (StringUtilities::Equals(formeElement->Attribute("type"), "grid"))
            {
                Metre width = Metre(formeElement->FloatAttribute("width", 10.0f));
                Metre depth = Metre(formeElement->FloatAttribute("depth", 10.0f));

                uint32 m = formeElement->UnsignedAttribute("m", 10);
                uint32 n = formeElement->UnsignedAttribute("n", 10);

                repeatX = formeElement->FloatAttribute("uRepeat", 1.0f);
                repeatY = formeElement->FloatAttribute("vRepeat", 1.0f);

                if (colorSpecified)
                {
                    objGeom = GeometryHelper::CreateGrid(width, depth, m, n, LoadColor(geomInfo));
                }
                else
                {
                    objGeom = GeometryHelper::CreateGrid(width, depth, m, n, repeatX, repeatY);
                }
            }
			else if (StringUtilities::Equals(formeElement->Attribute("type"), "revolution"))
			{
				uint32 precision = formeElement->UnsignedAttribute("precision", 60);
				const tinyxml2::XMLElement* pointsElement = formeElement->FirstChildElement("points");
				if (pointsElement != nullptr)
				{
					std::vector<Point2<Metre>> vertices;
					const tinyxml2::XMLElement* pointInfo = pointsElement->FirstChildElement("point");
					while (pointInfo != nullptr)
					{
						Point2<Metre> p = LoadPoint2<Metre>(pointInfo);
						vertices.push_back(p);
						pointInfo = pointInfo->NextSiblingElement("point");
					}
					if (vertices.size() > 0)
					{
						objGeom = GeometryHelper::CreateRevolutionSurface(vertices, precision);
					}
					else
					{
						Log() << "--Erreur : L'objet de revolution n'a pas de points pour definir sa silhouette." << std::endl;
					}
				}
				else
				{
					Log() << "--Erreur : L'objet de revolution n'a pas de points pour definir sa silhouette." << std::endl;
				}
			}
            else
            {
                Log() << "--Erreur : Type de forme inconnu. " << formeElement->Attribute("type") << " recu." << std::endl;
            }
        }
        else
        {
            Log() << "--Erreur : Noeud 'forme' attendu." << std::endl;
        }
    }
    else if (StringUtilities::Equals(element->Attribute("type"), "fichier"))
    {
        const tinyxml2::XMLElement* formeElement = element->FirstChildElement("fichier");
        if (formeElement != nullptr)
        {
            objGeom = GeometryManager::GetInstance()->loadGeometry(formeElement->Attribute("name"));
        }
        else
        {
            Log() << "--Erreur : Noeud 'fichier' attendu pour une geometrie de type 'fichier'." << std::endl;
        }
    }
    else
    {
        Log() << "--Erreur : Type de geometrie invalide; Attendu 'forme' ou 'fichier'; Recu : " << element->Attribute("type") << "." << std::endl;
    }
    return objGeom;
}