#include "TextureManager.h"

#include <iostream>

TextureManager* TextureManager::s_instance = nullptr;

TextureManager* TextureManager::GetInstance()
{
	return s_instance;
}

void TextureManager::Initialize()
{
	if (GetInstance() == nullptr)
	{
		s_instance = new TextureManager();
	}
}

void TextureManager::Uninitialize()
{
	if (GetInstance() != nullptr)
	{
		delete s_instance;
		s_instance = nullptr;
	}
}

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	unloadAll();
}

Texture2D* TextureManager::loadTexture(const std::string& textureName)
{
	if (textureName.empty())
		return nullptr;

	auto it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		InstanceCounter<Texture2D>* instance = m_textures[textureName];
		instance->AddRef();
		return instance->getObjectPtr();
	}
	else
	{
        try
        {
            Texture2D* texture = Texture2D::CreateTexture(textureName);
            if (texture != nullptr)
            {
                m_textures.insert(std::pair<std::string, InstanceCounter<Texture2D>*>(textureName, new InstanceCounter<Texture2D>(texture)));
                m_inverseLookup.insert(std::pair<Texture2D*, std::string>(texture, textureName));
            }

            return texture;
        }
        catch (...)
        {
            return nullptr;
        }
	}
}

Texture2D* TextureManager::operator[](const std::string& textureName) const
{
	auto it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return (*it).second->getObjectPtr();
	}
	return nullptr;
}

std::string TextureManager::getTextureName(Texture2D * const texture) const
{
	auto it = m_inverseLookup.find(texture);
	if (it != m_inverseLookup.end())
	{
		return (*it).second;
	}
	return std::string();
}

bool TextureManager::unloadTexture(const std::string& textureName)
{
	auto it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		(*it).second->RemoveRef();

		if (!(*it).second->hasRef())
		{
			m_inverseLookup.erase((*it).second->getObjectPtr());
			delete (*it).second->getObjectPtr();
			delete (*it).second;
			m_textures.erase(it);
		}
		return true;
	}
	return false;
}

bool TextureManager::unloadTexture(Texture2D*& texture)
{
	const std::string& key = getTextureName(texture);
	if (!key.empty())
	{
		InstanceCounter<Texture2D>* instance = m_textures[key];
		instance->RemoveRef();

		if (!instance->hasRef())
		{
            m_inverseLookup.erase(texture);
			delete texture;
			delete instance;
			m_textures.erase(key);			
		}
		// On modifie le pointeur original puisqu'on a demandé 
		// de le decharger. Mais la geometrie pourrait encore exister 
		// car d'autre reference peuvent encore exister.
		texture = nullptr;
		return true;
	}
    else if (texture != nullptr)
    {
        // Lorsqu'on demande de décharger une géométrie qui est une forme générée
        // elle n'aura pas de clé dans le map, mais on doit la supprimer quand même.
        delete texture;
        texture = nullptr;
        return true;
    }
	return false;
}

void TextureManager::unloadAll()
{
	if (m_textures.size() > 0)
	{
        std::cout << "Il reste " << m_textures.size() << " textures en memoire..." << std::endl;
		for(auto &gi : m_textures)
		{
			delete gi.second->getObjectPtr();
			delete gi.second;
		}
		m_textures.clear();
		m_inverseLookup.clear();
	}
}