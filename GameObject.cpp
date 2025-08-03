#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "TextureComponent.h"
#include "BaseComponentSystem.h"
#include "GameObjectManager.h"
#include "DebugUIManager.h"
#include "UndoRedoManager.h"
#include "UndoRedoAction.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

GameObject::GameObject(std::string name) : name(name), localPosition(0.0f, 0.0f, 0.0f), localScale(1.0f, 1.0f, 1.0f), localRotation(0.0f, 0.0f, 0.0f)
{

}

GameObject::~GameObject()
{

}


void GameObject::setPosition(float x, float y, float z)
{
	localPosition = Vector3D(x, y, z);

	this->position[0] = localPosition.x;
	this->position[1] = localPosition.y;
	this->position[2] = localPosition.z;
}

void GameObject::setPosition(Vector3D position)
{
	localPosition = position;

	this->position[0] = localPosition.x;
	this->position[1] = localPosition.y;
	this->position[2] = localPosition.z;
}

Vector3D GameObject::getLocalPosition()
{
	return localPosition;
}

void GameObject::setScale(float x, float y, float z)
{
	localScale = Vector3D(x, y, z);

	this->scale[0] = localScale.x;
	this->scale[1] = localScale.y;
	this->scale[2] = localScale.z;
}

void GameObject::setScale(Vector3D scale)
{
	localScale = scale;

	this->scale[0] = localScale.x;
	this->scale[1] = localScale.y;
	this->scale[2] = localScale.z;
}

Vector3D GameObject::getLocalScale()
{
	return localScale;
}

void GameObject::setRotation(float x, float y, float z)
{
	localRotation = Vector3D(x, y, z);

	this->rotation[0] = localRotation.x * (180.0 / M_PI);
	this->rotation[1] = localRotation.y * (180.0 / M_PI);
	this->rotation[2] = localRotation.z * (180.0 / M_PI);
}

void GameObject::setRotation(Vector3D rotation)
{
	localRotation = rotation;
}

Vector3D GameObject::getLocalRotation()
{
	return localRotation;
}

void GameObject::getInspectorUI()
{

	if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat3("Position", &position[0]);
		ImGui::InputFloat3("Scale", &scale[0]);
		ImGui::InputFloat3("Rotation", &rotation[0]);

		if (GameStateManager::getInstance()->getGameState() != GameStateManager::Play)
		{
			if (ImGui::Button("Apply"))
			{
				UndoRedoAction* act = new UndoRedoAction(true, this,
					localPosition, Vector3D(position[0], position[1], position[2]),
					localRotation, Vector3D(rotation[0] * (M_PI / 180.0), rotation[1] * (M_PI / 180.0), rotation[2] * (M_PI / 180.0)),
					localScale, Vector3D(scale[0], scale[1], scale[2]));

				localPosition = Vector3D(position[0], position[1], position[2]);
				localScale = Vector3D(scale[0], scale[1], scale[2]);
				UndoRedoManager::getInstance()->addToHistory(act);

				localRotation = Vector3D(rotation[0] * (M_PI / 180.0), rotation[1] * (M_PI / 180.0), rotation[2] * (M_PI / 180.0));
				this->reconstructMatrix();

				if (this->findComponentByType(Component::Physics, name + " P6 Component"))
				{
					PhysicsComponent* physicsComponent = static_cast<PhysicsComponent*>(this->findComponentByType(Component::Physics, name + " P6 Component"));
					if (physicsComponent)
					{
						physicsComponent->adjustRigidbody();
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete"))
			{
				if (GameObjectManager::getInstance()->getCurrentObject() == this)
				{
					GameObjectManager::getInstance()->inspectorWindowOpen = false;
				}

				if (this->findComponentByType(Component::Physics, name + " P6 Component"))
				{
					PhysicsComponent* physicsComponent = static_cast<PhysicsComponent*>(this->findComponentByType(Component::Physics, name + " P6 Component"));
					if (physicsComponent)
					{
						BaseComponentSystem::getInstance()->getPhysicsSystem()->unregisterComponent(physicsComponent);
					}
				}

				GameObjectManager::getInstance()->removeGameObject(this);
			}
			ImGui::SameLine();
			if (ImGui::Button("More Info"))
			{
				GameObjectManager::getInstance()->setCurrentObject(this);
				GameObjectManager::getInstance()->inspectorWindowOpen = true;
			}
		}

	}
	
}

void GameObject::getInspectorUIPlus()
{
	String s = "Inspector: " + this->name;

	static char texturePath[128] = "Assets\\Textures\\";
	static char texturePathOut[128];
	static Texture* loadedTexture = nullptr;

	if (GameObjectManager::getInstance()->inspectorWindowOpen)
	{
		ImGui::Begin(s.c_str(), &GameObjectManager::getInstance()->inspectorWindowOpen, ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::CollapsingHeader("Texture Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (!this->findComponentByType(Component::Material, name + " TX Component"))
			{
				if (ImGui::Button("Add Texture Component"))
				{
					this->attachComponent(new TextureComponent(name + " TX Component", this));
				}
			}
			else
			{
				static char buf2[128] = "";
				ImGui::InputText("Texture File Name", buf2, 64);

				if (ImGui::Button("Change Texture"))
				{
					strcat(texturePath, buf2);
					size_t cSize2 = strlen(texturePath) + 1;
					wchar_t* wc2 = new wchar_t[cSize2];
					mbstowcs(wc2, texturePath, cSize2);
					loadedTexture = TextureManager::getInstance()->createTextureFromFile(wc2);
					strcpy(texturePathOut, texturePath);
					strcpy(texturePath, "Assets\\Textures\\");

					if (loadedTexture != nullptr)
					{
						((TextureComponent*)(this->findComponentByType(Component::Material, name + " TX Component")))->changeTexture(wc2);
					}
					else
					{
						DebugUIManager::getInstance()->Log("Texture Loading Failed!");
					}

				}
				ImGui::SameLine();
				if (ImGui::Button("Remove Component"))
				{
					this->detachComponent(this->findComponentByType(Component::Material, name + " TX Component"));
				}

			}
		}
		if (this->objectType != ObjectType::MeshObject && ImGui::CollapsingHeader("Physics Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (!this->findComponentByType(Component::Physics, name + " P6 Component"))
			{
				if (ImGui::Button("Add Physics Component"))
				{
					this->attachComponent(new PhysicsComponent(name + " P6 Component", this));
				}
			}
			else
			{
				if (ImGui::Button("Remove Component"))
				{
					PhysicsComponent* physicsComponent = static_cast<PhysicsComponent*>(this->findComponentByType(Component::Physics, name + " P6 Component"));
					if (physicsComponent)
					{
						BaseComponentSystem::getInstance()->getPhysicsSystem()->unregisterComponent(physicsComponent);
					}
						this->detachComponent(this->findComponentByType(Component::Physics, name + " P6 Component"));
				}
			}
		}
		ImGui::End();
	}
		

}

void GameObject::reconstructMatrix()
{
	Matrix4x4 matrix, temp;

	matrix.setScale(this->getLocalScale());

	temp.setRotationZ(this->getLocalRotation().z);
	matrix *= temp;
	temp.setRotationY(this->getLocalRotation().y);
	matrix *= temp;
	temp.setRotationX(this->getLocalRotation().x);
	matrix *= temp;

	temp.setTranslation(this->getLocalPosition());
	matrix *= temp;

	this->localMatrix = matrix;
}

void GameObject::reconstructVectors()
{
	this->localPosition = localMatrix.getTranslation();

	Vector3D
		x = { 1,0,0 },
		y = { 0,1,0 },
		z = { 0,0,1 };

	this->localRotation.x = std::acosf(this->localMatrix.getXDirection().dot(x));
	this->localRotation.y = std::acosf(this->localMatrix.getYDirection().dot(y));
	this->localRotation.z = std::acosf(this->localMatrix.getZDirection().dot(z));


}

void GameObject::setLocalMatrix(float* matrix)
{

	this->localMatrix.m[0][0] = matrix[0];
	this->localMatrix.m[0][1] = matrix[1];
	this->localMatrix.m[0][2] = matrix[2];
	this->localMatrix.m[0][3] = matrix[3];

	this->localMatrix.m[1][0] = matrix[4];
	this->localMatrix.m[1][1] = matrix[5];
	this->localMatrix.m[1][2] = matrix[6];
	this->localMatrix.m[1][3] = matrix[7];

	this->localMatrix.m[2][0] = matrix[8];
	this->localMatrix.m[2][1] = matrix[9];
	this->localMatrix.m[2][2] = matrix[10];
	this->localMatrix.m[2][3] = matrix[11];

	this->localMatrix.m[3][0] = matrix[12];
	this->localMatrix.m[3][1] = matrix[13];
	this->localMatrix.m[3][2] = matrix[14];
	this->localMatrix.m[3][3] = matrix[15];


	this->reconstructVectors();
}

float* GameObject::getPhysicsLocalMatrix()
{
	Matrix4x4 temp;
	this->localMatrix.setIdentity();
	//this->localMatrix.setScale(localScale);


	temp.setRotationZ(localRotation.z);
	this->localMatrix *= temp;
	temp.setRotationY(localRotation.y);
	this->localMatrix *= temp;
	temp.setRotationX(localRotation.x);
	this->localMatrix *= temp;

	temp.setTranslation(localPosition);
	this->localMatrix *= temp;

	static float matrix[16];

	matrix[0] = this->localMatrix.m[0][0];
	matrix[1] = this->localMatrix.m[0][1];
	matrix[2] = this->localMatrix.m[0][2];
	matrix[3] = this->localMatrix.m[0][3];

	matrix[4] = this->localMatrix.m[1][0];
	matrix[5] = this->localMatrix.m[1][1];
	matrix[6] = this->localMatrix.m[1][2];
	matrix[7] = this->localMatrix.m[1][3];

	matrix[8] = this->localMatrix.m[2][0];
	matrix[9] = this->localMatrix.m[2][1];
	matrix[10] = this->localMatrix.m[2][2];
	matrix[11] = this->localMatrix.m[2][3];

	matrix[12] = this->localMatrix.m[3][0];
	matrix[13] = this->localMatrix.m[3][1];
	matrix[14] = this->localMatrix.m[3][2];
	matrix[15] = this->localMatrix.m[3][3];

	return &matrix[0];

}

void GameObject::attachComponent(Component* component)
{
	if (component)
	{
		components.push_back(component);
		component->attachOwner(this);
	}
}

void GameObject::detachComponent(Component* component)
{
	if (component)
	{
		auto it = std::find(components.begin(), components.end(), component);
		if (it != components.end())
		{
			components.erase(it);
			component->detachOwner();
		}
	}
}

void GameObject::detachAllComponents()
{
	for (auto& component : components)
	{
		component->detachOwner();
	}
	components.clear();
}

Component* GameObject::findComponentByName(std::string name)
{
	for (auto& component : components)
	{
		if (component->getName() == name)
		{
			return component;
		}
	}
	return nullptr;
}

Component* GameObject::findComponentByType(Component::ComponentType type, std::string name)
{
	for (auto& component : components)
	{
		if (component->getType() == type && component->getName() == name)
		{
			return component;
		}
	}
	return nullptr;
}

std::vector<Component::ComponentType> GameObject::getAttachedComponentTypes()
{
	std::vector<Component::ComponentType> list;
	bool notset = false, script = false, renderer = false, input = false, physics = false, material = false;
	for (auto& component : components)
	{
		switch (component->getType()) {
		case Component::NotSet:
			notset = true;
			break;
		case Component::Script:
			script = true;
			break;
		case Component::Renderer:
			renderer = true;
			break;
		case Component::Input:
			input = true;
			break;
		case Component::Physics:
			physics = true;
			break;
		case Component::Material:
			material = true;
			break;
		}
	}

	if (notset) list.push_back(Component::NotSet);
	if (script) list.push_back(Component::Script);
	if (renderer) list.push_back(Component::Renderer);
	if (input) list.push_back(Component::Input);
	if (physics) list.push_back(Component::Physics);
	if (material) list.push_back(Component::Material);

	return list;
}

