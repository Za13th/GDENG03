#pragma once
#include "ResourceManager.h"
#include "Mesh.h"
class MeshManager : public ResourceManager
{
public:
	static MeshManager* getInstance();
	static void destroy();
	Mesh* createMeshFromFile(const wchar_t* filePath);
	Mesh* createMeshFromFile(const wchar_t* filePath, bool t, bool n);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath);
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath, bool t, bool n);

private:
	MeshManager();
	virtual ~MeshManager();
	MeshManager(const MeshManager&) {}
	MeshManager& operator=(const MeshManager&) {}
	static MeshManager* sharedInstance;
	std::unordered_map<std::wstring, Resource*> m_map_meshes;
};
