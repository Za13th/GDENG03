#include "TextureManager.h"

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
	Texture* texture = nullptr;
	try
	{
		texture = new Texture(filePath);
		m_map_textures[filePath] = texture; // Store the texture in the map
		return texture;
	}
	catch (...) {}

	return texture; 
}