#ifndef _TEXTURE_TEXTURE_MANAGER_H_
#define _TEXTURE_TEXTURE_MANAGER_H_

#include "Texture.h"
#include "../Utilities/InstanceCounter.h"

#include <map>
#include <string>

class TextureManager
{
private:
	static TextureManager* s_instance;

	TextureManager();
	~TextureManager();

	TextureManager(const TextureManager& other) = delete;
	TextureManager& operator=(const TextureManager& other) = delete;

	std::map<std::string, InstanceCounter<Texture2D>*> m_textures;
	std::map<Texture2D*, std::string> m_inverseLookup;
	

public:
	static TextureManager* GetInstance();
	static void Initialize();
	static void Uninitialize();

	Texture2D* loadTexture(const std::string& textureName);
	Texture2D* operator[](const std::string& textureName) const;
	std::string getTextureName(Texture2D * const texture) const;
	bool unloadTexture(const std::string& textureName);
	bool unloadTexture(Texture2D*& texture);
	void unloadAll();
};

#endif
