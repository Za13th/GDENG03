#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>
#include "Resource.h"
class ResourceManager
{
public:
	static ResourceManager* getInstance();
	static void destroy();

	Resource* createResourceFromFile(const wchar_t* filePath);
	std::unordered_map<std::wstring, Resource*> getMapResources();

protected: 
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager&) {}
	ResourceManager& operator=(const ResourceManager&) {}
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath) { return nullptr; }

private:
	static ResourceManager* sharedInstance;
	std::unordered_map<std::wstring, Resource*> m_map_resources; // Map to hold resources by name
};

