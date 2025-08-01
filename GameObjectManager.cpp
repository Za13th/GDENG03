#include "GameObjectManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "BaseComponentSystem.h"
#include "SceneCameraHolder.h"
#include "PhysicsComponent.h"
#include "DebugUIManager.h"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

GameObjectManager* GameObjectManager::sharedInstance = nullptr;

GameObjectManager* GameObjectManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new GameObjectManager();
	}
	return sharedInstance;
}

void GameObjectManager::initialize(void* shaderByteCode, size_t sizeShader)
{
	if (!sharedInstance)
	{
		sharedInstance = new GameObjectManager();

		sharedInstance->templateCube = new Cube("Template Cube", shaderByteCode, sizeShader);
		sharedInstance->templatePlane = new Plane("Template Plane", shaderByteCode, sizeShader);
		sharedInstance->templateSphere = new Sphere("Template Sphere", shaderByteCode, sizeShader);
		sharedInstance->templateCylinder = new Cylinder("Template Cylinder", shaderByteCode, sizeShader);
		sharedInstance->templateCapsule = new Capsule("Template Capsule", shaderByteCode, sizeShader);
	}
}

void GameObjectManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		sharedInstance->templateCube->release();
		sharedInstance->templatePlane->release();
		sharedInstance->templateSphere->release();
		sharedInstance->templateCylinder->release();
		sharedInstance->templateCapsule->release();

		for (int i = 0; i < sharedInstance->meshes.size(); i++)
			sharedInstance->meshes[i]->release();

		sharedInstance->cubes.clear();
		sharedInstance->planes.clear();
		sharedInstance->spheres.clear();
		sharedInstance->cylinders.clear();
		sharedInstance->capsules.clear();
		sharedInstance->meshes.clear();

		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

GameObjectManager::GameObjectManager()
{

}

GameObjectManager::~GameObjectManager()
{

}

void GameObjectManager::addGameObject(GameObject* gameObject)
{
	if (gameObject->objectType == GameObject::Cube)
	{
		Cube* cube = static_cast<Cube*>(gameObject);
		cubes.push_back(cube);
	}
	else if (gameObject->objectType == GameObject::Plane)
	{
		Plane* plane = static_cast<Plane*>(gameObject);
		planes.push_back(plane);
	}
	else if (gameObject->objectType == GameObject::Sphere)
	{
		Sphere* sphere = static_cast<Sphere*>(gameObject);
		spheres.push_back(sphere);
	}
	else if (gameObject->objectType == GameObject::Cylinder)
	{
		Cylinder* cylinder = static_cast<Cylinder*>(gameObject);
		cylinders.push_back(cylinder);
	}
	else if (gameObject->objectType == GameObject::Capsule)
	{
		Capsule* capsule = static_cast<Capsule*>(gameObject);
		capsules.push_back(capsule);
	}
	else if (gameObject->objectType == GameObject::MeshObject)
	{
		MeshObject* meshObj = static_cast<MeshObject*>(gameObject);
		meshes.push_back(meshObj);
	}
}

void GameObjectManager::removeGameObject(GameObject* gameObject)
{
	if (gameObject->objectType == GameObject::Cube)
	{
		Cube* cube = static_cast<Cube*>(gameObject);
		auto it = std::find(cubes.begin(), cubes.end(), cube);
		if (it != cubes.end())
		{
			cubes.erase(it);
			gameObject->detachAllComponents();
		}
	}
	else if (gameObject->objectType == GameObject::Plane)
	{
		Plane* plane = static_cast<Plane*>(gameObject);
		auto it = std::find(planes.begin(), planes.end(), plane);
		if (it != planes.end())
		{
			planes.erase(it);
			gameObject->detachAllComponents();
		}
	}
	else if (gameObject->objectType == GameObject::Sphere)
	{
		Sphere* sphere = static_cast<Sphere*>(gameObject);
		auto it = std::find(spheres.begin(), spheres.end(), sphere);
		if (it != spheres.end())
		{
			spheres.erase(it);
			gameObject->detachAllComponents();
		}
	}
	else if (gameObject->objectType == GameObject::Cylinder)
	{
		Cylinder* cylinder = static_cast<Cylinder*>(gameObject);
		auto it = std::find(cylinders.begin(), cylinders.end(), cylinder);
		if (it != cylinders.end())
		{
			cylinders.erase(it);
			gameObject->detachAllComponents();
		}
	}
	else if (gameObject->objectType == GameObject::Capsule)
	{
		Capsule* capsule = static_cast<Capsule*>(gameObject);
		auto it = std::find(capsules.begin(), capsules.end(), capsule);
		if (it != capsules.end())
		{
			capsules.erase(it);
			gameObject->detachAllComponents();
		}
	}
	else if (gameObject->objectType == GameObject::MeshObject)
	{
		MeshObject* meshObj = static_cast<MeshObject*>(gameObject);
		auto it = std::find(meshes.begin(), meshes.end(), meshObj);
		if (it != meshes.end())
		{
			meshes.erase(it);
			gameObject->detachAllComponents();
		}
	}
}

void GameObjectManager::clearAll()
{
	cubes.clear();
	planes.clear();
	meshes.clear();
	spheres.clear();
	cylinders.clear();
	capsules.clear();
}

void GameObjectManager::drawObjects(bool update, float deltaTime, int width, int height, VertexShader* vs, PixelShader* ps)
{
	for (int i = 0; i < cubes.size(); i++)
	{
		if (update)
		{
			cubes[i]->update(deltaTime);
		}
		cubes[i]->draw(width, height, vs, ps);
	}
	for (int i = 0; i < planes.size(); i++)
	{
		if (update)
		{
			planes[i]->update(deltaTime);
		}
		planes[i]->draw(width, height, vs, ps);
	}
	for (int i = 0; i < spheres.size(); i++)
	{
		if (update)
		{
			spheres[i]->update(deltaTime);
		}
		spheres[i]->draw(width, height, vs, ps);
	}
	for (int i = 0; i < cylinders.size(); i++)
	{
		if (update)
		{
			cylinders[i]->update(deltaTime);
		}
		cylinders[i]->draw(width, height, vs, ps);
	}
	for (int i = 0; i < capsules.size(); i++)
	{
		if (update)
		{
			capsules[i]->update(deltaTime);
		}
		capsules[i]->draw(width, height, vs, ps);
	}
	for (int i = 0; i < meshes.size(); i++)
	{
		if (update)
		{
			meshes[i]->update(deltaTime);
		}
		meshes[i]->draw(width, height, vs, ps);
	}
}

GameObject* GameObjectManager::findGameObjectByName(const String& name)
{
	for (auto& cube : cubes)
	{
		if (cube->name == name)
			return cube;
	}
	for (auto& plane : planes)
	{
		if (plane->name == name)
			return plane;
	}
	for (auto& sphere : spheres)
	{
		if (sphere->name == name)
			return sphere;
	}
	for (auto& cylinder : cylinders)
	{
		if (cylinder->name == name)
			return cylinder;
	}
	for (auto& capsule : capsules)
	{
		if (capsule->name == name)
			return capsule;
	}
	for (auto& mesh : meshes)
	{
		if (mesh->name == name)
			return mesh;
	}
	return nullptr;
}

std::vector<GameObject*> GameObjectManager::getAllGameObjects()
{
	std::vector<GameObject*> allObjects;
	allObjects.reserve(cubes.size() + planes.size() + meshes.size());
	for (auto& cube : cubes)
		allObjects.push_back(cube);
	for (auto& plane : planes)
		allObjects.push_back(plane);
	for (auto& sphere : spheres)
		allObjects.push_back(sphere);
	for (auto& cylinder : cylinders)
		allObjects.push_back(cylinder);
	for (auto& capsule : capsules)
		allObjects.push_back(capsule);
	for (auto& mesh : meshes)
		allObjects.push_back(mesh);
	return allObjects;
}

std::vector<GameObject*> GameObjectManager::getAllGameObjectsOfType(objType type)
{
	std::vector<GameObject*> objects;
	switch (type) {
	case Cubes:
		for (auto& cube : cubes)
			objects.push_back(cube);
		break;
	case Planes:
		for (auto& plane : planes)
			objects.push_back(plane);
		break;
	case Meshes:
		for (auto& mesh : meshes)
			objects.push_back(mesh);
		break;
	case Spheres:
		for (auto& sphere : spheres)
			objects.push_back(sphere);
		break;
	case Cylinders:
		for (auto& cylinder : cylinders)
			objects.push_back(cylinder);
		break;
	case Capsules:
		for (auto& capsule : capsules)
			objects.push_back(capsule);
		break;
	}

	return objects;
}

void GameObjectManager::getInspectorUI()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x, 0.0f), ImGuiCond_FirstUseEver, ImVec2(1.0f, 0.0f));

	ImGuiWindowFlags inspectorFlags = 0;
	inspectorFlags |= ImGuiWindowFlags_NoMove;
	inspectorFlags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Inspector", nullptr, inspectorFlags);

	int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;
	for (; i < cubes.size(); i++)
	{
		ImGui::PushID(i);
		cubes[i]->getInspectorUI();
		ImGui::PopID();
	}
	for (; j < planes.size(); j++)
	{
		ImGui::PushID(i + j);
		planes[j]->getInspectorUI();
		ImGui::PopID();
	}
	for (; k < spheres.size(); k++)
	{
		ImGui::PushID(i + j + k);
		spheres[k]->getInspectorUI();
		ImGui::PopID();
	}
	for (; l < cylinders.size(); l++)
	{
		ImGui::PushID(i + j + k + l);
		cylinders[l]->getInspectorUI();
		ImGui::PopID();
	}
	for (; m < capsules.size(); m++)
	{
		ImGui::PushID(i + j + k + l + m);
		capsules[m]->getInspectorUI();
		ImGui::PopID();
	}
	for (; n < meshes.size(); n++)
	{
		ImGui::PushID(i + j + k + l + m + n);
		meshes[n]->getInspectorUI();
		ImGui::PopID();
	}
	ImGui::End();
}

void GameObjectManager::getObjectSpawnUI()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Spawn"))
		{
			if (ImGui::MenuItem("Spawn Cube"))
			{
				this->spawnCube();
			}
			if (ImGui::MenuItem("Spawn Plane"))
			{
				this->spawnPlane();
			}
			if (ImGui::MenuItem("Spawn Sphere"))
			{
				this->spawnSphere();
			}
			if (ImGui::MenuItem("Spawn Cylinder"))
			{
				this->spawnCylinder();
			}
			if (ImGui::MenuItem("Spawn Capsule"))
			{
				this->spawnCapsule();
			}
			if (ImGui::MenuItem("Spawn Physics Cube"))
			{
				this->spawnP6Cube();
			}
			if (ImGui::MenuItem("Spawn Physics Plane"))
			{
				this->spawnP6Plane();
			}
			if (ImGui::MenuItem("Spawn Physics Sphere"))
			{
				this->spawnP6Sphere();
			}
			if (ImGui::MenuItem("Spawn Physics Cylinder"))
			{
				this->spawnP6Cylinder();
			}
			if (ImGui::MenuItem("Spawn Physics Capsule"))
			{
				this->spawnP6Capsule();
			}
			if (ImGui::MenuItem("Spawn Mesh"))
			{
				meshScreen = true;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

		static char meshPath[128] = "Assets\\Meshes\\";
		static char texturePath[128] = "Assets\\Textures\\";

		static char meshPathOut[128];
		static char texturePathOut[128];

		static Mesh* loadedMesh = nullptr;
		static Texture* loadedTexture = nullptr;
		if (meshScreen)
		{
			ImGui::Begin("Spawn Mesh Menu", &meshScreen, ImGuiWindowFlags_AlwaysAutoResize);
			static char buf[128] = ""; ImGui::InputText("Mesh File Name", buf, 64);
			static char buf2[128] = ""; ImGui::InputText("Texture File Name", buf2, 64);
			static float pos[3] = { 0.0f,0.0f,0.0f };
			ImGui::InputFloat3("Position", pos);
			static float scale = 1.0f;
			ImGui::InputFloat("Scale", &scale, 0.01f, 1.f, "%.3f");

			if (ImGui::Button("Load"))
			{
				strcat(meshPath, buf);
				size_t cSize = strlen(meshPath) + 1;
				wchar_t* wc = new wchar_t[cSize];
				mbstowcs(wc, meshPath, cSize);
				loadedMesh = MeshManager::getInstance()->createMeshFromFile(wc);
				strcpy(meshPathOut, meshPath);
				strcpy(meshPath, "Assets\\Meshes\\");

				strcat(texturePath, buf2);
				size_t cSize2 = strlen(texturePath) + 1;
				wchar_t* wc2 = new wchar_t[cSize2];
				mbstowcs(wc2, texturePath, cSize2);
				loadedTexture = TextureManager::getInstance()->createTextureFromFile(wc2);
				strcpy(texturePathOut, texturePath);
				strcpy(texturePath, "Assets\\Textures\\");

				if (loadedMesh != nullptr)
				{
					std::cout << "Mesh Loading\n";

					if (loadedTexture != nullptr)
						std::cout << "Texture Loading\n";
					else
					{
						DebugUIManager::getInstance()->Log("Texture Loading Failed!");
						loadedTexture = TextureManager::getInstance()->createTextureFromFile(L"Assets\\Textures\\error.jpg");
						strcpy(texturePathOut, "Assets\\Textures\\error.jpg");
					}


					MeshObject* m;
					if (findGameObjectByName("Mesh " + std::to_string(this->meshes.size() + 1)) == nullptr)
						m = new MeshObject("Mesh " + std::to_string(this->meshes.size() + 1), loadedMesh, loadedTexture);
					else
						m = new MeshObject("Mesh " + std::to_string(this->meshes.size() + 2), loadedMesh, loadedTexture);
					m->setPosition(Vector3D(pos[0], pos[1], pos[2]));
					m->setScale(Vector3D(scale));
					m->meshLoc = meshPathOut;
					m->textureLoc = texturePathOut;
					this->meshes.push_back(m);
				}
				else
					DebugUIManager::getInstance()->Log("Mesh Loading Failed!");
			}
			ImGui::End();
		}
	}
	SceneCameraHolder::getInstance()->getCamera()->setCameraMovement(!meshScreen);
}

void GameObjectManager::spawnCube()
{
	Cube* newCube = new Cube(*templateCube);
	if (findGameObjectByName("Cube " + std::to_string(cubes.size() + 1)) != nullptr)
		newCube->name = "Cube " + std::to_string(cubes.size() + 2);
	else
		newCube->name = "Cube " + std::to_string(cubes.size() + 1);

	newCube->setPosition(0, 0, 0);
	newCube->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newCube->setRotation(Vector3D(0.0f, 0.0f, 0.0f));

	newCube->reconstructMatrix();
	cubes.push_back(newCube);
}

void GameObjectManager::spawnPlane()
{
	Plane* newPlane = new Plane(*templatePlane);
	if (findGameObjectByName("Plane " + std::to_string(planes.size() + 1)) != nullptr)
		newPlane->name = "Plane " + std::to_string(planes.size() + 2);
	else
		newPlane->name = "Plane " + std::to_string(planes.size() + 1);

	newPlane->setPosition(0, 0, 0);
	newPlane->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newPlane->setRotation(Vector3D(0.0f, 0.0f, 0.0f));

	newPlane->reconstructMatrix();
	planes.push_back(newPlane);
}

void GameObjectManager::spawnSphere()
{
	Sphere* newSphere = new Sphere(*templateSphere);
	if (findGameObjectByName("Sphere " + std::to_string(spheres.size() + 1)) != nullptr)
		newSphere->name = "Sphere " + std::to_string(spheres.size() + 2);
	else
		newSphere->name = "Sphere " + std::to_string(spheres.size() + 1);

	newSphere->setPosition(0, 0, 0);
	newSphere->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newSphere->setRotation(Vector3D(0.0f, 0.0f, 0.0f));

	newSphere->reconstructMatrix();
	spheres.push_back(newSphere);
}

void GameObjectManager::spawnCylinder()
{
	Cylinder* newCylinder = new Cylinder(*templateCylinder);
	if (findGameObjectByName("Cylinder " + std::to_string(cylinders.size() + 1)) != nullptr)
		newCylinder->name = "Cylinder " + std::to_string(cylinders.size() + 2);
	else
		newCylinder->name = "Cylinder " + std::to_string(cylinders.size() + 1);

	newCylinder->setPosition(0, 0, 0);
	newCylinder->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newCylinder->setRotation(Vector3D(0.0f, 0.0f, 0.0f));

	newCylinder->reconstructMatrix();
	cylinders.push_back(newCylinder);
}

void GameObjectManager::spawnCapsule()
{
	Capsule* newCapsule = new Capsule(*templateCapsule);
	if (findGameObjectByName("Capsule " + std::to_string(capsules.size() + 1)) != nullptr)
		newCapsule->name = "Capsule " + std::to_string(capsules.size() + 2);
	else
		newCapsule->name = "Capsule " + std::to_string(capsules.size() + 1);

	newCapsule->setPosition(0, 0, 0);
	newCapsule->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newCapsule->setRotation(Vector3D(0.0f, 0.0f, 0.0f));

	newCapsule->reconstructMatrix();
	capsules.push_back(newCapsule);
}



void GameObjectManager::spawnP6Cube()
{
	Cube* newPhysicsCube = new Cube(*templateCube);
	if (findGameObjectByName("P6 Cube " + std::to_string(cubes.size() + 1)) != nullptr)
		newPhysicsCube->name = "P6 Cube " + std::to_string(cubes.size() + 2);
	else
		newPhysicsCube->name = "P6 Cube " + std::to_string(cubes.size() + 1);

	newPhysicsCube->setPosition(0, 0, 0);
	newPhysicsCube->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newPhysicsCube->setRotation(Vector3D(0.0f, 0.0f, 0.0f));

	newPhysicsCube->reconstructMatrix();
	cubes.push_back(newPhysicsCube);

	newPhysicsCube->attachComponent(new PhysicsComponent(newPhysicsCube->name + " P6 Component", newPhysicsCube));
}

void GameObjectManager::spawnP6Plane()
{
	Plane* newPhysicsPlane = new Plane(*templatePlane);
	if (findGameObjectByName("P6 Plane " + std::to_string(planes.size() + 1)) != nullptr)
		newPhysicsPlane->name = "P6 Plane " + std::to_string(planes.size() + 2);
	else
		newPhysicsPlane->name = "P6 Plane " + std::to_string(planes.size() + 1);

	newPhysicsPlane->setPosition(0, -5, 0);
	newPhysicsPlane->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newPhysicsPlane->setRotation(Vector3D(0.0f, 0.0f, 0.0f));

	newPhysicsPlane->reconstructMatrix();
	planes.push_back(newPhysicsPlane);

	newPhysicsPlane->attachComponent(new PhysicsComponent(newPhysicsPlane->name + " P6 Component", newPhysicsPlane));
}

void GameObjectManager::spawnP6Sphere()
{
	Sphere* newPhysicsSphere = new Sphere(*templateSphere);
	if (findGameObjectByName("P6 Sphere " + std::to_string(spheres.size() + 1)) != nullptr)
		newPhysicsSphere->name = "P6 Sphere " + std::to_string(spheres.size() + 2);
	else
		newPhysicsSphere->name = "P6 Sphere " + std::to_string(spheres.size() + 1);
	newPhysicsSphere->setPosition(0, 0, 0);
	newPhysicsSphere->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newPhysicsSphere->setRotation(Vector3D(0.0f, 0.0f, 0.0f));
	newPhysicsSphere->reconstructMatrix();
	spheres.push_back(newPhysicsSphere);
	newPhysicsSphere->attachComponent(new PhysicsComponent(newPhysicsSphere->name + " P6 Component", newPhysicsSphere));
}

void GameObjectManager::spawnP6Cylinder()
{
	Cylinder* newPhysicsCylinder = new Cylinder(*templateCylinder);
	if (findGameObjectByName("P6 Cylinder " + std::to_string(cylinders.size() + 1)) != nullptr)
		newPhysicsCylinder->name = "P6 Cylinder " + std::to_string(cylinders.size() + 2);
	else
		newPhysicsCylinder->name = "P6 Cylinder " + std::to_string(cylinders.size() + 1);
	newPhysicsCylinder->setPosition(0, 0, 0);
	newPhysicsCylinder->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newPhysicsCylinder->setRotation(Vector3D(0.0f, 0.0f, 0.0f));
	newPhysicsCylinder->reconstructMatrix();
	cylinders.push_back(newPhysicsCylinder);
	newPhysicsCylinder->attachComponent(new PhysicsComponent(newPhysicsCylinder->name + " P6 Component", newPhysicsCylinder));
}

void GameObjectManager::spawnP6Capsule()
{
	Capsule* newPhysicsCapsule = new Capsule(*templateCapsule);
	if (findGameObjectByName("P6 Capsule " + std::to_string(capsules.size() + 1)) != nullptr)
		newPhysicsCapsule->name = "P6 Capsule " + std::to_string(capsules.size() + 2);
	else
		newPhysicsCapsule->name = "P6 Capsule " + std::to_string(capsules.size() + 1);
	newPhysicsCapsule->setPosition(0, 0, 0);
	newPhysicsCapsule->setScale(Vector3D(1.0f, 1.0f, 1.0f));
	newPhysicsCapsule->setRotation(Vector3D(0.0f, 0.0f, 0.0f));
	newPhysicsCapsule->reconstructMatrix();
	capsules.push_back(newPhysicsCapsule);
	newPhysicsCapsule->attachComponent(new PhysicsComponent(newPhysicsCapsule->name + " P6 Component", newPhysicsCapsule));
}











//with data

void GameObjectManager::spawnCube(Vector3D pos, Vector3D scale, Vector3D rot)
{
	Cube* newCube = new Cube(*templateCube);
	if (findGameObjectByName("Cube " + std::to_string(cubes.size() + 1)) != nullptr)
		newCube->name = "Cube " + std::to_string(cubes.size() + 2);
	else
		newCube->name = "Cube " + std::to_string(cubes.size() + 1);

	newCube->setPosition(pos.x, pos.y, pos.z);
	newCube->setScale(scale);
	newCube->setRotation(rot);

	newCube->reconstructMatrix();
	cubes.push_back(newCube);
}

void GameObjectManager::spawnPlane(Vector3D pos, Vector3D scale, Vector3D rot)
{
	Plane* newPlane = new Plane(*templatePlane);
	if (findGameObjectByName("Plane " + std::to_string(planes.size() + 1)) != nullptr)
		newPlane->name = "Plane " + std::to_string(planes.size() + 2);
	else
		newPlane->name = "Plane " + std::to_string(planes.size() + 1);

	newPlane->setPosition(pos.x, pos.y, pos.z);
	newPlane->setScale(scale);
	newPlane->setRotation(rot);

	newPlane->reconstructMatrix();
	planes.push_back(newPlane);
}

void GameObjectManager::spawnMesh(Vector3D pos, Vector3D scale, Vector3D rot, const char meshPath[128], const char texturePath[128])
{
	static Mesh* loadedMesh = nullptr;
	static Texture* loadedTexture = nullptr;
	std::cout << meshPath;
	std::cout << texturePath;

	size_t cSize = strlen(meshPath) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, meshPath, cSize);
	loadedMesh = MeshManager::getInstance()->createMeshFromFile(wc);

	size_t cSize2 = strlen(texturePath) + 1;
	wchar_t* wc2 = new wchar_t[cSize2];
	mbstowcs(wc2, texturePath, cSize2);
	loadedTexture = TextureManager::getInstance()->createTextureFromFile(wc2);

	MeshObject* m;
	if (findGameObjectByName("Mesh " + std::to_string(this->meshes.size() + 1)) == nullptr)
		m = new MeshObject("Mesh " + std::to_string(this->meshes.size() + 1), loadedMesh, loadedTexture);
	else
		m = new MeshObject("Mesh " + std::to_string(this->meshes.size() + 2), loadedMesh, loadedTexture);
	m->setPosition(pos.x, pos.y, pos.z);
	m->setScale(scale);
	m->setRotation(rot);

	this->meshes.push_back(m);
}

void GameObjectManager::spawnP6Cube(Vector3D pos, Vector3D scale, Vector3D rot)
{
	Cube* newPhysicsCube = new Cube(*templateCube);
	if (findGameObjectByName("P6 Cube " + std::to_string(cubes.size() + 1)) != nullptr)
		newPhysicsCube->name = "P6 Cube " + std::to_string(cubes.size() + 2);
	else
		newPhysicsCube->name = "P6 Cube " + std::to_string(cubes.size() + 1);

	newPhysicsCube->setPosition(pos.x, pos.y, pos.z);
	newPhysicsCube->setScale(scale);
	newPhysicsCube->setRotation(rot);

	newPhysicsCube->reconstructMatrix();
	cubes.push_back(newPhysicsCube);

	newPhysicsCube->attachComponent(new PhysicsComponent(newPhysicsCube->name + " P6 Component", newPhysicsCube));
}

void GameObjectManager::spawnP6Plane(Vector3D pos, Vector3D scale, Vector3D rot)
{
	Plane* newPhysicsPlane = new Plane(*templatePlane);
	if (findGameObjectByName("P6 Plane " + std::to_string(planes.size() + 1)) != nullptr)
		newPhysicsPlane->name = "P6 Plane " + std::to_string(planes.size() + 2);
	else
		newPhysicsPlane->name = "P6 Plane " + std::to_string(planes.size() + 1);

	newPhysicsPlane->setPosition(pos.x, pos.y, pos.z);
	newPhysicsPlane->setScale(scale);
	newPhysicsPlane->setRotation(rot);

	newPhysicsPlane->reconstructMatrix();
	planes.push_back(newPhysicsPlane);

	newPhysicsPlane->attachComponent(new PhysicsComponent(newPhysicsPlane->name + " P6 Component", newPhysicsPlane));
}

void GameObjectManager::spawnP6Sphere(Vector3D pos, Vector3D scale, Vector3D rot)
{
	Sphere* newPhysicsSphere = new Sphere(*templateSphere);
	if (findGameObjectByName("P6 Sphere " + std::to_string(spheres.size() + 1)) != nullptr)
		newPhysicsSphere->name = "P6 Sphere " + std::to_string(spheres.size() + 2);
	else
		newPhysicsSphere->name = "P6 Sphere " + std::to_string(spheres.size() + 1);
	newPhysicsSphere->setPosition(pos.x, pos.y, pos.z);
	newPhysicsSphere->setScale(scale);
	newPhysicsSphere->setRotation(rot);
	newPhysicsSphere->reconstructMatrix();
	spheres.push_back(newPhysicsSphere);
	newPhysicsSphere->attachComponent(new PhysicsComponent(newPhysicsSphere->name + " P6 Component", newPhysicsSphere));
}

void GameObjectManager::spawnP6Cylinder(Vector3D pos, Vector3D scale, Vector3D rot)
{
	Cylinder* newPhysicsCylinder = new Cylinder(*templateCylinder);
	if (findGameObjectByName("P6 Cylinder " + std::to_string(cylinders.size() + 1)) != nullptr)
		newPhysicsCylinder->name = "P6 Cylinder " + std::to_string(cylinders.size() + 2);
	else
		newPhysicsCylinder->name = "P6 Cylinder " + std::to_string(cylinders.size() + 1);
	newPhysicsCylinder->setPosition(pos.x, pos.y, pos.z);
	newPhysicsCylinder->setScale(scale);
	newPhysicsCylinder->setRotation(rot);
	newPhysicsCylinder->reconstructMatrix();
	cylinders.push_back(newPhysicsCylinder);
	newPhysicsCylinder->attachComponent(new PhysicsComponent(newPhysicsCylinder->name + " P6 Component", newPhysicsCylinder));
}

void GameObjectManager::spawnP6Capsule(Vector3D pos, Vector3D scale, Vector3D rot)
{
	Capsule* newPhysicsCapsule = new Capsule(*templateCapsule);
	if (findGameObjectByName("P6 Capsule " + std::to_string(capsules.size() + 1)) != nullptr)
		newPhysicsCapsule->name = "P6 Capsule " + std::to_string(capsules.size() + 2);
	else
		newPhysicsCapsule->name = "P6 Capsule " + std::to_string(capsules.size() + 1);
	newPhysicsCapsule->setPosition(pos.x, pos.y, pos.z);
	newPhysicsCapsule->setScale(scale);
	newPhysicsCapsule->setRotation(rot);
	newPhysicsCapsule->reconstructMatrix();
	capsules.push_back(newPhysicsCapsule);
	newPhysicsCapsule->attachComponent(new PhysicsComponent(newPhysicsCapsule->name + " P6 Component", newPhysicsCapsule));
}
