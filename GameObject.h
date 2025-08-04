#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Component.h"
#include "TextureComponent.h"

class VertexShader;
class PixelShader;

class GameObject
{
public:

	typedef std::string String;
	typedef std::unordered_map<String, Component*> ComponentTable;
	typedef std::vector<Component*> ComponentList;


	enum ObjectType { MeshObject = -1, Cube = 0, Plane = 1, Sphere = 2, Cylinder = 3, Capsule = 4 };


	GameObject(std::string name);
	~GameObject();

	virtual void update(float deltaTime) = 0;
	virtual void draw(int width, int height, VertexShader* vs, PixelShader* ps) = 0;

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D position);
	Vector3D getLocalPosition();

	void setScale(float x, float y, float z);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D rotation);
	Vector3D getLocalRotation();

	void toggleActive() { if (this->active == false) active = true;  else if (this->active == true) active = false;}
	bool isActive() { return this->active; }

	void getInspectorUI();
	void getInspectorUIPlus();
	void reconstructMatrix();
	void reconstructVectors();

	void setLocalMatrix(float* matrix);
	float* getPhysicsLocalMatrix();

	void attachComponent(Component* component);
	void detachComponent(Component* component);
	void detachAllComponents();

	Component* findComponentByName(std::string name);
	Component* findComponentByType(Component::ComponentType type, std::string name);
	ComponentList getComponentsOfType(Component::ComponentType type) {}
	std::vector<Component::ComponentType> getAttachedComponentTypes();

	std::string name;
	ObjectType objectType;
	std::string meshLoc;
	std::string textureLoc;

protected:
	Vector3D localPosition = Vector3D(0.f, 0.f, 0.f);
	Vector3D localScale = Vector3D(1.f, 1.f, 1.f);
	Vector3D localRotation = Vector3D(0.f, 0.f, 0.f);;
	Matrix4x4 localMatrix;

	float m_angle = 0;

	float position[3] = { localPosition.x,localPosition.y ,localPosition.z };
	float scale[3] = { localScale.x,localScale.y,localScale.z };
	float rotation[3] = { localRotation.x,localRotation.y,localRotation.z };




	ComponentList components;

	bool overrideMatrix = false;
	bool active = true;

	virtual void awake() {}

};

