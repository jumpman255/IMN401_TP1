#ifndef _MATERIAL_SHADERMANAGER_H_
#define _MATERIAL_SHADERMANAGER_H_

#include "Shaders.h"
#include "../Utilities/InstanceCounter.h"

#include <map>
#include <string>

class ShaderManager
{
	static ShaderManager* m_instance;

	ShaderManager();
	~ShaderManager();

	ShaderManager(const ShaderManager& other) = delete;
	ShaderManager& operator=(const ShaderManager& other) = delete;

	std::map<std::string, InstanceCounter<BaseShader>*> m_shaders;
	std::map<BaseShader*, std::string> m_inverseLookup;
	

public:
	static ShaderManager* GetInstance();
	static void Initialize();
	static void Uninitialize();

	VertexShader* LoadVertexShader(const std::string& path, const std::string& vShaderName);
    FragmentShader* LoadFragmentShader(const std::string& path, const std::string& fShaderName);
	std::string GetShaderName(BaseShader * const geom) const;
	bool UnloadShader(const std::string& shaderName);
    bool UnloadShader(VertexShader*& shader);
    bool UnloadShader(FragmentShader*& shader);
	void UnloadAll();
};

#endif
