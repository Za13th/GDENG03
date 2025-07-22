#include "MeshManager.h"


MeshManager* MeshManager::sharedInstance = nullptr;

MeshManager* MeshManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new MeshManager();
	}
	return sharedInstance;
}

void MeshManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

MeshManager::MeshManager() : ResourceManager()
{
}

MeshManager::~MeshManager()
{

}

Mesh* MeshManager::createMeshFromFile(const wchar_t* filePath)
{
	return  (Mesh*)(createResourceFromFileConcrete(filePath));
}

Mesh* MeshManager::createMeshFromFile(const wchar_t* filePath, bool t, bool n)
{
	return  (Mesh*)(createResourceFromFileConcrete(filePath, t, n));
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* filePath)
{
	Mesh* mesh = nullptr;
	try
	{
		mesh = new Mesh(filePath);
		m_map_meshes[filePath] = mesh; // Store the Mesh in the map
		return mesh;
	}
	catch (...) {}

	return mesh;
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* filePath, bool t, bool n)
{
	Mesh* mesh = nullptr;
	try
	{
		mesh = new Mesh(filePath,t,n);
		m_map_meshes[filePath] = mesh; // Store the Mesh in the map
		return mesh;
	}
	catch (...) {}

	return mesh;
}