#include "ResourcesManager.h"
#include "../Geometry/GeometryManager.h"
#include "../Material/ShaderManager.h"
#include "../Texture/TextureManager.h"

bool ResourcesManager::Initialize()
{
    GeometryManager::Initialize();
    ShaderManager::Initialize();
    TextureManager::Initialize();
    return GeometryManager::GetInstance() != nullptr && ShaderManager::GetInstance() != nullptr && TextureManager::GetInstance() != nullptr;
}

void ResourcesManager::Uninitialize()
{
	TextureManager::Uninitialize();
    ShaderManager::Uninitialize();
    GeometryManager::Uninitialize();    
}