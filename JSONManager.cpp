#include "JSONManager.h"
#include "GameObjectManager.h"
#include "DebugUIManager.h"
#include <fstream>
#include <string>

JSONManager* JSONManager::sharedInstance = nullptr;

JSONManager* JSONManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new JSONManager();
	}
	return sharedInstance;
}

void JSONManager::initialize(void* shaderByteCode, size_t sizeShader)
{
	if (!sharedInstance)
	{
		sharedInstance = new JSONManager();
	}
}

void JSONManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void JSONManager::save()
{
	int total = 0;
	json j;
	DebugUIManager::getInstance()->Log("Starting Export...");
	std::vector<GameObject*> list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Cubes);
	if (list.size() > 0) {
		DebugUIManager::getInstance()->Log("Cubes found, exporting");
		for (int i = 0; i < list.size(); i++) {
			j[std::to_string(total)] = {
				{"name", list[i]->name},
				{"objType", GameObjectManager::Cubes},
				{"position", {
					{"px", list[i]->getLocalPosition().x},
					{"py", list[i]->getLocalPosition().y},
					{"pz", list[i]->getLocalPosition().z}}
				},
				{"scale", {
					{"sx", list[i]->getLocalScale().x},
					{"sy", list[i]->getLocalScale().y},
					{"sz",list[i]->getLocalScale().z}}
				},
				{"rotation", {
					{"rx", list[i]->getLocalRotation().x},
					{"ry", list[i]->getLocalRotation().y},
					{"rz", list[i]->getLocalRotation().z}}
				},
				{"components", list[i]->getAttachedComponentTypes()}
			};
			total++;
		}
	}

	list.clear();
	list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Planes);
	if (list.size() > 0) {
		DebugUIManager::getInstance()->Log("Planes found, exporting");
		for (int i = 0; i < list.size(); i++) {

			j[std::to_string(total)] = {
				{"name", list[i]->name},
				{"objType", GameObjectManager::Planes},
				{"position", {
					{"px", list[i]->getLocalPosition().x},
					{"py", list[i]->getLocalPosition().y},
					{"pz", list[i]->getLocalPosition().z}}
				},
				{"scale", {
					{"sx", list[i]->getLocalScale().x},
					{"sy", list[i]->getLocalScale().y},
					{"sz",list[i]->getLocalScale().z}}
				},
				{"rotation", {
					{"rx", list[i]->getLocalRotation().x},
					{"ry", list[i]->getLocalRotation().y},
					{"rz", list[i]->getLocalRotation().z}}
				},
				{"components", list[i]->getAttachedComponentTypes()}
			};
			total++;
		}
	}

	list.clear();
	list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Meshes);
	if (list.size() > 0) {
		DebugUIManager::getInstance()->Log("Meshes found, exporting");
		for (int i = 0; i < list.size(); i++) {

			j[std::to_string(total)] = {
				{"name", list[i]->name},
				{"objType", GameObjectManager::Meshes},
				{"position", {
					{"px", list[i]->getLocalPosition().x},
					{"py", list[i]->getLocalPosition().y},
					{"pz", list[i]->getLocalPosition().z}}
				},
				{"scale", {
					{"sx", list[i]->getLocalScale().x},
					{"sy", list[i]->getLocalScale().y},
					{"sz",list[i]->getLocalScale().z}}
				},
				{"rotation", {
					{"rx", list[i]->getLocalRotation().x},
					{"ry", list[i]->getLocalRotation().y},
					{"rz", list[i]->getLocalRotation().z}}
				},
				{"components", list[i]->getAttachedComponentTypes()},
				{"texture", list[i]->textureLoc },
				{"mesh", list[i]->meshLoc}
			};
			total++;
		}
	}

	list.clear();
	list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Spheres);
	if (list.size() > 0) {
		DebugUIManager::getInstance()->Log("Spheres found, exporting");
		for (int i = 0; i < list.size(); i++) {

			j[std::to_string(total)] = {
				{"name", list[i]->name},
				{"objType", GameObjectManager::Spheres},
				{"position", {
					{"px", list[i]->getLocalPosition().x},
					{"py", list[i]->getLocalPosition().y},
					{"pz", list[i]->getLocalPosition().z}}
				},
				{"scale", {
					{"sx", list[i]->getLocalScale().x},
					{"sy", list[i]->getLocalScale().y},
					{"sz",list[i]->getLocalScale().z}}
				},
				{"rotation", {
					{"rx", list[i]->getLocalRotation().x},
					{"ry", list[i]->getLocalRotation().y},
					{"rz", list[i]->getLocalRotation().z}}
				},
				{"components", list[i]->getAttachedComponentTypes()}
			};
			total++;
		}
	}

	list.clear();
	list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Cylinders);
	if (list.size() > 0) {
		DebugUIManager::getInstance()->Log("Cylinders found, exporting");
		for (int i = 0; i < list.size(); i++) {

			j[std::to_string(total)] = {
				{"name", list[i]->name},
				{"objType", GameObjectManager::Cylinders},
				{"position", {
					{"px", list[i]->getLocalPosition().x},
					{"py", list[i]->getLocalPosition().y},
					{"pz", list[i]->getLocalPosition().z}}
				},
				{"scale", {
					{"sx", list[i]->getLocalScale().x},
					{"sy", list[i]->getLocalScale().y},
					{"sz",list[i]->getLocalScale().z}}
				},
				{"rotation", {
					{"rx", list[i]->getLocalRotation().x},
					{"ry", list[i]->getLocalRotation().y},
					{"rz", list[i]->getLocalRotation().z}}
				},
				{"components", list[i]->getAttachedComponentTypes()}
			};
			total++;
		}
	}

	list.clear();
	list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Capsules);
	if (list.size() > 0) {
		DebugUIManager::getInstance()->Log("Capsules found, exporting");
		for (int i = 0; i < list.size(); i++) {

			j[std::to_string(total)] = {
				{"name", list[i]->name},
				{"objType", GameObjectManager::Capsules},
				{"position", {
					{"px", list[i]->getLocalPosition().x},
					{"py", list[i]->getLocalPosition().y},
					{"pz", list[i]->getLocalPosition().z}}
				},
				{"scale", {
					{"sx", list[i]->getLocalScale().x},
					{"sy", list[i]->getLocalScale().y},
					{"sz",list[i]->getLocalScale().z}}
				},
				{"rotation", {
					{"rx", list[i]->getLocalRotation().x},
					{"ry", list[i]->getLocalRotation().y},
					{"rz", list[i]->getLocalRotation().z}}
				},
				{"components", list[i]->getAttachedComponentTypes()}
			};
			total++;
		}
	}


	DebugUIManager::getInstance()->Log("Exported to level.json");
	std::ofstream o("level.json");
	o << std::setw(4) << j << std::endl;
}

void JSONManager::load()
{
	GameObjectManager::getInstance()->clearAll();
	std::ifstream f("level.json");
	json j = json::parse(f);

	for (int i = 0; i < j.size(); i++) {
		Vector3D p(j[std::to_string(i)]["position"]["px"], j[std::to_string(i)]["position"]["py"], j[std::to_string(i)]["position"]["pz"]);
		Vector3D s(j[std::to_string(i)]["scale"]["sx"], j[std::to_string(i)]["scale"]["sy"], j[std::to_string(i)]["scale"]["sz"]);
		Vector3D r(j[std::to_string(i)]["rotation"]["rx"], j[std::to_string(i)]["rotation"]["ry"], j[std::to_string(i)]["rotation"]["rz"]);

		std::vector<int> c;
		for (auto& elem : j[std::to_string(i)]["components"])
			c.push_back(elem);
		bool hasPhys = false;
		for (auto& i : c) {
			if (i == 3) {
				hasPhys = true;
			}
		}



		if (j[std::to_string(i)]["objType"] == 0)
		{
			if (hasPhys) GameObjectManager::getInstance()->spawnP6Cube(p, s, r);
			else GameObjectManager::getInstance()->spawnCube(p, s, r);
		}
		else if (j[std::to_string(i)]["objType"] == 1)
		{
			if (hasPhys) GameObjectManager::getInstance()->spawnP6Plane(p, s, r);
			else GameObjectManager::getInstance()->spawnPlane(p, s, r);
		}
		else if (j[std::to_string(i)]["objType"] == 2)
		{
			GameObjectManager::getInstance()->spawnMesh(p, s, r, j[std::to_string(i)]["texture"].get<std::string>().c_str(), j[std::to_string(i)]["mesh"].get<std::string>().c_str());
		}
		else if (j[std::to_string(i)]["objType"] == 3)
		{
			if (hasPhys) GameObjectManager::getInstance()->spawnP6Sphere(p, s, r);
			else GameObjectManager::getInstance()->spawnSphere(p, s, r);
		}
		else if (j[std::to_string(i)]["objType"] == 4)
		{
			if (hasPhys) GameObjectManager::getInstance()->spawnP6Cylinder(p, s, r);
			else GameObjectManager::getInstance()->spawnCylinder(p, s, r);
		}
		else if (j[std::to_string(i)]["objType"] == 5)
		{
			if (hasPhys) GameObjectManager::getInstance()->spawnP6Capsule(p, s, r);
			else GameObjectManager::getInstance()->spawnCapsule(p, s, r);
		}


	}
}

JSONManager::JSONManager()
{

}

JSONManager::~JSONManager()
{

}