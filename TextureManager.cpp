#include "TextureManager.h"
#include <iostream>
#include <filesystem>

TextureManager* TextureManager::sharedInstance = nullptr;

TextureManager* TextureManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new TextureManager();
	}
	return sharedInstance;
}

void TextureManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

TextureManager::TextureManager() : ResourceManager()
{
}

TextureManager::~TextureManager()
{

}

Texture* TextureManager::createTextureFromFile(const wchar_t* filePath)
{
	return (Texture*)(createResourceFromFileConcrete(filePath));
}

Resource* TextureManager::createResourceFromFileConcrete(const wchar_t* filePath)
{
	if (!std::filesystem::exists(filePath))
		return nullptr;

	Texture* texture = (Texture*)(this->m_map_textures[filePath]);

	if (!texture)
	{
		try
		{
			texture = new Texture(filePath);
			m_map_textures[filePath] = texture; // Store the texture in the map
			return texture;
		}
		catch (...) {}
	}


	return texture; 
}