#pragma once
#include "GameObject.h"
#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Capsule.h"
#include "MeshObject.h"
class GameObjectManager
{
public:
	typedef std::string String;
	enum objType {Cubes = 0, Planes = 1, Meshes = 2, Spheres = 3, Cylinders = 4, Capsules = 5};

	static GameObjectManager* getInstance();
	static void initialize(void* shaderByteCode, size_t sizeShader);
	static void destroy();
	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);
	void clearAll();
	void drawObjects(bool update, float deltaTime,int width, int height, VertexShader* vs, PixelShader* ps);
	GameObject* findGameObjectByName(const String& name);
	std::vector<GameObject*> getAllGameObjects();
	std::vector<GameObject*> getAllGameObjectsOfType(objType type);
	void getInspectorUI();
	void getObjectSpawnUI();

	void spawnCube();
	void spawnPlane();
	void spawnSphere();
	void spawnCylinder();
	void spawnCapsule();


	void spawnP6Cube();
	void spawnP6Plane();
	void spawnP6Sphere();
	void spawnP6Cylinder();
	void spawnP6Capsule();	
	
	void spawnCube(Vector3D pos, Vector3D scale, Vector3D rot);
	void spawnPlane(Vector3D pos, Vector3D scale, Vector3D rot);
	void spawnMesh(Vector3D pos, Vector3D scale, Vector3D rot, const char meshPath[128], const char texturePath[128]);
	void spawnP6Cube(Vector3D pos, Vector3D scale, Vector3D rot);
	void spawnP6Plane(Vector3D pos, Vector3D scale, Vector3D rot);
	void spawnP6Sphere(Vector3D pos, Vector3D scale, Vector3D rot);
	void spawnP6Cylinder(Vector3D pos, Vector3D scale, Vector3D rot);
	void spawnP6Capsule(Vector3D pos, Vector3D scale, Vector3D rot);

	void setCurrentObject(GameObject* g) { this->currentObject = g; }
	bool inspectorWindowOpen = false;

private:
	GameObjectManager();
	~GameObjectManager();
	GameObjectManager(const GameObjectManager&) {}
	GameObjectManager& operator=(const GameObjectManager&) {}
	static GameObjectManager* sharedInstance;

	bool meshScreen = false;
	Cube* templateCube = nullptr;
	Plane* templatePlane = nullptr;
	Sphere* templateSphere = nullptr;
	Cylinder* templateCylinder = nullptr;
	Capsule* templateCapsule = nullptr;

	std::vector<Cube*> cubes;
	std::vector<Plane*> planes;
	std::vector<Sphere*> spheres;
	std::vector<Cylinder*> cylinders;
	std::vector<Capsule*> capsules;
	std::vector<MeshObject*> meshes;

	GameObject* currentObject = nullptr;
	

};

