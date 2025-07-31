#pragma once
#include "GameObject.h"
#include "Cube.h"
#include "Plane.h"
#include "MeshObject.h"
class GameObjectManager
{
public:
	typedef std::string String;
	enum objType {Cubes = 0, Planes = 1, Meshes = 2};

	static GameObjectManager* getInstance();
	static void initialize(void* shaderByteCode, size_t sizeShader);
	static void destroy();
	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);
	void drawObjects(bool update, float deltaTime,int width, int height, VertexShader* vs, PixelShader* ps);
	GameObject* findGameObjectByName(const String& name);
	std::vector<GameObject*> getAllGameObjects();
	std::vector<GameObject*> getAllGameObjectsOfType(objType type);
	void getInspectorUI();
	void getObjectSpawnUI();

	void spawnCube();
	void spawnPlane();
	void spawnP6Cube();
	void spawnP6Plane();

private:
	GameObjectManager();
	~GameObjectManager();
	GameObjectManager(const GameObjectManager&) {}
	GameObjectManager& operator=(const GameObjectManager&) {}
	static GameObjectManager* sharedInstance;

	bool meshScreen = false;
	Cube* templateCube = nullptr;
	Plane* templatePlane = nullptr;

	std::vector<Cube*> cubes;
	std::vector<Plane*> planes;
	std::vector<MeshObject*> meshes;

};

