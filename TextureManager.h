#pragma once
#include "ResourceManager.h"
#include "Texture.h"
class TextureManager : public ResourceManager
{
public:
	static TextureManager* getInstance();
	static void destroy();
	Texture* createTextureFromFile(const wchar_t* filePath);

protected: 
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath);

private:
	TextureManager();
	virtual ~TextureManager();
	TextureManager(const TextureManager&) {}
	TextureManager& operator=(const TextureManager&) {}
	static TextureManager* sharedInstance;
	std::unordered_map<std::wstring, Resource*> m_map_textures; 
};

