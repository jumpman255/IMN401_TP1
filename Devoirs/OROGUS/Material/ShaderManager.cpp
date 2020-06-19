#include "ShaderManager.h"
#include "ShaderHelper.h"

#include <fstream>
#include <iostream>

ShaderManager* ShaderManager::m_instance = nullptr;

ShaderManager* ShaderManager::GetInstance()
{
	return m_instance;
}

void ShaderManager::Initialize()
{
	if (m_instance == nullptr)
	{
		m_instance = new ShaderManager();
	}
}

void ShaderManager::Uninitialize()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	UnloadAll();
}

VertexShader* ShaderManager::LoadVertexShader(const std::string& path, const std::string& vShaderName)
{
	if (vShaderName == "")
		return nullptr;

	auto it = m_shaders.find(vShaderName);
	if (it != m_shaders.end())
	{
		InstanceCounter<BaseShader>* instance = m_shaders[vShaderName];
		instance->AddRef();
		return static_cast<VertexShader*>(instance->getObjectPtr());
	}
	else
	{
		VertexShader* vShader = nullptr;
        std::ifstream vShaderFile;
        vShaderFile.open(path + vShaderName);
        if (vShaderFile.is_open())
        {
            std::string contents;
            vShaderFile.seekg(0, std::ios::end);
            contents.resize((size_t)vShaderFile.tellg());
            vShaderFile.seekg(0, std::ios::beg);
            vShaderFile.read(&contents[0], contents.size());
            vShaderFile.close();

            vShader = new VertexShader(vShaderName, contents);
        }
        else
        {
            vShader = ShaderHelper::LoadVertexShader(vShaderName);
        }
			
		if (vShader != nullptr)
		{
			m_shaders.insert(std::pair<std::string, InstanceCounter<BaseShader>*>(vShader->shaderName(), new InstanceCounter<BaseShader>(vShader)));
			m_inverseLookup.insert(std::pair<BaseShader*, std::string>(vShader, vShader->shaderName()));
		}

		return vShader;
	}
}

FragmentShader* ShaderManager::LoadFragmentShader(const std::string& path, const std::string& fShaderName)
{
    if (fShaderName == "")
        return nullptr;

    auto it = m_shaders.find(fShaderName);
    if (it != m_shaders.end())
    {
        InstanceCounter<BaseShader>* instance = m_shaders[fShaderName];
        instance->AddRef();
        return static_cast<FragmentShader*>(instance->getObjectPtr());
    }
    else
    {
        FragmentShader* fShader = nullptr;
        std::ifstream fShaderFile;
        fShaderFile.open(path + fShaderName);
        if (fShaderFile.is_open())
        {
            std::string contents;
            fShaderFile.seekg(0, std::ios::end);
            contents.resize((size_t)fShaderFile.tellg());
            fShaderFile.seekg(0, std::ios::beg);
            fShaderFile.read(&contents[0], contents.size());
            fShaderFile.close();

            fShader = new FragmentShader(fShaderName, contents);
        }
        else
        {
            fShader = ShaderHelper::LoadFragmentShader(fShaderName);
        }

        if (fShader != nullptr)
        {
            m_shaders.insert(std::pair<std::string, InstanceCounter<BaseShader>*>(fShader->shaderName(), new InstanceCounter<BaseShader>(fShader)));
            m_inverseLookup.insert(std::pair<BaseShader*, std::string>(fShader, fShader->shaderName()));
        }

        return fShader;
    }
}

std::string ShaderManager::GetShaderName(BaseShader * const shader) const
{
	auto it = m_inverseLookup.find(shader);
	if (it != m_inverseLookup.end())
	{
		return (*it).second;
	}
	return std::string();
}

bool ShaderManager::UnloadShader(const std::string& shaderName)
{
	auto it = m_shaders.find(shaderName);
	if (it != m_shaders.end())
	{
		(*it).second->RemoveRef();

		if (!(*it).second->hasRef())
		{
            InstanceCounter<BaseShader>* instance = (*it).second;
			m_inverseLookup.erase(instance->getObjectPtr());
            m_shaders.erase(it);
            delete instance->getObjectPtr();
            delete instance;
		}
		return true;
	}
	return false;
}

bool ShaderManager::UnloadShader(VertexShader*& shader)
{
    if (shader != nullptr)
    {
        auto it = m_shaders.find(shader->shaderName());
        if (it != m_shaders.end())
        {
            InstanceCounter<BaseShader>* instance = (*it).second;
            instance->RemoveRef();

            if (!instance->hasRef())
            {
                m_inverseLookup.erase(shader);
                delete shader;
                delete instance;
                m_shaders.erase(it);
            }
            // On modifie le pointeur original puisqu'on a demandé 
            // de le decharger. Mais le shader pourrait encore exister 
            // car d'autre reference peuvent encore exister.
            shader = nullptr;
            return true;
        }
        else
        {
            // Lorsqu'on demande de décharger un shader qui est chargé autrement que par ce manager
            // il n'aura pas de clé dans le map, mais on doit le supprimer quand même.
            delete shader;
            shader = nullptr;
            return true;
        }
    }
    return false;
}

bool ShaderManager::UnloadShader(FragmentShader*& shader)
{
    if (shader != nullptr)
    {
        auto it = m_shaders.find(shader->shaderName());
        if (it != m_shaders.end())
        {
            InstanceCounter<BaseShader>* instance = (*it).second;
            instance->RemoveRef();

            if (!instance->hasRef())
            {
                m_inverseLookup.erase(shader);
                delete shader;
                delete instance;
                m_shaders.erase(it);
            }
            // On modifie le pointeur original puisqu'on a demandé 
            // de le decharger. Mais le shader pourrait encore exister 
            // car d'autre reference peuvent encore exister.
            shader = nullptr;
            return true;
        }
        else
        {
            // Lorsqu'on demande de décharger un shader qui est chargé autrement que par ce manager
            // il n'aura pas de clé dans le map, mais on doit le supprimer quand même.
            delete shader;
            shader = nullptr;
            return true;
        }
    }
    return false;
}

void ShaderManager::UnloadAll()
{
	if (m_shaders.size() > 0)
	{
        std::cout << "Il reste " << m_shaders.size() << " shaders en memoire..." << std::endl;
		for(auto &gi : m_shaders)
		{
			delete gi.second->getObjectPtr();
			delete gi.second;
		}
		m_shaders.clear();
		m_inverseLookup.clear();
	}
}