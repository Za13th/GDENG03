#define _USE_MATH_DEFINES
#include <cmath>
#include "GameObject.h"

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
	;

	if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat3("Position", &position[0]);
		ImGui::InputFloat3("Scale", &scale[0]);
		ImGui::InputFloat3("Rotation", &rotation[0]);

		if (ImGui::Button("Apply"))
		{
			localPosition = Vector3D(position[0], position[1], position[2]);
			localScale = Vector3D(scale[0], scale[1], scale[2]);
			localRotation = Vector3D(rotation[0] * (M_PI / 180.0), rotation[1] * (M_PI / 180.0), rotation[2] * (M_PI / 180.0));
			this->reconstructMatrix();
		}
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
	

	this->setPosition(matrix[12], matrix[13], matrix[14]);
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

