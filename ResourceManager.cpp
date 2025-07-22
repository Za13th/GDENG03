#include "ResourceManager.h"

ResourceManager* ResourceManager::sharedInstance = nullptr;

ResourceManager* ResourceManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new ResourceManager();
	}
	return sharedInstance;
}

void ResourceManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	// Clean up all resources
	for (auto& pair : m_map_resources)
	{
		delete pair.second;
	}
	m_map_resources.clear();
}

Resource* ResourceManager::createResourceFromFile(const wchar_t* filePath)
{
	std::wstring fullPath = std::filesystem::absolute(filePath).wstring();

	auto it = m_map_resources.find(fullPath);

	if (it != m_map_resources.end())
	{
		// Resource already exists, return it
		return it->second;
	}

	Resource* raw_res = createResourceFromFileConcrete(fullPath.c_str());

	if(raw_res)
	{
		m_map_resources[fullPath] = raw_res;
		return raw_res;
	}

	return nullptr;
}

std::unordered_map<std::wstring, Resource*> ResourceManager::getMapResources()
{
	return m_map_resources;
}