#include "ShaderHelper.h"

#include "Shaders.h"
#include "ShaderManager.h"
#include "../Utilities/StringUtilities.h"

#include <fstream>
#include <string>

VertexShader* ShaderHelper::LoadBaseVertexShader()
{
	return ShaderManager::GetInstance()->LoadVertexShader("", "BaseVertexShader.vs");
}

FragmentShader* ShaderHelper::LoadBaseNoLitFragmentShader()
{
	return ShaderManager::GetInstance()->LoadFragmentShader("", "BaseColorNoLitFragmentShader.fs");
}

VertexShader* ShaderHelper::LoadBaseCurveVertexShader()
{
	return ShaderManager::GetInstance()->LoadVertexShader("", "BaseCurveVertexShader.vs");
}

FragmentShader* ShaderHelper::LoadBaseCurveFragmentShader()
{
	return ShaderManager::GetInstance()->LoadFragmentShader("", "BaseCurveFragmentShader.fs");
}

VertexShader* ShaderHelper::LoadEngineNormalVertexShader()
{
	return ShaderManager::GetInstance()->LoadVertexShader("", "EngineNormalVertexShader");
}

FragmentShader* ShaderHelper::LoadEngineNormalFragmentShader()
{
	return ShaderManager::GetInstance()->LoadFragmentShader("", "EngineNormalFragmentShader");
}

VertexShader* ShaderHelper::LoadVertexShader(const std::string& shaderName)
{
    if (StringUtilities::EndsWith(shaderName, "BaseVertexShader.vs"))
    {
        std::string code = "#version 410 \n \
        uniform mat4 gProjectionMatrix; \
        uniform mat4 gViewMatrix; \
        uniform mat4 gModelMatrix; \
        uniform vec4 uColor; \
        in vec3 aPosition; \
        in vec3 aNormal; \
        in vec2 aTexCoord; \
        struct FS_In { vec3 Color; vec2 TexCoord; vec3 Normal; vec3 WorldPosition; };\
        out FS_In fsIn; \
        void main() { \
            fsIn.WorldPosition = (gModelMatrix * vec4(aPosition, 1.0f)).xyz; \
            gl_Position = gProjectionMatrix * gViewMatrix * vec4(fsIn.WorldPosition, 1.0f); \
            fsIn.TexCoord = aTexCoord; \
            fsIn.Color = uColor.rgb; \
            fsIn.Normal = mat3(transpose(inverse(gModelMatrix))) * aNormal; \
        }";
        return new VertexShader("BaseVertexShader.vs", code);
    }
    else if (StringUtilities::EndsWith(shaderName, "BaseCurveVertexShader.vs"))
    {
        std::string code = "#version 410 \n \
            uniform mat4 gProjectionMatrix; \
            uniform mat4 gViewMatrix; \
            uniform vec4 gColor; \
            in vec3 aPosition; \
            out vec3 color; \
            void main() { \
                gl_Position = gProjectionMatrix * gViewMatrix * vec4(aPosition, 1.0f); \
                color = gColor.rgb; \
            }";
        return new VertexShader("BaseCurveVertexShader.vs", code);
    }
	else if (StringUtilities::Equals(shaderName, "EngineNormalVertexShader"))
	{
		std::string code = "#version 410 \n \
            uniform mat4 gProjectionMatrix; \
            uniform mat4 gViewMatrix; \
            uniform mat4 gModelMatrix; \
            in vec3 aPosition; \
            out vec3 color; \
            void main() { \
                gl_Position = gProjectionMatrix * gViewMatrix * gModelMatrix * vec4(aPosition, 1.0f); \
                color = vec3(1,1,1); \
            }";
		return new VertexShader("EngineNormalVertexShader", code);
	}
    return nullptr;
}

FragmentShader* ShaderHelper::LoadFragmentShader(const std::string& shaderName)
{
    if (StringUtilities::EndsWith(shaderName, "BaseColorLitFragmentShader.fs"))
    {
        std::string code = "#version 410 \n \
        #define MAX_POINT_LIGHT 10 \n \
        #define MAX_DIR_LIGHT 5 \n \
        #define MAX_SPOT_LIGHT 5 \n \
        struct FS_In { vec3 Color; vec2 TexCoord; vec3 Normal; vec3 WorldPosition; }; \n \
        struct Material { vec3 ambientColor; vec3 diffuseColor; vec3 specularColor; float shininess; }; \n \
        struct DirLight { vec3 direction; vec3 ambientColor; vec3 diffuseColor; vec3 specularColor; }; \n \
        struct PointLight { vec3 position; vec3 ambientColor; vec3 diffuseColor; vec3 specularColor; float constant; float linear; float quadratic; }; \n \
        struct SpotLight { vec3 position; vec3 direction; vec3 ambientColor; vec3 diffuseColor; vec3 specularColor; float cosAngle; }; \n \
        in FS_In fsIn; \n \
        uniform Material material; \n \
        uniform vec3 ambientColor; \n \
        uniform vec3 ambientPower; \n \
        uniform vec3 cameraPosition; \n \
        uniform int currentPointLights; \n \
        uniform int currentDirLights; \n \
        uniform int currentSpotLights; \n \
        uniform PointLight pointLights[MAX_POINT_LIGHT]; \n \
        uniform DirLight directionalLights[MAX_DIR_LIGHT]; \n \
        uniform SpotLight spotLights[MAX_SPOT_LIGHT]; \n \
        out vec3 outColor; \n \
        vec3 CalculateDirectionalLight(DirLight light, Material mat, vec3 objColor, vec3 normal, vec3 viewDir) { \n \
            vec3 lightDir = normalize(-light.direction); \n \
            float diff = max(dot(normal, lightDir), 0.0); \n \
            float spec = 0.0; \n \
            if (diff > 0.0) { \n \
                vec3 reflectDir = reflect(-lightDir, normal); \n \
                spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess); \n\
            } \n \
            vec3 ambient = light.ambientColor  * mat.ambientColor; \n \
            vec3 diffuse = light.diffuseColor  * diff * mat.diffuseColor; \n \
            vec3 specular = light.specularColor * spec * mat.specularColor; \n \
            return ambient + diffuse + specular; \n \
        } \n \
        vec3 CalculatePointLight(PointLight light, Material mat, vec3 objColor, vec3 normal, vec3 fragPos, vec3 viewDir) { \n \
            vec3 lightDir = normalize(light.position - fragPos); \n \
            float diff = max(dot(normal, lightDir), 0.0); \n \
            float spec = 0.0; \n \
			if (diff != 0.0) { \n \
			    vec3 reflectDir = reflect(-lightDir, normal); \n \
			    spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess); \n \
		    } \n \
            vec3 ambient = light.ambientColor  * mat.ambientColor; \n \
            vec3 diffuse = light.diffuseColor  * diff * mat.diffuseColor; \n \
            vec3 specular = light.specularColor * spec * mat.specularColor; \n \
            float distance = length(light.position - fragPos); \n \
            float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance))); \n \
            ambient *= attenuation; \n \
            diffuse *= attenuation; \n \
            specular *= attenuation; \n \
            return ambient + diffuse + specular; \n \
        } \
        vec3 CalculateSpotLight(SpotLight light, Material mat, vec3 objColor, vec3 normal, vec3 fragPos, vec3 viewDir) { \n \
            vec3 lightDir = normalize(light.position - fragPos); \n \
            float cosTheta = max(dot(lightDir, normalize(-light.direction)), 0); \n \
            vec3 diffuse = vec3(0, 0, 0); \n \
            vec3 specular = vec3(0, 0, 0); \n \
            vec3 ambient = vec3(0, 0, 0); \n \
            if (cosTheta > light.cosAngle) { \n \
                float NdotD = dot(light.direction, normal); \n \
                if (NdotD < 0.0) { \n \
                    float diff = 0.0; \n \
                    float spec = 0.0; \n \
                    diff = max(dot(normal, lightDir), 0.0); \n \
                    if (diff != 0.0) { \n \
                        vec3 reflectDir = reflect(-lightDir, normal); \n \
                        spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess); \n \
                    } \n \
                    ambient = light.ambientColor * mat.ambientColor; \n \
                    diffuse = light.diffuseColor  * diff * mat.diffuseColor; \n \
                    specular = light.specularColor * spec * mat.specularColor; \n \
                    float intensity = pow(cosTheta, 5); \n \
                    ambient *= intensity; \n \
                    diffuse *= intensity; \n \
                    specular *= intensity; \n \
                } \n \
            } \n \
            return ambient + diffuse + specular; \n \
        } \n \
        void main() { \n \
            vec3 aColor = ambientColor * material.ambientColor * ambientPower; \n \
            vec3 viewDir = normalize(cameraPosition - fsIn.WorldPosition); \n \
            vec3 normal = normalize(fsIn.Normal); \n \
            vec3 colorResult = vec3(0, 0, 0); \n \
            if (!gl_FrontFacing) { normal = -normal; } \n \
            for (int i = 0; i < currentDirLights; i++) { \n \
                colorResult += CalculateDirectionalLight(directionalLights[i], material, fsIn.Color, normal, viewDir); \n \
            } \n \
            for (int i = 0; i < currentPointLights; i++) { \n \
                colorResult += CalculatePointLight(pointLights[i], material, fsIn.Color, normal, fsIn.WorldPosition, viewDir); \n \
            } \n \
            for (int i = 0; i < currentSpotLights; i++) { \n \
                colorResult += CalculateSpotLight(spotLights[i], material, fsIn.Color, normal, fsIn.WorldPosition, viewDir); \n \
            } \n \
            outColor = (aColor + colorResult);\n \
        }\n";
        return new FragmentShader("BaseColorLitFragmentShader.fs", code);
    }
    else if (StringUtilities::EndsWith(shaderName, "BaseColorNoLitFragmentShader.fs"))
    {
        std::string code = "#version 410 \n \
        struct FS_In { vec3 Color; vec2 TexCoord; vec3 Normal; vec3 WorldPosition; }; \
        in FS_In fsIn; \
        out vec3 outColor; \
        void main() { outColor = fsIn.Color; }";
        return new FragmentShader("BaseColorNoLitFragmentShader.fs", code);
    }
    else if (StringUtilities::EndsWith(shaderName, "BaseCurveFragmentShader.fs"))
    {
        std::string code = "#version 410 \n \
        in vec3 color; \
        out vec3 outColor; \
        void main() { outColor = color; }";
        return new FragmentShader("BaseCurveFragmentShader.fs", code);
    }
	else if (StringUtilities::EndsWith(shaderName, "EngineNormalFragmentShader"))
	{
		std::string code = "#version 410 \n \
        in vec3 color; \
        out vec3 outColor; \
        void main() { outColor = color; }";
		return new FragmentShader("EngineNormalFragmentShader", code);
	}
    return nullptr;
}