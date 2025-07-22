#pragma once
#include <string>

class GameObject;

class Component
{
public:
	typedef std::string String;
	enum ComponentType {NotSet = -1, Script = 0, Renderer = 1, Input = 2, Physics = 3};

	Component(String name, ComponentType type, GameObject* owner);
	~Component();

	void attachOwner(GameObject* owner);
	void detachOwner();
	GameObject* getOwner();
	ComponentType getType();
	String getName();

	virtual void perform(float deltaTime) = 0;

protected:
	GameObject* owner;
	ComponentType type;
	String name;

};

