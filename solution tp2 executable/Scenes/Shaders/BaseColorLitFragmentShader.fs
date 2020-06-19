#version 410

#define MAX_POINT_LIGHT 10
#define MAX_DIR_LIGHT 5
#define MAX_SPOT_LIGHT 5

struct FS_In 
{
	vec3 Color;
	vec2 TexCoord;
	vec3 Normal;
	vec3 WorldPosition;
};

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
};

struct DirLight
{
	vec3 direction;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

struct PointLight
{
	vec3 position;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float cosAngle;
};

in FS_In fsIn;
uniform Material material;
uniform vec3 ambientColor;
uniform vec3 ambientPower;
uniform vec3 cameraPosition;
uniform int currentPointLights;
uniform int currentDirLights;
uniform int currentSpotLights;
uniform PointLight pointLights[MAX_POINT_LIGHT];
uniform DirLight directionalLights[MAX_DIR_LIGHT];
uniform SpotLight spotLights[MAX_SPOT_LIGHT];

out vec3 outColor;

vec3 CalculateDirectionalLight(DirLight light, Material mat, vec3 objColor, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.0;
    if (diff > 0.0) 
	{
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    }
    vec3 ambient = light.ambientColor  * mat.ambientColor;
    vec3 diffuse = light.diffuseColor  * diff * mat.diffuseColor;
    vec3 specular = light.specularColor * spec * mat.specularColor;
    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, Material mat, vec3 objColor, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.0;
	if (diff != 0.0)
	{
	    vec3 reflectDir = reflect(-lightDir, normal);
	    spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    }
    vec3 ambient = light.ambientColor  * mat.ambientColor;
    vec3 diffuse = light.diffuseColor  * diff * mat.diffuseColor;
    vec3 specular = light.specularColor * spec * mat.specularColor;
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}

vec3 CalculateSpotLight(SpotLight light, Material mat, vec3 objColor, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float cosTheta = max(dot(lightDir, normalize(-light.direction)), 0);
    vec3 diffuse = vec3(0, 0, 0);
    vec3 specular = vec3(0, 0, 0);
    vec3 ambient = vec3(0, 0, 0);
    if (cosTheta > light.cosAngle)
	{
        float NdotD = dot(light.direction, normal);
        if (NdotD < 0.0)
		{
            float diff = 0.0;
            float spec = 0.0;
            diff = max(dot(normal, lightDir), 0.0);
            if (diff != 0.0)
			{
                vec3 reflectDir = reflect(-lightDir, normal);
                spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
            }
            ambient = light.ambientColor * mat.ambientColor;
            diffuse = light.diffuseColor  * diff * mat.diffuseColor;
            specular = light.specularColor * spec * mat.specularColor;
            float intensity = pow(cosTheta, 5);
            ambient *= intensity;
            diffuse *= intensity;
            specular *= intensity;
        }
    }
    return ambient + diffuse + specular;
}

void main()
{
    vec3 aColor = ambientColor * material.ambientColor * ambientPower;
    vec3 viewDir = normalize(cameraPosition - fsIn.WorldPosition);
    vec3 normal = normalize(fsIn.Normal);
    vec3 colorResult = vec3(0, 0, 0);
    if (!gl_FrontFacing)
	{
		normal = -normal;
	}
    
	for (int i = 0; i < currentDirLights; i++)
	{
        colorResult += CalculateDirectionalLight(directionalLights[i], material, fsIn.Color, normal, viewDir);
    }

    for (int i = 0; i < currentPointLights; i++)
	{
        colorResult += CalculatePointLight(pointLights[i], material, fsIn.Color, normal, fsIn.WorldPosition, viewDir);
    }

    for (int i = 0; i < currentSpotLights; i++)
	{
        colorResult += CalculateSpotLight(spotLights[i], material, fsIn.Color, normal, fsIn.WorldPosition, viewDir);
    }
    outColor = (aColor + colorResult);
}
