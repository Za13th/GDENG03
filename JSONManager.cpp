#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <string>

#include "JSONManager.h"
#include "GameObjectManager.h"
#include "DebugUIManager.h"
#include "BaseComponentSystem.h"


JSONManager* JSONManager::sharedInstance = nullptr;

JSONManager* JSONManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new JSONManager();
	}
	return sharedInstance;
}

void JSONManager::initialize()
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

void JSONManager::save(bool bg)
{
    int total = 0;
    json j, wrapper;

    if(!bg)
    DebugUIManager::getInstance()->Log("Starting Export...");

    std::vector<GameObject*> list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Cubes);
    if (list.size() > 0) 
    {
        if (!bg)
        DebugUIManager::getInstance()->Log("Cubes found, exporting");
        for (int i = 0; i < list.size(); i++) 
        {
            const wchar_t* path = L"null";
            if (list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")) 
            {
                path = list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")->getTexturePath();
            }

            json obj = 
            {
                {"name", list[i]->name},
                {"objType", GameObjectManager::Cubes},
                {"position", 
                {
                    {"px", list[i]->getLocalPosition().x},
                    {"py", list[i]->getLocalPosition().y},
                    {"pz", list[i]->getLocalPosition().z}
                }},
                {"scale", 
                {
                    {"sx", list[i]->getLocalScale().x},
                    {"sy", list[i]->getLocalScale().y},
                    {"sz", list[i]->getLocalScale().z}
                }},
                {"rotation", 
                {
                    {"rx", list[i]->getLocalRotation().x * (180.0 / M_PI)},
                    {"ry", list[i]->getLocalRotation().y * (180.0 / M_PI)},
                    {"rz", list[i]->getLocalRotation().z * (180.0 / M_PI)}
                }},
                {"components", list[i]->getAttachedComponentTypes()},
                {"texture", path}
            };

            j.push_back(obj);  
        }
    }
    list.clear();

    list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Planes);
    if (list.size() > 0) 
    {
        if (!bg)
        DebugUIManager::getInstance()->Log("Planes found, exporting");
        for (int i = 0; i < list.size(); i++) 
        {
            const wchar_t* path = L"null";
            if (list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")) 
            {
                path = list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")->getTexturePath();
            }

            json obj = 
            {
                {"name", list[i]->name},
                {"objType", GameObjectManager::Planes},
                {"position", 
                {
                    {"px", list[i]->getLocalPosition().x},
                    {"py", list[i]->getLocalPosition().y},
                    {"pz", list[i]->getLocalPosition().z}
                }},
                {"scale", 
                {
                    {"sx", list[i]->getLocalScale().x},
                    {"sy", list[i]->getLocalScale().y},
                    {"sz", list[i]->getLocalScale().z}
                }},
                {"rotation", 
                {
                    {"rx", list[i]->getLocalRotation().x * (180.0 / M_PI)},
                    {"ry", list[i]->getLocalRotation().y * (180.0 / M_PI)},
                    {"rz", list[i]->getLocalRotation().z * (180.0 / M_PI)}
                }},
                {"components", list[i]->getAttachedComponentTypes()},
                {"texture", path}
            };

            j.push_back(obj);  
        }
    }
    list.clear();

    list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Meshes);
    if (list.size() > 0) 
    {
        if (!bg)
        DebugUIManager::getInstance()->Log("Meshes found, exporting");
        for (int i = 0; i < list.size(); i++) 
        {
            json obj = 
            {
                {"name", list[i]->name},
                {"objType", GameObjectManager::Meshes},
                {"position", 
                {
                    {"px", list[i]->getLocalPosition().x},
                    {"py", list[i]->getLocalPosition().y},
                    {"pz", list[i]->getLocalPosition().z}
                }},
                {"scale", 
                {
                    {"sx", list[i]->getLocalScale().x},
                    {"sy", list[i]->getLocalScale().y},
                    {"sz", list[i]->getLocalScale().z}
                }},
                {"rotation", 
                {
                    {"rx", list[i]->getLocalRotation().x * (180.0 / M_PI)},
                    {"ry", list[i]->getLocalRotation().y * (180.0 / M_PI)},
                    {"rz", list[i]->getLocalRotation().z * (180.0 / M_PI)}
                }},
                {"components", list[i]->getAttachedComponentTypes()},
                {"texture", list[i]->textureLoc},
                {"mesh", list[i]->meshLoc}
            };

            j.push_back(obj);  
        }
    }
    list.clear();

    list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Spheres);
    if (list.size() > 0) 
    {
        if (!bg)
        DebugUIManager::getInstance()->Log("Spheres found, exporting");
        for (int i = 0; i < list.size(); i++) 
        {
            const wchar_t* path = L"null";
            if (list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")) 
            {
                path = list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")->getTexturePath();
            }

            json obj = 
            {
                {"name", list[i]->name},
                {"objType", GameObjectManager::Spheres},
                {"position", 
                {
                    {"px", list[i]->getLocalPosition().x},
                    {"py", list[i]->getLocalPosition().y},
                    {"pz", list[i]->getLocalPosition().z}
                }},
                {"scale", 
                {
                    {"sx", list[i]->getLocalScale().x},
                    {"sy", list[i]->getLocalScale().y},
                    {"sz", list[i]->getLocalScale().z}
                }},
                {"rotation", 
                {
                    {"rx", list[i]->getLocalRotation().x * (180.0 / M_PI)},
                    {"ry", list[i]->getLocalRotation().y * (180.0 / M_PI)},
                    {"rz", list[i]->getLocalRotation().z * (180.0 / M_PI)}
                }},
                {"components", list[i]->getAttachedComponentTypes()},
                {"texture", path}
            };

            j.push_back(obj);  
        }
    }
    list.clear();

    list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Cylinders);
    if (list.size() > 0) 
    {
        if (!bg)
        DebugUIManager::getInstance()->Log("Cylinders found, exporting");
        for (int i = 0; i < list.size(); i++) 
        {
            const wchar_t* path = L"null";
            if (list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")) 
            {
                path = list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")->getTexturePath();
            }

            json obj = 
            {
                {"name", list[i]->name},
                {"objType", GameObjectManager::Cylinders},
                {"position", 
                {
                    {"px", list[i]->getLocalPosition().x},
                    {"py", list[i]->getLocalPosition().y},
                    {"pz", list[i]->getLocalPosition().z}
                }},
                {"scale", 
                {
                    {"sx", list[i]->getLocalScale().x},
                    {"sy", list[i]->getLocalScale().y},
                    {"sz", list[i]->getLocalScale().z}
                }},
                {"rotation", 
                {
                    {"rx", list[i]->getLocalRotation().x * (180.0 / M_PI)},
                    {"ry", list[i]->getLocalRotation().y * (180.0 / M_PI)},
                    {"rz", list[i]->getLocalRotation().z * (180.0 / M_PI)}
                }},
                {"components", list[i]->getAttachedComponentTypes()},
                {"texture", path}
            };

            j.push_back(obj);  
        }
    }
    list.clear();

    list = GameObjectManager::getInstance()->getAllGameObjectsOfType(GameObjectManager::Capsules);
    if (list.size() > 0) 
    {
        if (!bg)
        DebugUIManager::getInstance()->Log("Capsules found, exporting");
        for (int i = 0; i < list.size(); i++) 
        {
            const wchar_t* path = L"null";
            if (list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")) 
            {
                path = list[i]->findComponentByType(Component::Material, list[i]->name + " TX Component")->getTexturePath();
            }

            json obj = 
            {
                {"name", list[i]->name},
                {"objType", GameObjectManager::Capsules},
                {"position", 
                {
                    {"px", list[i]->getLocalPosition().x},
                    {"py", list[i]->getLocalPosition().y},
                    {"pz", list[i]->getLocalPosition().z}
                }},
                {"scale", 
                {
                    {"sx", list[i]->getLocalScale().x},
                    {"sy", list[i]->getLocalScale().y},
                    {"sz", list[i]->getLocalScale().z}
                }},
                {"rotation", 
                {
                    {"rx", list[i]->getLocalRotation().x * (180.0 / M_PI)},
                    {"ry", list[i]->getLocalRotation().y * (180.0 / M_PI)},
                    {"rz", list[i]->getLocalRotation().z * (180.0 / M_PI)}
                }},
                {"components", list[i]->getAttachedComponentTypes()},
                {"texture", path}
            };

            j.push_back(obj);  
        }
    }



    wrapper["GameObjects"] = j;

    if (bg) 
    {
        std::ofstream o("bg.json");
        o << std::setw(4) << wrapper << std::endl;
    }
    else 
    {
        DebugUIManager::getInstance()->Log("Exported to level.json");
        std::ofstream o("scene.level");
        o << std::setw(4) << wrapper << std::endl;
    }
}



void JSONManager::load(bool bg)
{
    GameObjectManager::getInstance()->clearAll();
    BaseComponentSystem::getInstance()->getPhysicsSystem()->unregisterAllComponents();

    std::ifstream f("bg.json");
    std::ifstream f2("scene.level");
    json j;

    if (bg) 
        j = json::parse(f);

    else 
        j = json::parse(f2);
    
    if (j.contains("GameObjects")) 
    {
        for (const auto& obj : j["GameObjects"]) 
        {  
            Vector3D p(obj["position"]["px"], obj["position"]["py"], obj["position"]["pz"]);
            Vector3D s(obj["scale"]["sx"], obj["scale"]["sy"], obj["scale"]["sz"]);
            Vector3D r(obj["rotation"]["rx"], obj["rotation"]["ry"], obj["rotation"]["rz"]);

            r = r * (M_PI / 180.0);

            std::vector<int> c;
            for (const auto& elem : obj["components"]) 
            {
                c.push_back(elem);
            }

            bool hasPhys = false;
            bool hasMat = false;
            for (auto& i : c) 
            {
                if (i == 3) 
                    hasPhys = true;
                

                if (i == 4) 
                    hasMat = true;
            }

            if (obj["objType"] == 0) 
            {
                if (hasPhys) GameObjectManager::getInstance()->spawnP6Cube(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
                else GameObjectManager::getInstance()->spawnCube(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
            }
            else if (obj["objType"] == 1) 
            {
                if (hasPhys) GameObjectManager::getInstance()->spawnP6Plane(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
                else GameObjectManager::getInstance()->spawnPlane(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
            }
            else if (obj["objType"] == 2) 
            {
                GameObjectManager::getInstance()->spawnMesh(p, s, r, obj["mesh"].get<std::string>().c_str(), obj["texture"].get<std::string>().c_str());
            }
            else if (obj["objType"] == 3) 
            {
                if (hasPhys) GameObjectManager::getInstance()->spawnP6Sphere(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
                else GameObjectManager::getInstance()->spawnSphere(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
            }
            else if (obj["objType"] == 4) 
            {
                if (hasPhys) GameObjectManager::getInstance()->spawnP6Cylinder(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
                else GameObjectManager::getInstance()->spawnCylinder(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
            }
            else if (obj["objType"] == 5) 
            {
                if (hasPhys) GameObjectManager::getInstance()->spawnP6Capsule(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
                else GameObjectManager::getInstance()->spawnCapsule(p, s, r, hasMat, obj["texture"].get<std::string>().c_str());
            }
        }
    }
    else 
        DebugUIManager::getInstance()->Log("No Game Objects found in the save file.");
    
}



JSONManager::JSONManager()
{

}

JSONManager::~JSONManager()
{

}