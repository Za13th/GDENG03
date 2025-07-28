#include "GameObjectManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "BaseComponentSystem.h"
#include "SceneCameraHolder.h"
#include "PhysicsComponent.h"

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
	}
}

void GameObjectManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		sharedInstance->templateCube->release();
		sharedInstance->templatePlane->release();

		for (int i = 0; i < sharedInstance->meshes.size(); i++)
			sharedInstance->meshes[i]->release();

		sharedInstance->cubes.clear();
		sharedInstance->planes.clear();
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
	for (auto& mesh : meshes)
		allObjects.push_back(mesh);
	return allObjects;
}

void GameObjectManager::getInspectorUI()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x, 0.0f), ImGuiCond_FirstUseEver, ImVec2(1.0f, 0.0f));

	ImGuiWindowFlags inspectorFlags = 0;
	inspectorFlags |= ImGuiWindowFlags_NoMove;
	inspectorFlags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Inspector", nullptr, inspectorFlags);

	int i = 0, j = 0, k = 0;
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
	for (; k < meshes.size(); k++)
	{
		ImGui::PushID(i + j + k);
		meshes[k]->getInspectorUI();
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
			if (ImGui::MenuItem("Spawn Physics Cube"))
			{
				this->spawnP6Cube();
			}
			if (ImGui::MenuItem("Spawn Physics Plane"))
			{
				this->spawnP6Plane();
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
				strcpy(meshPath, "Assets\\Meshes\\");

				strcat(texturePath, buf2);
				size_t cSize2 = strlen(texturePath) + 1;
				wchar_t* wc2 = new wchar_t[cSize2];
				mbstowcs(wc2, texturePath, cSize2);
				loadedTexture = TextureManager::getInstance()->createTextureFromFile(wc2);
				strcpy(texturePath, "Assets\\Textures\\");

				if (loadedMesh != nullptr)
				{
					std::cout << "Mesh Loading\n";

					if (loadedTexture != nullptr)
						std::cout << "Texture Loading\n";
					else
					{
						std::cout << "Texture Loading Failed\n";
						loadedTexture = TextureManager::getInstance()->createTextureFromFile(L"Assets\\Textures\\error.jpg");
					}


					auto m = new MeshObject("Mesh " + std::to_string(this->meshes.size() + 1), loadedMesh, loadedTexture);
					m->setPosition(Vector3D(pos[0], pos[1], pos[2]));
					m->setScale(Vector3D(scale));
					this->meshes.push_back(m);
				}
				else
					std::cout << "Mesh Loading Failed" << std::endl;
			}
			ImGui::End();
		}
	}
	SceneCameraHolder::getInstance()->getCamera()->setCameraMovement(!meshScreen);
}

void GameObjectManager::spawnCube()
{
	Cube* newCube = new Cube(*templateCube);
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
	newPlane->name = "Plane " + std::to_string(planes.size() + 1);

	newPlane->setPosition(0, 0, 0);
	newPlane->setScale(Vector3D(3.0f, 0.1f, 3.0f));
	newPlane->setRotation(Vector3D(0.0f, 0.0f, 0.0f));

	newPlane->reconstructMatrix();
	planes.push_back(newPlane);
}

void GameObjectManager::spawnP6Cube()
{
	Cube* newPhysicsCube = new Cube(*templateCube);
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
	newPhysicsPlane->name = "P6 Plane " + std::to_string(planes.size() + 1);

	newPhysicsPlane->setPosition(0, -5, 0);
	newPhysicsPlane->setScale(Vector3D(3.0f, 0.1f, 3.0f));
	newPhysicsPlane->setRotation(Vector3D(0.0f, 0.0f, 0.0f));

	newPhysicsPlane->reconstructMatrix();
	planes.push_back(newPhysicsPlane);

	newPhysicsPlane->attachComponent(new PhysicsComponent(newPhysicsPlane->name + " P6 Component", newPhysicsPlane));
}