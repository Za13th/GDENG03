#include "JSONManager.h"
#include "GameObjectManager.h"
#include "DebugUIManager.h"
#include <fstream>

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
	json j;
	DebugUIManager::getInstance()->Log("Starting Export...");
	std::vector<GameObject*> list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Cubes);
	if (list.size() > 0) {
		DebugUIManager::getInstance()->Log("Cubes found, exporting");
		for (int i = 0; i < list.size(); i++) {

			j["Game Object"] = {
				{"name", list[i]->name},
				{"objType", GameObjectManager::Cubes},
				{"position", {list[i]->getLocalPosition().x, list[i]->getLocalPosition().y, list[i]->getLocalPosition().z}},
				{"scale", {list[i]->getLocalScale().x, list[i]->getLocalScale().y, list[i]->getLocalScale().z}},
				{"rotation", {list[i]->getLocalRotation().x, list[i]->getLocalRotation().y, list[i]->getLocalRotation().z}},
				{"components", list[i]->getAttachedComponentTypes()}
			};
		}
	}

	list.clear();
	list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Planes);
	if (list.size() > 0) {
		DebugUIManager::getInstance()->Log("Planes found, exporting");
		for (int i = 0; i < list.size(); i++) {

			j["Game Object"] = {
				{"name", list[i]->name},
				{"objType", GameObjectManager::Planes},
				{"position", {list[i]->getLocalPosition().x, list[i]->getLocalPosition().y, list[i]->getLocalPosition().z}},
				{"scale", {list[i]->getLocalScale().x, list[i]->getLocalScale().y, list[i]->getLocalScale().z}},
				{"rotation", {list[i]->getLocalRotation().x, list[i]->getLocalRotation().y, list[i]->getLocalRotation().z}},
				{"components", list[i]->getAttachedComponentTypes()}
			};
		}
	}

	list.clear();
	list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Meshes);
	if (list.size() > 0) {
		DebugUIManager::getInstance()->Log("Meshes found, exporting");
		for (int i = 0; i < list.size(); i++) {

			j["Game Object"] = {
				{"name", list[i]->name},
				{"objType", GameObjectManager::Meshes},
				{"position", {list[i]->getLocalPosition().x, list[i]->getLocalPosition().y, list[i]->getLocalPosition().z}},
				{"scale", {list[i]->getLocalScale().x, list[i]->getLocalScale().y, list[i]->getLocalScale().z}},
				{"rotation", {list[i]->getLocalRotation().x, list[i]->getLocalRotation().y, list[i]->getLocalRotation().z}},
				{"components", list[i]->getAttachedComponentTypes()},
				{"texture", list[i]->textureLoc },
				{"mesh", list[i]->meshLoc}
			};
		}
	}

	DebugUIManager::getInstance()->Log("Exported to level.json");
	std::ofstream o("level.json");
	o << std::setw(4) << j << std::endl;
}

void JSONManager::load()
{
}

JSONManager::JSONManager()
{

}

JSONManager::~JSONManager()
{

}